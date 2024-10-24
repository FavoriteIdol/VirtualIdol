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
	// httpactor
	UPROPERTY( )
	class AHttpActor_KMK* HttpActor;

	class AJJH_SelectManager* SM;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SetupWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SettingPanel;

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

	UFUNCTION ()
	void OnAfternoonButtonClicked ( );	
	UFUNCTION ( )
	void OnNightButtonClicked ( );

	//지면
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FloorButton;	
	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* GroundButton;

	UFUNCTION ( )
	void OnFogButtonClicked ( );
	UFUNCTION ( )
	void OnGroundButtonClicked ( );

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

	//캡쳐를 위한 뒤로가기

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* BackButton;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* ReturnButton;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* ReturnToMenuButton;
	UFUNCTION ( )
	void OnBackButtonClicked ( );	
	UFUNCTION ( )
	void OnReturnButtonClicked ( );
	UFUNCTION ( )
	void OnReturnToMenuButtonClicked ( );

	UPROPERTY ( Transient , meta = ( BindWidgetAnim ) )
	class UWidgetAnimation* BackButtonAnimation;

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

	//3팝업(무대제작완료)
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* SetUpFinishBorder;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* MakeStageCompleteButton;

	UFUNCTION ( )
	void OnMakeStageCompleteButtonClicked ( );

	UPROPERTY ( meta = ( BindWidget ) )
	class UEditableText* StageName;

	UPROPERTY ( meta = ( BindWidget ) )
	class UTextBlock* StageNameText;

	//4팝업
	UPROPERTY ( meta = ( BindWidget ) )
	class UImage* CapturedImage_1;
	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* ReturnToMenuButton_1;

	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* SetUpFinishBorder_1;

	UPROPERTY ( meta = ( BindWidget ) )
	class UTextBlock* StageNameText_1;

	virtual void NativeConstruct() override;
};
