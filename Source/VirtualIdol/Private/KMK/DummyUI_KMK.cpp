// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/DummyUI_KMK.h"
#include "Components/Image.h"

void UDummyUI_KMK::NativeConstruct ( )
{
    Super::NativeConstruct();

    DummyImage->SetVisibility(ESlateVisibility::Hidden);
}

void UDummyUI_KMK::SetImage ( UTexture2D* texture )
{
    DummyImage->SetBrushFromTexture(texture);
}

void UDummyUI_KMK::SetImageVisible ( int32 num )
{
    if(num == 0) DummyImage->SetVisibility(ESlateVisibility::Hidden);
    else DummyImage->SetVisibility(ESlateVisibility::Visible);
}
