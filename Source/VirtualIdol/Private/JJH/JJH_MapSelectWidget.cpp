﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_MapSelectWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "JJH/JJH_SelectManager.h"
#include "JJH_SetupPlayerController.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"

void UJJH_MapSelectWidget::OnEffectButton1Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect(1);
	}
}

void UJJH_MapSelectWidget::OnEffectButton2Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect(2);
	}
}

void UJJH_MapSelectWidget::NativeConstruct ( )
{
	Super::NativeConstruct ( );

	SM = Cast<AJJH_SelectManager>(UGameplayStatics::GetActorOfClass ( GetWorld ( ) , AJJH_SelectManager::StaticClass ( ) ) );

	WeatherButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnWeatherButtonClicked );
	ThemeButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnThemeButtonClicked );
	EffectButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButtonClicked );
	FloorButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnFloorButtonClicked );

	//뒤로가기
	BackButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnBackButtonClicked );
	ReturnButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReturnButtonClicked );

	
	//낮밤 바꾸기
	NightButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnNightButtonClicked );
	AfternoonButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnAfternoonButtonClicked );

	//맵 바꾸기
	CyberpunkButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnCyberpunkButtonClicked );
	NaturalButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnNaturalButtonClicked );
	SpaceButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnSpaceButtonClicked );


	//이펙트 바꾸기
	EffectButton1->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton1Clicked );
	EffectButton2->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton2Clicked );

	//지면 바꾸기
	FogButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnFogButtonClicked );

	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );

	//캡쳐
	CaptureButton->OnClicked.AddDynamic( this , &UJJH_MapSelectWidget::OnCaptureButtonClicked );
	SetThumbnailButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnSetThumbnailButtonClicked );
	ReCaptureButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReCaptureButtonClicked );

}
void UJJH_MapSelectWidget::OnWeatherButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Visible );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ChooseBackground->SetBrushColor(FLinearColor::Green);

	WeatherButton->SetBackgroundColor( FLinearColor::Green);
	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnThemeButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility(ESlateVisibility::Visible );
	VFXHorizontal->SetVisibility(ESlateVisibility::Hidden);
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ChooseBackground->SetBrushColor(FLinearColor::Blue);

	ThemeButton->SetBackgroundColor ( FLinearColor::Blue );
	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnEffectButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Visible );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ChooseBackground->SetBrushColor ( FLinearColor::Red);

	EffectButton->SetBackgroundColor ( FLinearColor::Red );
	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnFloorButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Visible );
	ChooseBackground->SetBrushColor ( FLinearColor::Yellow );

	FloorButton->SetBackgroundColor ( FLinearColor::Yellow );
	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
}

// 낮밤 바꾸기
void UJJH_MapSelectWidget::OnNightButtonClicked ( )
{
	if(SM) 
	{
		SM->UpdateSunNightPosition(true);
	}
}

void UJJH_MapSelectWidget::OnAfternoonButtonClicked ( )
{
	if (SM) SM->UpdateSunNightPosition ( false );
}


void UJJH_MapSelectWidget::OnNaturalButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap ( 1 );
	}
}

void UJJH_MapSelectWidget::OnSpaceButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap(1);
	}
}

void UJJH_MapSelectWidget::OnCyberpunkButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap (1);
	}
}


void UJJH_MapSelectWidget::OnFogButtonClicked ( )
{
	if (SM) SM->ChangeFloor(1);
}

void UJJH_MapSelectWidget::OnCaptureButtonClicked ( )
{
	AJJH_SetupPlayerController* SPC = Cast<AJJH_SetupPlayerController> ( GetWorld ( )->GetFirstPlayerController ( ) );
	if (SPC)
	{
		SPC->TakeScreenshot ( );
		CaptureButton->SetVisibility(ESlateVisibility::Hidden);
		ReturnButton->SetVisibility ( ESlateVisibility::Hidden );
	}

	// 0.3초 후에 SetImageWithCapturedImage 함수 호출
	//바로 찍는속도가 너무 빨라서 버튼이 사라지기전에 찍힘
	FTimerHandle UnusedHandle;
	GetWorld ( )->GetTimerManager ( ).SetTimer (
		UnusedHandle ,
		this ,
		&UJJH_MapSelectWidget::SetImageWithCapturedImage ,
		0.3f ,
		false
	);
}
void UJJH_MapSelectWidget::OnReCaptureButtonClicked ( )
{
	SetupWidgetSwitcher->SetActiveWidgetIndex ( 0 );
	CaptureButton->SetVisibility ( ESlateVisibility::Visible);
	ReturnButton->SetVisibility ( ESlateVisibility::Visible );
}
void UJJH_MapSelectWidget::OnSetThumbnailButtonClicked ( )
{
	
}
void UJJH_MapSelectWidget::SetImageWithCapturedImage ( )
{
	SetupWidgetSwitcher->SetActiveWidgetIndex ( 1 );
	PlayAnimation ( ShutterAnimation );
}

void UJJH_MapSelectWidget::OnBackButtonClicked ( )
{
	PlayAnimation(BackButtonAnimation);
}

void UJJH_MapSelectWidget::OnReturnButtonClicked ( )
{
	PlayAnimation ( BackButtonAnimation, 1.0f, 1, EUMGSequencePlayMode::PingPong);
	UE_LOG(LogTemp, Error, TEXT("!@#$"));
}

