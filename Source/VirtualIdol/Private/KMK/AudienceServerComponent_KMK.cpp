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
	if (playerMesh && playerMesh->HasAuthority())
	{
		
		if (playerMesh->IsLocallyControlled())
        {
            playerMesh->SetActorLocation ( FVector ( 0, 0, 100 ) );
        }
        else
        {
            playerMesh->SetActorLocation ( FVector ( 3600.0f, 0.f, 300.f ) );
            playerMesh->SetActorRotation(FRotator(0, 180, 0));
        }
	}
	else
	{
		FindVirtualCharacter ( );
	}
	if (gi && playerMesh)
	{
		
		// 플레이어가 로컬 플레이어 일때
		if (playerMesh && playerMesh->IsLocallyControlled ( ))
		{
			// 서버에 요청하여 고유한 `playerMeshNum` 할당
			ServerRPC_ChangeMyMesh ( gi->playerMeshNum );
		}
		//else
		//{
		//	UVirtualGameInstance_KMK* g = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
		//	 //로컬이 아닌 경우에 플레이어의 playerMeshNum에 따라 
		//	if (playerMeshNum < 0)
		//	{
		//		SetVirtualVisible ( playerMesh , false );
		//	}
		//	else if (playerMeshNum > 1)
		//	{
		//		SetVirtualVisible ( playerMesh , true );
		//	}
		//	else
		//	{
		//		SetVirtualVisible ( playerMesh , true );
		//		playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[g->playerMeshNum] );
		//		UMaterialInstanceDynamic* meshMat = playerMesh->ChangeMyMeshMat ( g->playerMeshNum );
		//		playerMesh->GetMesh()->SetMaterial(0, meshMat);
		//	}
		//}
	}


}

void UAudienceServerComponent_KMK::FindVirtualCharacter ( )
{
	TArray<AActor*> actorArray;
	UGameplayStatics::GetAllActorsWithTag ( GetWorld ( ) , TEXT ( "Virtual" ) , actorArray );
	for (AActor* actor : actorArray)
	{
		if (actor->FindComponentByTag < USkeletalMeshComponent > ( TEXT ( "Mesh" ) ))
		{
			virtualCharacter = actor->FindComponentByClass<UVirtual_KMK > ( );
			UE_LOG(LogTemp, Warning, TEXT("Find!!!!!!!!" ) );
			if (virtualCharacter) virtualCharacter->SetVirtualVisible ( false );
		}
	}
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
void UAudienceServerComponent_KMK::ServerRPCChat_Implementation ( const FString& chat )
{
	 AHSW_GameState_Auditorium* gs = GetWorld()->GetGameState<AHSW_GameState_Auditorium>();
    if ( gs)
    {
         gs->MultiRPCChat(chat);  // GameMode로 메시지 전달
    }
}

void UAudienceServerComponent_KMK::MultiRPCChat_Implementation ( const FString& chat )
{
	auto* p = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	if (p->audienceWidget)
	{
		p->audienceWidget->CreateChatWidget(chat );
	}

}
#pragma endregion

void UAudienceServerComponent_KMK::ServerRPC_ChangeMyMesh_Implementation ( int32 num)
{
	if (playerMesh->HasAuthority ( ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Server is setting playerMeshNum to: %d" ) , num );
		playerMeshNum = num;
		UE_LOG ( LogTemp , Warning , TEXT ( "Server updated playerMeshNum: %d" ) , playerMeshNum );

		// 모든 클라이언트에게 RPC 호출
		// MultiRPC_ChangeMyMesh ( playerMeshNum );
	}

}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num )
{
	auto* TargetMesh = Cast<AHSW_ThirdPersonCharacter> (GetOwner());
	UE_LOG ( LogTemp , Warning , TEXT ( "OnRep_ChangePlayerMesh: %s의 playerMeshNum: %d" ) , *GetOwner ( )->GetName ( ) , num );

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
		SetVirtualVisible ( TargetMesh , true );
		TargetMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[num] );
		UMaterialInstanceDynamic* meshMat = TargetMesh->ChangeMyMeshMat ( num );
		TargetMesh->GetMesh()->SetMaterial(0, meshMat);
	}
}

void UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh()
{
	AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter>(GetOwner());

	if (!playerCharacter || playerMeshNum < 0 || playerMeshNum >= audienceMesh.Num ( ) || !audienceMesh[playerMeshNum])
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "OnRep_ChangePlayerMesh 로드 실패" ));
		// 만약 메쉬 설정이 실패하면 타이머로 재시도
		GetWorld ( )->GetTimerManager ( ).SetTimerForNextTick ( this , &UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh );
		return;
	}
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
		onReq = true;
		SetVirtualVisible ( playerMesh , true );
        UMaterialInstanceDynamic* meshMat = playerCharacter->FeverDynamicMats[playerMeshNum];
        playerCharacter->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
		playerCharacter->GetMesh()->SetMaterial(0, playerCharacter->FeverDynamicMats[playerMeshNum] );
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
	bTime = true;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( startCountDownHandle , FTimerDelegate::CreateLambda ( [this]( )
		{
			playerMesh->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
			bTime = false;
			virtualCharacter->SetVirtualVisible(true);
			if (appearFact.Num ( ) > 0) GetWorld ( )->SpawnActor<AActor> ( appearFact[0] , FTransform ( FVector ( 0 ) ) );
		} ) , 6 , false );
}


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
}

