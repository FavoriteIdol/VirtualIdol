// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "HSW/HSW_GameState_Auditorium.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundBase.h"

AHSW_AuditoriumGameMode::AHSW_AuditoriumGameMode ( )
{
	PrimaryActorTick.bCanEverTick = true;

	// 디폴트 폰 클래스 설정
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass ( TEXT ( "/Game/Project/Personal/HSW/Blueprints/HSW_BP_ThirdPerson" ) );
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void AHSW_AuditoriumGameMode::Tick ( float DeltaTime )
{

}

void AHSW_AuditoriumGameMode::BeginPlay ( )
{
	Super::BeginPlay();

	Player = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	FName tag = TEXT ( "FeverGaugeLocation" );
	for (TActorIterator<AActor> It ( GetWorld ( ) , AStaticMeshActor::StaticClass ( ) ); It; ++It)
	{
		AActor* Actor = *It;
		if (IsValid ( Actor ) && Actor->ActorHasTag ( tag ))
		{
			FeverEffectLocation = Actor->GetTransform ( );
		}
	}
}

void AHSW_AuditoriumGameMode::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AHSW_AuditoriumGameMode , bFever );
	DOREPLIFETIME ( AHSW_AuditoriumGameMode , FeverEffect_Particle );
	DOREPLIFETIME ( AHSW_AuditoriumGameMode , FeverEffectLocation );
}


void AHSW_AuditoriumGameMode::Multicast_FeverEffect_Implementation ( )
{
	UGameplayStatics::SpawnEmitterAtLocation ( GetWorld ( ) , FeverEffect_Particle , FeverEffectLocation );
	UE_LOG(LogTemp, Warning, TEXT("Multicast_FeverEffect_Implementation"));
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , FeverEffect_Niagara , FeverEffectLocation.GetLocation() );
}

void AHSW_AuditoriumGameMode::BroadcastChatMessage ( const FString& Chat )
{
    AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium>();
    if (gs)
    {
        gs->MultiRPCChat(Chat);
    }
}

void AHSW_AuditoriumGameMode::BroadcastPlayMusic ( USoundBase* wavFile )
{
	AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium> ( );
	if (gs)
	{
		gs->MultiRPCPlaySound ( wavFile );
	}
}

void AHSW_AuditoriumGameMode::BroadcastSetInterviewee ( bool bInterview , APlayerState* interviewee , FTransform preLoc )
{
	AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium> ( );
	if (gs)
	{
		gs->MultiRPC_SetInterviewee( bInterview, interviewee, preLoc );
	}
}

void AHSW_AuditoriumGameMode::BroadcastCountDown ( )
{
	AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium> ( );
	if (gs)
	{
		gs->MultiRPC_ShowCountDown();
	}
}
void AHSW_AuditoriumGameMode::ServerPlayMusic_Implementation ( class UAudioComponent* selectedMusic )
{
	for (FConstPlayerControllerIterator It = GetWorld ( )->GetPlayerControllerIterator ( ); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AHSW_ThirdPersonCharacter* PlayerCharacter = Cast<AHSW_ThirdPersonCharacter>(PlayerController->GetPawn( ));
			if (PlayerCharacter)
			{
				//SoundFile = sound;
				PlayerCharacter->ClientPlayMusic ( selectedMusic );
			}
		}
	}
}
