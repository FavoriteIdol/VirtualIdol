﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/AudienceServerComponent_KMK.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "KMK/Audience_KMK.h"
#include "Net/UnrealNetwork.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "chrono"

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

	player = Cast<ATP_ThirdPersonCharacter> (GetWorld()->GetFirstPlayerController()->GetPawn());
	playerMesh = Cast<ATP_ThirdPersonCharacter> (GetOwner());
	if (playerMesh->HasAuthority ( ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Server playerMeshNum: %d" ) , gi->playerMeshNum );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Client playerMeshNum: %d" ) , gi->playerMeshNum );
	}
	if (gi)
	{
		// 플레이어가 로컬 플레이어 일때
		if (!playerMesh->IsLocallyControlled())
        {
			
			// 로컬이 아닌 경우에 플레이어의 playerMeshNum에 따라 
			if (playerMeshNum < 0)
			{
				SetVirtualVisible ( playerMesh , false );
			}
			else if (playerMeshNum > 1)
			{
				SetVirtualVisible ( playerMesh , true );
			}
			else
			{
				playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
			}
        }
		else
		{
			// 클라이언트에서 서버로 RPC 호출
			ServerRPC_ChangeMyMesh ( gi->playerMeshNum );
		}
	}
}


// Called every frame
void UAudienceServerComponent_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bVis)
	{
		s = GetTimeDifference ( setConcertTime );
		// 서버에서 시간을 계산하고 클라이언트에게 전파
		ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetOwner ( ) );
		if (playerCharacter && playerCharacter->audienceWidget && playerCharacter->IsLocallyControlled())
		{
			// 남은 시간을 위젯에 표시
			playerCharacter->audienceWidget->CountDownText ( s );
		}

	}
	else
	{
		ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetOwner ( ) );
		// 카운트다운 UI 패널 표시
		if (playerCharacter && playerCharacter->audienceWidget)
		{
			playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
		}
	}
}

void UAudienceServerComponent_KMK::ServerRPCChat_Implementation ( const FString& chat )
{
	MultiRPCChat(chat);
}

void UAudienceServerComponent_KMK::MultiRPCChat_Implementation ( const FString& chat )
{
	auto* p = Cast<ATP_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	if (p->audienceWidget)
	{
		p->audienceWidget->CreateChatWidget(chat );
	}

}


void UAudienceServerComponent_KMK::ServerRPC_ChangeMyMesh_Implementation ( int32 num)
{
	playerMeshNum = num;
	MultiRPC_ChangeMyMesh ( playerMeshNum, playerMesh ); // 클라이언트에게 RPC 호출
}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num, class ATP_ThirdPersonCharacter* TargetMesh )
{
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
	if (gi)
	{
		if (num < 0)
		{
			SetVirtualVisible ( playerMesh , false );
		}
		else if (num > 1)
		{
			SetVirtualVisible ( playerMesh , true );
		}
		else
		{
			playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[num] );
		}
	}
}

void UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh ( )
{
	// Get the PlayerController owning this component
	APlayerController* playerController = Cast<APlayerController> ( GetOwner ( ) );
	UE_LOG ( LogTemp , Error , TEXT ( "Player %s MeshNum: %d" ) , *GetOwner ( )->GetName ( ) , playerMeshNum );

	// Make sure the PlayerController is valid
	if (playerMesh)
	{
		if (playerMeshNum < 0)
		{
			SetVirtualVisible ( playerMesh , false );
		}
		else if (playerMeshNum > 1)
		{
			SetVirtualVisible ( playerMesh , true );
		}
		else
		{
			playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
		}
	}
}

void UAudienceServerComponent_KMK::SetVirtualVisible ( class ATP_ThirdPersonCharacter* mesh , bool bVisible )
{
	mesh->GetMesh ( )->SetRenderInMainPass ( bVisible );
	mesh->GetMesh ( )->SetRenderInDepthPass ( bVisible );
}

void UAudienceServerComponent_KMK::StartCountDown ( )
{
	bTime = true;

}
void UAudienceServerComponent_KMK::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 DOREPLIFETIME(UAudienceServerComponent_KMK, playerMeshNum);
}

void UAudienceServerComponent_KMK::ServerRPC_StartConcert_Implementation ( )
{
	MultiRPC_StartConcert( durationTime );
}

void UAudienceServerComponent_KMK::MultiRPC_StartConcert_Implementation ( float CountdownTime )
{
	ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	// 카운트다운 UI 패널 표시
	if (playerCharacter && playerCharacter->audienceWidget)
	{
		playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
	}
}


void UAudienceServerComponent_KMK::MultiRPC_UpdateCount_Implementation ( const FString& TimeText )
{
	ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetOwner() );
	if (playerCharacter && playerCharacter->audienceWidget)
	{
		// 남은 시간을 위젯에 표시
		playerCharacter->audienceWidget->CountDownText ( TimeText );
	}
}

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
	UE_LOG ( LogTemp , Warning , TEXT ( "Current Time: %s" ) , *CurrentTimeStr );

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
	ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
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