﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	// 보더 골랐을 때
	UPROPERTY(EditAnywhere)
	UTexture2D* SelectedBox;	
	
	// 안골라진 보더
	UPROPERTY(EditAnywhere)
	UTexture2D* SelectBox;

	UPROPERTY(EditAnywhere)
	TArray<class UBorder*> Borders;	
	
	//카테고리 버튼 보더
	UPROPERTY ( EditAnywhere )
	TArray<class UButton*> CategoryButtons;

	UPROPERTY()
	class UButton* AlreadyClickedButton;	
	
	UPROPERTY ()
	class UButton* NextClickButton;


	// 사운드
	UPROPERTY(EditAnywhere, Category = Sound )
	class USoundBase* PopUpSFV;

	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_0;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_1;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_2;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_3;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_4;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_5;	
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_6;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_7;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_8;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_9;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_10;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_11; 
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_12; 
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_13;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_14;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_15;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_20;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_21;
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_22;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_23;	
	
	UPROPERTY ( meta = ( BindWidget ) )
	class UBorder* Border_16;

	UFUNCTION()
	void ChangeBorder(int32 BorderNum);
	UFUNCTION ( )
	void ChangeCategoryButton ( int32 ButtonNum );

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
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* BatManSkyButton;

	UFUNCTION ()
	void OnAfternoonButtonClicked ( );	
	UFUNCTION ( )
	void OnBatManSkyButtonClicked ( );
	UFUNCTION ( )
	void OnNightButtonClicked ( );

	//지면

	UPROPERTY ( meta = ( BindWidget ) )
	class UHorizontalBox* FloorHorizontal;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FloorButton;


	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FogButton;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* GroundButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* OceanButton;	
	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* DystopiaGroundButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* BookButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* PresentButton;


	UFUNCTION ( )
	void OnFogButtonClicked ( );
	UFUNCTION ( )
	void OnGroundButtonClicked ( );
	UFUNCTION ( )
	void OnOceanButtonClicked ( );	
	UFUNCTION ( )
	void OnDystopiaGroundButtonClicked ( );	
	UFUNCTION ( )
	void OnBookButtonClicked ( );	
	UFUNCTION ( )
	void OnPresentButtonClicked ( );


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
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* DystopiaButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* FairytaleButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* OceanThemeButton;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* SkyThemeButton;

	UFUNCTION( )
	void OnNaturalButtonClicked( );
	UFUNCTION( )
	void OnSpaceButtonClicked ( );
	UFUNCTION( )
	void OnDystopiaButtonClicked ( );
	UFUNCTION ( )
	void OnFairytaleButtonClicked ( );
	UFUNCTION ( )
	void OnCyberpunkButtonClicked ( );	
	UFUNCTION ( )
	void OnOceanThemeButtonClicked ( );
	UFUNCTION ( )
	void OnSkyThemeButtonClicked ( );

	//특수효과 바꾸기
	UPROPERTY ( meta = ( BindWidget ) )
	class UHorizontalBox* VFXHorizontal;

	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton1;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton2;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton3;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton4;	
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* EffectButton5;
	
	UFUNCTION( )
	void OnEffectButton1Clicked ();
	UFUNCTION( )
	void OnEffectButton2Clicked ();
	UFUNCTION( )
	void OnEffectButton3Clicked ( );
	UFUNCTION( )
	void OnEffectButton4Clicked ( );
	UFUNCTION( )
	void OnEffectButton5Clicked ( );
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

	void MakeSetUpFinishBorder_1Visible ( );
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
	
	UPROPERTY()
    UButton* CurrentHoveredButton;

};
