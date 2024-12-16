// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/AudienceServerComponent_KMK.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "KMK/Audience_KMK.h"
#include "Net/UnrealNetwork.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "chrono"
#include "HSW/HSW_ThirdPersonCharacter.h"

#include "HSW/HSW_GameState_Auditorium.h"
#include "HSW/HSW_AuditoriumGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/Virtual_KMK.h"
#include "Components/WidgetComponent.h"
#include "KMK/MyNameWidget_KMK.h"
// Sets default values for this component's properties
UAudienceServerComponent_KMK::UAudienceServerComponent_KMK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UAudienceServerComponent_KMK::BeginPlay()
{
	Super::BeginPlay();
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());

	player = Cast<AHSW_ThirdPersonCharacter> (GetWorld()->GetFirstPlayerController()->GetPawn());
	playerMesh = Cast<AHSW_ThirdPersonCharacter> (GetOwner());
	if (gi && playerMesh)
	{
		// 플레이어가 로컬 플레이어 일때
		if (playerMesh->IsLocallyControlled())
        {
			// if(gi->playerMeshNum >= 0) playerMesh->GetMesh()->SetSkeletalMesh(audienceMesh[gi->playerMeshNum]);
			// 클라이언트에서 서버로 RPC 호출
			ServerRPC_ChangeMyMesh ( gi->playerMeshNum );
			ServerRPC_SetNickName(gi->GetMyInfo().userName); // 서버에서 닉네임 설정

        }
		else
		{
			 //로컬이 아닌 경우에 플레이어의 playerMeshNum에 따라 메쉬 상태 변경
			 // 관객 : 0 또는 1
			 // 버츄얼 : -1 또는 2
			if (playerMeshNum < 0)
			{
				// 버츄얼이 안보이는 상태
				SetVirtualVisible ( playerMesh , false );
			}
			else if (playerMeshNum > 1)
			{
				// 버추얼이 보이는 상태
				SetVirtualVisible ( playerMesh , true );
			}
			else
			{
				// 관객인 경우
				// 내 모습을 보이게 만들고
				SetVirtualVisible ( playerMesh , true );
				// vip 여부에 따라 mesh를 변경함
				playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );

				// 승우가 만든 dynamic Material로 변경
				UMaterialInstanceDynamic* meshMat = playerMesh->ChangeMyMeshMat ( playerMeshNum );
				playerMesh->GetMesh()->SetMaterial(0, meshMat);
			}
		}
	}

	if (playerMesh && playerMesh->HasAuthority())
	{
		// 관객의 초기 셋팅값 변경
 		if (playerMesh->IsLocallyControlled())
        {
			
        }
        else
        {
			//playerMesh->SetActorLocation ( FVector ( 4330 , -150 , 730 ) );
			//playerMesh->SetActorRotation ( FRotator ( 0 , 180, 0 ) );
			// 현재 위치를 모든 클라이언트에 동기화
			MulticastRPC_SetInitialLocation ( gi->concertStageInfo.terrain );
        }
	}
	else
	{
		// 버츄얼 찾고 할당하는 함수
		FindVirtualCharacter ( );
	}
}

void UAudienceServerComponent_KMK::MulticastRPC_SetInitialLocation_Implementation( int32 terrain )
{
	if (!playerMesh) return;

	FVector newLocation;
	FRotator newRotation ( 0 , 180 , 0 );

	switch (terrain)
	{
	case 0:
		newLocation = FVector ( 3600 , -137 , 400 );
		break;
	case 1:
		newLocation = FVector ( 1785 , -137 , 150 );
		break;
	case 2:
		newLocation = FVector ( 1575 , -200 , 490 );
		break;
	case 3:
		newLocation = FVector ( 2540 , 0 , 300 );
		break;
	case 4:
		newLocation = FVector ( 2540 , 0 , 300 );
		break;
	case 5:
		newLocation = FVector ( 1995 , -347 , 150 );
		break;
	default:
		return;
	}

	UE_LOG ( LogTemp , Error , TEXT ( "위치 멀티캐스트" ) );

	playerMesh->SetActorLocation ( newLocation );
	playerMesh->SetActorRotation ( newRotation );
}



// 월드상에서 버츄얼 찾기
void UAudienceServerComponent_KMK::FindVirtualCharacter ( )
{
	TArray<AActor*> actorArray;
	// 태그로 검색
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Virtual"), actorArray);
    for (AActor* actor : actorArray)
    {
		// 버츄얼을 찾았다면 버츄얼에 달린 component를 검색함
        UVirtual_KMK* virtualComp = actor->FindComponentByClass<UVirtual_KMK>();
        if (virtualComp)
        {
			// 버츄얼 캐릭터가 있다면 안보이게 만듦
            virtualCharacter = virtualComp;
            virtualCharacter->SetVirtualVisible(false);
            UE_LOG(LogTemp, Warning, TEXT("Found Virtual Character: %s"), *actor->GetName());
            return; // 성공적으로 찾았으므로 종료
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Virtual Character not found, retrying..."));
    // 버츄얼을 못찾았다면, 일정 시간 후 다시 시도
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UAudienceServerComponent_KMK::FindVirtualCharacter);
}
// 닉네임 변경
void UAudienceServerComponent_KMK::OnRep_NickName ( )
{
	UpdateWidgetNick();
}

void UAudienceServerComponent_KMK::UpdateWidgetNick ( )
{
	// 나에게 달린 컴포넌트중 Name이란 태그를 찾아 mynameComp에 할당
	UWidgetComponent* myNameComp = GetOwner()->FindComponentByTag<UWidgetComponent>(TEXT("Name" ));
	if (myNameComp)
	{
        UMyNameWidget_KMK* myNameWid = Cast<UMyNameWidget_KMK>(CreateWidget(GetWorld(), nameWidgetFact));
		myNameComp->SetWidget(myNameWid);
		// 위잿을 찾았다면
        if (myNameWid)
        {
			// gi에 있는 정보값 중 내 닉네임을 찾아 보여줌
            UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
            if (gi)
            {
                myNameWid->SetMyName(userName);
                comp = myNameComp;
            }
		}
	}
}
// 다른 사람에게 내 이름을 뿌리는 부분
void UAudienceServerComponent_KMK::ServerRPC_SetNickName_Implementation ( const FString& name )
{
	userName = name;
	MultiRPC_SetNickName();
}


void UAudienceServerComponent_KMK::MultiRPC_SetNickName_Implementation ( )
{
	UpdateWidgetNick ( );
}

// Called every frame
void UAudienceServerComponent_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bVis)
	{
		//s = GetTimeDifference ( setConcertTime );
		// 서버에서 시간을 계산하고 클라이언트에게 전파
		AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetOwner ( ) );
		if (playerCharacter && playerCharacter->audienceWidget && playerCharacter->IsLocallyControlled())
		{
			// 남은 시간을 위젯에 표시
			playerCharacter->audienceWidget->CountDownText ( s );
		}

	}
	else
	{
		AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetOwner ( ) );
		// 카운트다운 UI 패널 표시
		if (playerCharacter && playerCharacter->audienceWidget)
		{
			playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
			if (playerCharacter->HasAuthority ( ))
			{
				playerCharacter->audienceWidget->SetCountDownTextVisible( );
			}
		}
	}
	if (bTime)
	{
		remainTime = GetWorld ( )->GetTimerManager ( ).GetTimerRemaining ( startCountDownHandle );
		if (player && player->audienceWidget)
		{
			if (remainTime >= 0.0f && remainTime < 6)
			{
				UE_LOG ( LogTemp , Warning , TEXT ( "시간: %f" ) , remainTime );
				// 남은 시간을 위젯에 표시
				player->audienceWidget->CountDownText ( FString::FromInt( remainTime) );
			}
		}
	}
}
#pragma region Chat
void UAudienceServerComponent_KMK::ServerRPCChat_Implementation (const FString& nickName, const FString& chat )
{
	// 게임 스테이트를 찾아 채팅을 rpc로 전송
	 AHSW_GameState_Auditorium* gs = GetWorld()->GetGameState<AHSW_GameState_Auditorium>();
    if ( gs)
    {
         gs->MultiRPCChat(nickName, chat);  // GameMode로 메시지 전달
    }
}

void UAudienceServerComponent_KMK::MultiRPCChat_Implementation (const FString& nickName, const FString& chat )
{
	// 다른 관객이라면 찾아서 위잿 생성
	auto* p = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	if (p->audienceWidget)
	{
		p->audienceWidget->CreateChatWidget(nickName, chat );
	}

}
#pragma endregion
// 나를 제외한 다른 관객에게 나의 메쉬를 변경하라고 보내기
void UAudienceServerComponent_KMK::ServerRPC_ChangeMyMesh_Implementation ( int32 num)
{
    UE_LOG(LogTemp, Warning, TEXT("Server is setting playerMeshNum to: %d"), num);
    playerMeshNum = num;
    UE_LOG(LogTemp, Warning, TEXT("Server updated playerMeshNum: %d"), playerMeshNum);

    // 모든 클라이언트에게 RPC 호출
    MultiRPC_ChangeMyMesh(playerMeshNum);

}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num )
{
	auto* TargetMesh = Cast<AHSW_ThirdPersonCharacter> (GetOwner());
    // 로컬 플레이어도 포함하여 모든 클라이언트에서 메쉬 동기화
	if(!TargetMesh) return;
    if (num < 0)
    {
        if(!TargetMesh->HasAuthority())SetVirtualVisible(TargetMesh, false);
    }
    else if (num > 1)
    {
        SetVirtualVisible(TargetMesh, true);
    }
	else
	{
		// 이곳만 관객 관련 내용임
		SetVirtualVisible ( TargetMesh , true );
		TargetMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[num] );

		// 머리 위 스태틱 메쉬 보이게 할지 말지
		if (num == 1) 
		{
			TargetMesh->VIPObject->SetVisibility ( true );
			//UE_LOG ( LogTemp , Warning , TEXT ( "Multi RPC playerMeshNum: 1, " ) );
		}
		else
		{
			TargetMesh->VIPObject->SetVisibility ( false );
			//UE_LOG ( LogTemp , Warning , TEXT ( "Multi RPC playerMeshNum: 0, false" ) );
		}
		UMaterialInstanceDynamic* meshMat = TargetMesh->ChangeMyMeshMat ( num );
		TargetMesh->GetMesh()->SetMaterial(0, meshMat);
	}
}

void UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh()
{
	AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter>(GetOwner());
    UE_LOG(LogTemp, Warning, TEXT("OnRep_ChangePlayerMesh: %s의 playerMeshNum: %d"), *GetOwner()->GetName(), playerMeshNum);
	if (playerMeshNum < 0)
    {
        SetVirtualVisible ( playerCharacter , false );
    }
    else if (playerMeshNum > 1)
    {
        SetVirtualVisible ( playerCharacter , true );
    }
    else
    {
		SetVirtualVisible ( playerMesh , true );
        playerCharacter->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
        UMaterialInstanceDynamic* meshMat = playerCharacter->ChangeMyMeshMat ( playerMeshNum );
		playerCharacter->GetMesh()->SetMaterial(0, meshMat);
    }
}

void UAudienceServerComponent_KMK::SetVirtualVisible ( class AHSW_ThirdPersonCharacter* mesh , bool bVisible )
{
	if (mesh && !mesh->HasAuthority())
	{
		mesh->GetMesh ( )->SetRenderInMainPass ( bVisible );
		mesh->GetMesh ( )->SetRenderInDepthPass ( bVisible );
		UE_LOG ( LogTemp , Warning , TEXT ( "SetVirtualVisible: %s visibility: %s" ) , *mesh->GetName ( ) , bVisible ? TEXT ( "Visible" ) : TEXT ( "Hidden" ) );
	}
}
#pragma region StartConcert
void UAudienceServerComponent_KMK::StartCountDown ( )
{
	virtualCharacter->SetVirtualVisible ( true );
	UE_LOG(LogTemp, Warning, TEXT("Client Virtual Find" ));
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
	//if (gi->effectArray.Num ( ) > 0 && gi->GetConcertInfo().appearedVFX >= 0) GetWorld ( )->SpawnActor<AActor> ( gi->effectArray[gi->GetConcertInfo ( ).appearedVFX] , FTransform(FVector(0, 0, 2000 )));
	//if (gi->effectArray.Num ( ) > 0) GetWorld ( )->SpawnActor<AActor> ( gi->effectArray[3] , FTransform(FVector(0, 0, 2000 ) ));
	
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( startCountDownHandle , FTimerDelegate::CreateLambda ( [this]( )
	//	{
	//		playerMesh->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
	//		bTime = false;
	//		virtualCharacter->SetVirtualVisible(true);
	//		UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	//		if (appearFact.Num ( ) > 0) GetWorld ( )->SpawnActor<AActor> ( gi->effectArray[gi->GetConcertInfo().appearedVFX] , gi->spawnTrans );
	//	} ) , 0 , false );
}

// 콘서트가 시작됨을 알리는 코드
void UAudienceServerComponent_KMK::ServerRPC_StartConcert_Implementation ( )
{
	MultiRPC_StartConcert( durationTime );
}

void UAudienceServerComponent_KMK::MultiRPC_StartConcert_Implementation ( float CountdownTime )
{
	AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	// 카운트다운 UI 패널 표시
	if (playerCharacter && playerCharacter->audienceWidget)
	{
		playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
	}
}
void UAudienceServerComponent_KMK::MultiRPC_UpdateCount_Implementation ( const FString& TimeText )
{
	AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetOwner() );
	if (playerCharacter && playerCharacter->audienceWidget)
	{
		// 남은 시간을 위젯에 표시
		playerCharacter->audienceWidget->CountDownText ( TimeText );
	}
}
#pragma endregion
#pragma region Time
FString UAudienceServerComponent_KMK::GetTimeDifference ( const FString& SetTime )
{
	// 현재 시스템 시간을 가져오기
	auto now = std::chrono::system_clock::now ( );
	std::time_t currentTime = std::chrono::system_clock::to_time_t ( now );

	// tm 구조체로 변환 (로컬 시간)
	std::tm localTime;
	localtime_s ( &localTime , &currentTime );

	// 설정한 시간 (예: "20:19:00")을 tm 구조체로 변환
	int32 SetHour , SetMinute , SetSecond;
	if (sscanf_s ( TCHAR_TO_ANSI ( *SetTime ) , "%d:%d:%d" , &SetHour , &SetMinute , &SetSecond ) != 3)
	{
		return FString ( "Invalid time format" );
	}
	FString CurrentTimeStr = FString::Printf ( TEXT ( "%02d:%02d:%02d" ) ,
	localTime.tm_hour , localTime.tm_min , localTime.tm_sec );
	//UE_LOG ( LogTemp , Warning , TEXT ( "Current Time: %s" ) , *CurrentTimeStr );

	// 현재 시간의 시분초를 설정된 시간으로 덮어씀
	std::tm setTime = localTime;  // 현재 날짜 정보를 사용
	setTime.tm_hour = SetHour;
	setTime.tm_min = SetMinute;
	setTime.tm_sec = SetSecond;

	// 설정된 시간을 std::chrono::time_point로 변환
	std::time_t setTimeT = std::mktime ( &setTime );
	auto setTimeChrono = std::chrono::system_clock::from_time_t ( setTimeT );

	// 현재 시간과 설정된 시간의 차이를 계산
	auto duration = now - setTimeChrono;
	auto secondsDifference = std::chrono::duration_cast<std::chrono::seconds>( duration ).count ( );

	// 차이를 시:분:초로 변환
	int32 hours = FMath::Abs ( static_cast<int32>( secondsDifference / 3600 ) );
	int32 minutes = FMath::Abs ( static_cast<int32>( ( secondsDifference % 3600 ) / 60 ) );
	int32 seconds = FMath::Abs ( static_cast<int32>( secondsDifference % 60 ) );
	AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	if (secondsDifference == 0)  // 시간이 같을 때
	{
        if (!bVis)
        {
            bVis = true;
			ServerRPC_ChangeMyMesh ( 2 );
        }
	}
	else if (minutes == 0 && seconds < 6 && !bCount)
	{
		ServerRPC_StartConcert ( );
		bCount = true;
	}
	else if (minutes == 0 && seconds > 58)
	{
		
		if (playerCharacter && playerCharacter->HasAuthority ( ))
		{
			playerCharacter->audienceWidget->ChangeVirtualWidget ( );
		}
	}
	// FString으로 변환
    FString TimeDifference = FString::Printf ( TEXT ( "%02d:%02d" ) , minutes , seconds );
	if(playerCharacter && playerCharacter->HasAuthority()) playerCharacter->audienceWidget->ChangeTextClock(TimeDifference );
	return TimeDifference;
}



#pragma endregion
void UAudienceServerComponent_KMK::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UAudienceServerComponent_KMK, playerMeshNum);
    DOREPLIFETIME(UAudienceServerComponent_KMK, userName);
}

