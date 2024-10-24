﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SingWidget_KMK.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void USingWidget_KMK::NativeConstruct ( )
{
    Super::NativeConstruct();
    if (Butt_Stop && Butt_Start && Butt_Pause)
    {
        Butt_Stop->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressStopButt);
        Butt_Start->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressStartButt);
        Butt_Pause->OnClicked.AddDynamic ( this , &USingWidget_KMK::PressPauseButt);
    }
}

void USingWidget_KMK::PressStopButt ( )
{
    if(sound == nullptr) return;
    bStop = true;
    sound->Stop(); 
}

void USingWidget_KMK::PressStartButt ( )
{
    if(sound == nullptr) return;
    if(bStop) 
    {
        sound = preSound;
        bStop = false;
    }
    sound->Play();
    sound->SetPaused(false);
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

void USingWidget_KMK::SetMusic ( class UAudioComponent* music )
{
    sound = music;
    preSound = music;
}
