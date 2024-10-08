// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_SelectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"

// Sets default values
AJJH_SelectManager::AJJH_SelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJJH_SelectManager::BeginPlay()
{
	Super::BeginPlay();
	
	Sun = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));

}

// Called every frame
void AJJH_SelectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJJH_SelectManager::UpdateSunNightPosition ( bool isNight )
{
	if (isNight)
	{
		Sun->SetActorRotation ( NightSunRotation );
	}
	else
	{
		Sun->SetActorRotation ( AfternoonSunRotation );
	}
}

void AJJH_SelectManager::ChangeMap ( int32 index )
{
	//케이스 나누어서 하기
	UGameplayStatics::OpenLevel(this, TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap"));
}

void AJJH_SelectManager::ChangeEffect ( int32 index )
{
    UParticleSystem* EffectToSpawn = nullptr;
    UParticleSystem* EffectToSpawn1 = nullptr;

    switch (index)
    {
    case 1:
        EffectToSpawn = Effect1;
        break;
    case 2:
        EffectToSpawn = Effect2;
        break;
    default:
        UE_LOG ( LogTemp , Warning , TEXT ( "Invalid effect index: %d" ) , index );
        return;
    }

    if (EffectToSpawn)
    {
        UGameplayStatics::SpawnEmitterAtLocation (
            this ,
            EffectToSpawn ,
            GetActorLocation ( ) ,
            FRotator::ZeroRotator ,
            FVector ( 1.0f , 1.0f , 1.0f ) ,
            true ,
            EPSCPoolMethod::AutoRelease ,
            true
        );
    }
}


