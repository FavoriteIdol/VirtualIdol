// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyNameWidget_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UMyNameWidget_KMK : public UUserWidget
{
	GENERATED_BODY()
public :
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* TEXT_Name;

	UFUNCTION( )
	void SetMyName(const FString& name );
};
