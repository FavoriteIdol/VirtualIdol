// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HSW_MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UHSW_MainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct () override;

// 	UPROPERTY(EditDefaultsOnly, Category = FeverGauge)
// 	TSubclassOf<class UUserWidget> FeverGaugeFactory;

	UPROPERTY ( meta = ( BindWidget ) )
	class UHSW_FeverGaugeWidget* FeverGauge;

};
