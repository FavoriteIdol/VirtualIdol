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

	UPROPERTY ( meta = ( BindWidget ) )
	class UHorizontalBox* ImojiBox;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Btn_Imoji_1;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Btn_Imoji_2;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Btn_Imoji_3;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Btn_Imoji_4;

	UFUNCTION( )
	void OnMyImoji01( );
	UFUNCTION( )
	void OnMyImoji02( );
	UFUNCTION( )
	void OnMyImoji03( );
	UFUNCTION( )
	void OnMyImoji04( );

	UPROPERTY( )
	class AHSW_ThirdPersonCharacter* Player;
	UPROPERTY( )
	class UHSW_ImojiConponent* ImojiComponent;

};
