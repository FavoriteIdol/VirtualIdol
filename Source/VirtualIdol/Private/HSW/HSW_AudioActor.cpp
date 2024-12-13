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

	AmbientSoundComponent00 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent00" ) );
    AmbientSoundComponent01 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent01" ) );
	AmbientSoundComponent02 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent02" ) );
	AmbientSoundComponent03 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent03" ) );
	AmbientSoundComponent04 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent04" ) );
	AmbientSoundComponent05 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent05" ) );
	AmbientSoundComponent_start = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "AudioComponent_start" ) );
	FeverSoundComponent = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "FeverSoundComponent" ) );
	EffectSoundComponent01 = CreateDefaultSubobject<UAudioComponent> ( TEXT ( "EffectSoundComponent01" ) );
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
	if (AmbientSoundComponent00 && SoundCue00 && !AmbientSoundComponent00->IsPlaying ( ))
	{
		AmbientSoundComponent00->SetSound ( SoundCue00 );
		AmbientSoundComponent00->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent00->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent00->IsPlaying ( ))
	{
		AmbientSoundComponent00->SetVolumeMultiplier ( SoundVolume );
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

void AHSW_AudioActor::PlaySound3 ( float SoundVolume )
{
	if (AmbientSoundComponent03 && SoundCue03 && !AmbientSoundComponent03->IsPlaying ( ))
	{
		AmbientSoundComponent03->SetSound ( SoundCue03 );
		AmbientSoundComponent03->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent03->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent03->IsPlaying ( ))
	{
		AmbientSoundComponent03->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound4 ( float SoundVolume )
{
	if (AmbientSoundComponent04 && SoundCue04 && !AmbientSoundComponent04->IsPlaying ( ))
	{

		AmbientSoundComponent04->SetSound ( SoundCue04 );
		AmbientSoundComponent04->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent04->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent04->IsPlaying ( ))
	{
		AmbientSoundComponent04->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound5 ( float SoundVolume )
{
	if (AmbientSoundComponent05 && SoundCue05 && !AmbientSoundComponent05->IsPlaying ( ))
	{

		AmbientSoundComponent05->SetSound ( SoundCue05 );
		AmbientSoundComponent05->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent05->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent05->IsPlaying ( ))
	{
		AmbientSoundComponent05->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound_start ( float SoundVolume )
{
	if (AmbientSoundComponent_start && SoundCue_start && !AmbientSoundComponent_start->IsPlaying ( ))
	{

		AmbientSoundComponent_start->SetSound ( SoundCue_start );
		AmbientSoundComponent_start->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		AmbientSoundComponent_start->Play ( ); // 사운드 재생
	}
	else if (AmbientSoundComponent_start->IsPlaying ( ))
	{
		AmbientSoundComponent_start->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound_Fever ( float SoundVolume )
{
	if (FeverSoundComponent && SoundCue_Fever && !FeverSoundComponent->IsPlaying ( ))
	{

		FeverSoundComponent->SetSound ( SoundCue_Fever );
		FeverSoundComponent->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
		FeverSoundComponent->Play ( ); // 사운드 재생
	}
	else if (FeverSoundComponent->IsPlaying ( ))
	{
		FeverSoundComponent->SetVolumeMultiplier ( SoundVolume );
	}
}

void AHSW_AudioActor::PlaySound_Effect01 ( float SoundVolume )
{

	EffectSoundComponent01->SetSound ( SoundCue_Effect );
	EffectSoundComponent01->SetVolumeMultiplier ( SoundVolume ); // 음량 조절
	EffectSoundComponent01->Play ( ); // 사운드 재생

}
