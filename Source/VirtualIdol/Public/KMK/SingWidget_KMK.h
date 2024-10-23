// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SingWidget_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API USingWidget_KMK : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Music;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Stop;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Start;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Pause;

	bool bStop = false;

	UFUNCTION( )
	void PressStopButt( );
	UFUNCTION( )
	void PressStartButt( );
	UFUNCTION( )
	void PressPauseButt( );

	UFUNCTION( )
	void SetTextMusic(const FString& text );
    UPROPERTY ( )
    class UAudioComponent* sound;
    class UAudioComponent* preSound;
	UFUNCTION( )
	void SetMusic(class UAudioComponent* music );
};
