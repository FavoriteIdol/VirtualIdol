﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

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
