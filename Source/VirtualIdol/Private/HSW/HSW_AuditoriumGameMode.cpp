﻿// Fill out your copyright notice in the Description page of Project Settings.


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

void AHSW_AuditoriumGameMode::BroadcastChatMessage (const FString& nickName, const FString& Chat )
{
    AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium>();
    if (gs)
    {
        gs->MultiRPCChat(nickName, Chat);
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
APawn* FindPawnByTag(UWorld* World, FName Tag)
{
    // 월드의 모든 Pawn을 순회하면서 특정 태그를 가진 Pawn 찾기
    for (TActorIterator<APawn> It(World); It; ++It)
    {
        APawn* FoundPawn = *It;
        if (FoundPawn && FoundPawn->ActorHasTag(Tag))
        {
            return FoundPawn; // 태그가 일치하는 Pawn을 반환
        }
    }
    
    return nullptr; // 태그가 일치하는 Pawn이 없으면 nullptr 반환
}
void AHSW_AuditoriumGameMode::OnPostLogin(APlayerController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("OnPostLogin called for player: %s"), *NewPlayer->GetName());
    // 호스트 클라이언트라면 태그로 Pawn을 검색하고 소유
    if (GameState->PlayerArray.Num() == 1)
    {
        APawn* HostPawn = FindPawnByTag(GetWorld(), FName("Virtual"));
        if (HostPawn)
        {
            NewPlayer->Possess(HostPawn);
            UE_LOG(LogTemp, Log, TEXT("Host client possessed Pawn with 'HostPawn' tag: %s"), *HostPawn->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Pawn with 'HostPawn' tag found."));
        }
    }
    else
    {
        // 나머지 클라이언트용 Pawn 생성 및 소유
        APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (NewPawn)
        {
            NewPlayer->Possess(NewPawn);
        }
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
