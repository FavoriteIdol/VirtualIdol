// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_SoundManager.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
AHSW_SoundManager::AHSW_SoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHSW_SoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSW_SoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHSW_SoundManager::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AHSW_SoundManager , CurrentSongWav );
	DOREPLIFETIME ( AHSW_SoundManager , CurrentSongComp );
}

void AHSW_SoundManager::ServerRPC_SetMusic_Implementation ( USoundBase* wavToSet )
{
	MulticastRPC_SetMusic(wavToSet );
}

void AHSW_SoundManager::MulticastRPC_SetMusic_Implementation ( USoundBase* wavToSet )
{
	CurrentSongComp->SetSound ( wavToSet );
}

void AHSW_SoundManager::ServerRPC_PlayMusic_Implementation ( UAudioComponent* MusicToPlay )
{
	MulticastRPC_PlayMusic( MusicToPlay );
}

void AHSW_SoundManager::MulticastRPC_PlayMusic_Implementation ( UAudioComponent* MusicToPlay )
{
	MusicToPlay->Play( );
}

void AHSW_SoundManager::PlaySound ( USoundBase* MusicToPlay )
{

}

