// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_AudioActor.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AHSW_AudioActor::AHSW_AudioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AmbientSoundComponent01 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent01" ) );
	AmbientSoundComponent02 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent02" ) );

//     ConstructorHelpers::FObjectFinder<USoundCue> SoundCueAsset ( TEXT ( "SoundCue'/Game/Audio/MySoundCue.MySoundCue'" ) );
//     if (SoundCueAsset.Succeeded ( ))
//     {
//         SoundCue1 = SoundCueAsset.Object;
//         AmbientSoundComponent01->SetSound ( SoundCue1 );
//     }
}

// Called when the game starts or when spawned
void AHSW_AudioActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSW_AudioActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHSW_AudioActor::PlaySound0 ( float SoundVolume )
{
	if (AmbientSoundComponent01 && SoundCue01 && !AmbientSoundComponent01->IsPlaying ( ))
	{
		AmbientSoundComponent01->SetSound ( SoundCue01 );
		AmbientSoundComponent01->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent01->Play ( ); // 사운드 재생
	}
}

void AHSW_AudioActor::PlaySound1 ( float SoundVolume )
{
	if (AmbientSoundComponent01 && SoundCue01 && !AmbientSoundComponent01->IsPlaying())
	{
		
		AmbientSoundComponent01->SetSound( SoundCue01 );
		AmbientSoundComponent01->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent01->Play ( ); // 사운드 재생
	}
	else if(AmbientSoundComponent01->IsPlaying ( ))
	{
		AmbientSoundComponent01->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound2 ( float SoundVolume )
{
	if (AmbientSoundComponent02 && SoundCue02 && !AmbientSoundComponent02->IsPlaying ( ))
	{

		AmbientSoundComponent02->SetSound ( SoundCue02 );
		AmbientSoundComponent02->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent02->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent02->IsPlaying ( ))
	{
		AmbientSoundComponent02->SetVolumeMultiplier ( SoundVolume );
	}
}