// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SendChat_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API USendChat_KMK : public UUserWidget
{
	GENERATED_BODY()
public:
	// 버튼
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Chat;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_ChatName;

	UFUNCTION( )
	void SetChatText(const FString& nickname, const FString& chatText );
};
