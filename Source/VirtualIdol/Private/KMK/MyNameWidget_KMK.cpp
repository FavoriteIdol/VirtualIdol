// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/MyNameWidget_KMK.h"
#include "Components/TextBlock.h"

void UMyNameWidget_KMK::SetMyName ( const FString& name )
{
    TEXT_Name->SetText(FText::FromString(name) );
}
