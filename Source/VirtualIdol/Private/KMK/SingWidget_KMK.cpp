// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SingWidget_KMK.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "HSW/HSW_GameState_Auditorium.h"

void USingWidget_KMK::NativeConstruct ( )
{
    Super::NativeConstruct();
    if (Butt_Stop && Butt_Start && Butt_Pause)
    {
        Butt_Stop->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressStopButt);
        Butt_Start->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressStartButt);
        Butt_Pause->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressPauseButt);
    }

    gm = Cast<AHSW_AuditoriumGameMode>(GetWorld()->GetAuthGameMode());
    pc = Cast<AHSW_ThirdPersonCharacter>(GetOwningPlayer());
}

void USingWidget_KMK::PressStopButt ( )
{
    if(sound == nullptr) return;
    bStop = true;
    sound->Stop(); 
}

void USingWidget_KMK::PressStartButt ( )
{   

    //if(sound == nullptr) return;
    if(bStop) 
    {
        sound = preSound;
        bStop = false;
    }
    //sound->SetPaused(false);
    AHSW_GameState_Auditorium* gs = GetWorld ( )->GetGameState<AHSW_GameState_Auditorium> ( );

    if (gs)
    {
        gs->ServerRPCPlaySound( WavArray[0] );
    }
   

}

void USingWidget_KMK::PressPauseButt ( )
{
    if(sound == nullptr) return;
    sound->SetPaused(true);
}

void USingWidget_KMK::SetTextMusic ( const FString& text )
{
    if (Text_Music)
    {
        Text_Music->SetText(FText::FromString(text));
    }
}

void USingWidget_KMK::SetMusic ( class UAudioComponent* music, class USoundBase* wavFile )
{
    SoundFile = wavFile;
    sound = music;
    preSound = music;
}