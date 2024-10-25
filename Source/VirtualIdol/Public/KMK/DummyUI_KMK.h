// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DummyUI_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UDummyUI_KMK : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY ( meta = ( BindWidget ) )
	class UImage* DummyImage;

	UFUNCTION()
	void SetImage(UTexture2D* texture );
	UFUNCTION()
	void SetImageVisible(int32 num );
};
