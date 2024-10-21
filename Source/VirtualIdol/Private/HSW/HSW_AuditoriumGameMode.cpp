// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_ThirdPersonCharacter.h"

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
	if (bFevered)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fever!!"));
	}
}

void AHSW_AuditoriumGameMode::BeginPlay ( )
{
	Super::BeginPlay();

	Player = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

}

void AHSW_AuditoriumGameMode::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{

}
