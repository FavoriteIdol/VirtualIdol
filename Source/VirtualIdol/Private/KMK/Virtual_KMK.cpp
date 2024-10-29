// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Virtual_KMK.h"
#include "chrono"
#include "KMK/Audience_KMK.h"
#include "HSW/HSW_GameState_Auditorium.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerState.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "HSW/HSW_ThirdPersonCharacter.h"

// Sets default values for this component's properties
UVirtual_KMK::UVirtual_KMK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVirtual_KMK::BeginPlay()
{
	Super::BeginPlay();
	meshComp = GetOwner()->FindComponentByTag<USkeletalMeshComponent>(FName(TEXT("Mesh")));

	FName tag = TEXT ( "InterviewLocation" );
	for (TActorIterator<AActor> It ( GetWorld ( ) , AStaticMeshActor::StaticClass ( ) ); It; ++It)
	{
		AActor* Actor = *It;
		if (IsValid ( Actor ) && Actor->ActorHasTag ( tag ))
		{
			StageLocation = Actor->GetTransform ( );
		}
	}

	pc = GetWorld ( )->GetFirstPlayerController ( );

	if (pc)
	{
		UE_LOG(LogTemp, Warning, TEXT("PC" ));
		StartVoiceChat( );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "PC none" ) );
	}
}


// Called every frame
void UVirtual_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bTime)
	{
		remainTime = GetWorld ( )->GetTimerManager ( ).GetTimerRemaining ( startCountDownHandle );
		if (virtualWidget)
		{
			if (remainTime >= 0.0f)
			{
				// 남은 시간을 위젯에 표시
				virtualWidget->CountDownText ( FString::FromInt ( remainTime ) );
			}
		}
	}
}
#pragma region Time
FString UVirtual_KMK::GetTimeDifference ( const FString& SetTime )
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
	//AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	//if (secondsDifference == 0)  // 시간이 같을 때
	//{
 //       if (!bVis)
 //       {
 //           bVis = true;
	//		ServerRPC_ChangeMyMesh ( 2 );
 //       }
	//}
	//else if (minutes == 0 && seconds < 6 && !bCount)
	//{
	//	ServerRPC_StartConcert ( );
	//	bCount = true;
	//}
	//else if (minutes == 0 && seconds > 58)
	//{
	//	
	//	if (playerCharacter && playerCharacter->HasAuthority ( ))
	//	{
	//		playerCharacter->audienceWidget->ChangeVirtualWidget ( );
	//	}
	//}
	// FString으로 변환
    FString TimeDifference = FString::Printf ( TEXT ( "%02d:%02d" ) , minutes , seconds );
	if(virtualWidget) virtualWidget->ChangeTextClock(TimeDifference );
	return TimeDifference;
}

void UVirtual_KMK::SetVirtualChat ( const FString& text )
{
	if(virtualWidget)virtualWidget->CreateChatWidget(text);
}

void UVirtual_KMK::SetVirtualVisible ( bool bVisit /*= false */ )
{
	if (meshComp)
	{
		meshComp->SetRenderInMainPass ( bVisit );
		meshComp->SetRenderInDepthPass ( bVisit );
		meshComp->CastShadow = bVisit;
	}
}

void UVirtual_KMK::ShowCoundDownPanel ( )
{
	if (!virtualWidget) return;
	virtualWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
	StartCountDown ( );
}

void UVirtual_KMK::CallGMShowServer ( )
{
	AHSW_GameState_Auditorium* gs = GetWorld ( )->GetGameState<AHSW_GameState_Auditorium> ( );
	if (gs)
	{
		gs->ServerRPC_ShowCountDown();
	}
}

void UVirtual_KMK::StartCountDown ( )
{
	bTime = true;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( startCountDownHandle , FTimerDelegate::CreateLambda ( [this]( )
        {
            SetVirtualVisible ( true );
			virtualWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
			if(appearFact.Num() > 0) GetWorld ( )->SpawnActor<AActor> ( appearFact[0] , FTransform ( FVector ( 0 ) ) );
        } ) , 6 , false );

}

void UVirtual_KMK::PlayMusic ( USoundBase* wavFile )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Virtual : Play Music" ) );
	UAudioComponent* AudioComponent = NewObject<UAudioComponent> ( this );
	AudioComponent->SetSound ( wavFile );
	AudioComponent->RegisterComponent ( );
	AudioComponent->Play ( );  // 음원 재생
}

void UVirtual_KMK::SetInterviewee ( bool bInterview , APlayerState* interviewee, FTransform preLoc )
{
	if (bInterview)
	{
		//intervieweePlayer->CameraBoom->TargetArmLength = 0;
		interviewee->GetPawn ( )->SetActorTransform ( StageLocation );
		interviewee->GetPawn ( )->SetActorScale3D ( FVector ( 3.0 ) );

		if (APawn* pawn = interviewee->GetPawn ( ))
		{
			if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( pawn ))
			{
				Character->StartVoiceChat ( );
			}
		}
	}
	else
	{
		interviewee->GetPawn ( )->SetActorTransform ( preLoc );
		interviewee->GetPawn ( )->SetActorScale3D ( FVector ( 2.0 ) );

		if (APawn* pawn = interviewee->GetPawn ( ))
		{
			if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( pawn ))
			{
				Character->CancleVoiceChat ( );
			}
		}
	}
}

void UVirtual_KMK::StartVoiceChat ( )
{
	pc->StartTalking ( );
}

void UVirtual_KMK::CancleVoiceChat ( )
{
	pc->StopTalking( );
}

#pragma endregion

