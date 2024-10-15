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

}

// Called every frame
void AJJH_SelectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJJH_SelectManager::UpdateSunNightPosition ( bool isNight )
{
    FindActorAndDestroy ( TEXT ( "Skybox" ) );
	if (isNight)
	{
		GetWorld()->SpawnActor<AActor>(SkyFactory[0], GetActorTransform ( ) );
	}
	else
	{
        GetWorld()->SpawnActor<AActor>( SkyFactory[1] , GetActorTransform ());
	}
}

void AJJH_SelectManager::ChangeMap ( int32 index )
{
	//케이스 나누어서 하기
    FindActorAndDestroy(TEXT("Theme"));
    GetWorld ( )->SpawnActor<AActor>(ThemeFactory[index] , GetActorTransform ( ) );

}

void AJJH_SelectManager::FindActorAndDestroy(FName tag)
{
    TArray<AActor*> ActorToDestroy;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), tag , ActorToDestroy);
    if (!ActorToDestroy.IsEmpty ())
    {
		for (AActor* Actor : ActorToDestroy)
		{
			if (Actor) Actor->Destroy ( );
		}
    }
}

void AJJH_SelectManager::ChangeEffect ( int32 index )
{
    FindActorAndDestroy ( TEXT ( "VFX" ) );
    GetWorld ( )->SpawnActor<AActor> (VFXFactory[index] , GetActorTransform ( ) );
}

void AJJH_SelectManager::ChangeFloor (int32 index)
{
    FindActorAndDestroy ( TEXT ( "Floor" ) );
    GetWorld ( )->SpawnActor<AActor> (FloorFactory[index] , GetActorTransform ( ) );
}


