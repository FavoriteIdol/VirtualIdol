// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SendChat_KMK.h"
#include "Components/TextBlock.h"

void USendChat_KMK::SetChatText (const FString& nickname, const FString& chatText )
{
    FString s = nickname + FString(TEXT(" : " ) );
    Text_ChatName->SetText(FText::FromString(*s));
    Text_Chat->SetText(FText::FromString(chatText));
}
