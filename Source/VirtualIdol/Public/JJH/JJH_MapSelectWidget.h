// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JJH_MapSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UJJH_MapSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	class AJJH_SelectManager* SM;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SetupWidgetSwitcher;

	//3대장 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* WeatherButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ThemeButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* EffectButton;
		
	UPROPERTY(meta = (BindWidget))
	class UButton* CaptureButton;

	UFUNCTION( )
	void OnWeatherButtonClicked();
	UFUNCTION( )
	void OnThemeButtonClicked ( );
	UFUNCTION( )
	void OnEffectButtonClicked ( );
	UFUNCTION( )
	void OnFloorButtonClicked ( );

	//낮밤 바꾸기
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* DayHorizontal;	

	UPROPERTY(meta = (BindWidget))
	class UButton* AfternoonButton;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* NightButton;		
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FloorButton;

	UFUNCTION ()
	void OnAfternoonButtonClicked ( );	
	UFUNCTION ( )
	void OnFogButtonClicked ( );
	UFUNCTION ( )
	void OnNightButtonClicked ( );

	//캡쳐
	UPROPERTY(meta = (BindWidget))
	class UImage* CapturedImage;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SetThumbnailButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ReCaptureButton;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ShutterAnimation;

	UFUNCTION ( )
	void OnCaptureButtonClicked ( );
	UFUNCTION ( )
	void OnReCaptureButtonClicked ( );
	UFUNCTION ( )
	void OnSetThumbnailButtonClicked ( );
	UFUNCTION ( )
	void SetImageWithCapturedImage ();


	//테마 바꾸기
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* ThemeHorizontal;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* CyberpunkButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* NaturalButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SpaceButton;

	UFUNCTION( )
	void OnNaturalButtonClicked( );
	UFUNCTION( )
	void OnSpaceButtonClicked ( );
	UFUNCTION( )
	void OnCyberpunkButtonClicked ( );

	//특수효과 바꾸기
	UPROPERTY ( meta = ( BindWidget ) )
	class UHorizontalBox* VFXHorizontal;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton1;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton2;

	//지면 바꾸기

	UPROPERTY ( meta = ( BindWidget ) )
	class UHorizontalBox* FloorHorizontal;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FogButton;
	//UPROPERTY ( meta = ( BindWidget ) )
	//class UButton* EffectButton2;

	UFUNCTION( )
	void OnEffectButton1Clicked ();
	UFUNCTION( )
	void OnEffectButton2Clicked ();

	//선택버튼 뒤에 배경
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* ChooseBackground;

	virtual void NativeConstruct() override;
};
