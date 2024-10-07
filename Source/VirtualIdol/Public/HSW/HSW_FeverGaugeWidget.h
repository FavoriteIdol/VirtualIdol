// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HSW_FeverGaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UHSW_FeverGaugeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float CurrentGauge = 0.0f;

};
