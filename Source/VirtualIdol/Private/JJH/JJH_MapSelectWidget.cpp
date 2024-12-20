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
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UJJH_MapSelectWidget::NativeConstruct ( )
{
	Super::NativeConstruct ( );

	SM = Cast<AJJH_SelectManager>(UGameplayStatics::GetActorOfClass ( GetWorld ( ) , AJJH_SelectManager::StaticClass ( ) ) );
	HttpActor = Cast<AHttpActor_KMK> ( UGameplayStatics::GetActorOfClass ( GetWorld ( ) , AHttpActor_KMK::StaticClass() ) );
	
	Borders = { Border_0, Border_1, Border_2, Border_3, Border_4, Border_5,
			   Border_6, Border_7, Border_8, Border_9, Border_10, Border_11,
			   Border_12, Border_13, Border_14, Border_15, Border_20, Border_21, Border_22, Border_23, Border_16};

	CategoryButtons = { WeatherButton, ThemeButton, EffectButton, FloorButton };
		// 디버깅: 배열 확인
	for (int32 i = 0; i < Borders.Num ( ); i++)
	{
		if (Borders[i])
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "Initialized Border_%d: %s" ) , i , *Borders[i]->GetName ( ) );
		}
		else
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "Border_%d is nullptr" ) , i );
		}
	}
	WeatherButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnWeatherButtonClicked );
	ThemeButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnThemeButtonClicked );
	EffectButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButtonClicked );
	FloorButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnFloorButtonClicked );

	//뒤로가기
	BackButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnBackButtonClicked );
	ReturnButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReturnButtonClicked );
	ReturnToMenuButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReturnToMenuButtonClicked );

	//낮밤 바꾸기
	NightButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnNightButtonClicked );
	AfternoonButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnAfternoonButtonClicked );
	BatManSkyButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnBatManSkyButtonClicked );

	//맵 바꾸기
	CyberpunkButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnCyberpunkButtonClicked );
	NaturalButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnNaturalButtonClicked );
	SpaceButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnSpaceButtonClicked );
	DystopiaButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnDystopiaButtonClicked );
	FairytaleButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnFairytaleButtonClicked );
	OceanThemeButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnOceanThemeButtonClicked );
	SkyThemeButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnSkyThemeButtonClicked );

	//이펙트 바꾸기
	EffectButton1->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton1Clicked );
	EffectButton2->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton2Clicked );
	EffectButton3->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton3Clicked );
	EffectButton4->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton4Clicked );
	EffectButton5->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnEffectButton5Clicked );

	//지면 바꾸기
	FogButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnFogButtonClicked );
	GroundButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnGroundButtonClicked );
	OceanButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnOceanButtonClicked );
	DystopiaGroundButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnDystopiaGroundButtonClicked );
	BookButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnBookButtonClicked );
	PresentButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnPresentButtonClicked );

	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );

	//캡쳐
	CaptureButton->OnClicked.AddDynamic( this , &UJJH_MapSelectWidget::OnCaptureButtonClicked );
	SetThumbnailButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnSetThumbnailButtonClicked );
	ReCaptureButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReCaptureButtonClicked );

	//3팝업 숨기기
	SetUpFinishBorder->SetVisibility ( ESlateVisibility::Hidden );
	MakeStageCompleteButton->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnMakeStageCompleteButtonClicked );
	SetUpFinishBorder_1->SetVisibility ( ESlateVisibility::Hidden );
	
	//4팝업
	ReturnToMenuButton_1->OnClicked.AddDynamic ( this , &UJJH_MapSelectWidget::OnReturnToMenuButtonClicked );

	
}


void UJJH_MapSelectWidget::ChangeBorder ( int32 BorderNum )
{
	// 유효한 인덱스인지 확인
	if (BorderNum < 0 || BorderNum >= Borders.Num ( ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Invalid BorderNum: %d" ) , BorderNum );
		return;
	}

	// 모든 Borders 순회
	for (int32 i = 0; i < Borders.Num ( ); i++)
	{
		if (Borders[i])
		{
			FSlateBrush Brush;

			// 선택된 인덱스는 SelectedBox 텍스처 적용
			if (i == BorderNum)
			{
				if (SelectedBox)
				{
					Brush.SetResourceObject ( SelectedBox );
					Borders[i]->SetBrush ( Brush );
				}
			}
			// 나머지 인덱스는 SelectBox 텍스처 적용
			else
			{
				if (SelectBox)
				{
					Brush.SetResourceObject ( SelectBox );
					Borders[i]->SetBrush ( Brush );
				}
			}
		}
	}

	// 디버깅: 변경된 상태 출력
	UE_LOG ( LogTemp , Warning , TEXT ( "Changed texture for Border_%d to SelectedBox" ) , BorderNum );
}
void UJJH_MapSelectWidget::ChangeCategoryButton ( int32 ButtonNum )
{
	if (ButtonNum < 0 || ButtonNum >= CategoryButtons.Num ( ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Invalid BorderNum: %d" ) , ButtonNum );
		return;
	}

	for (int32 i = 0; i < CategoryButtons.Num ( ); i++)
	{
		if (CategoryButtons[i])
		{
			if (i == ButtonNum)
			{
				NextClickButton = CategoryButtons[i];
				if (AlreadyClickedButton && AlreadyClickedButton != NextClickButton)
				{
					// 새로 클릭된 버튼 처리
					FWidgetTransform NextcurrentTransform = NextClickButton->GetRenderTransform ( );
					FVector2D nextTranslation = NextcurrentTransform.Translation;
					NextcurrentTransform.Scale = FVector2D ( 1.1f , 1.0f );
					NextClickButton->SetRenderTransform ( NextcurrentTransform );

					// 이전에 클릭된 버튼 처리
					FWidgetTransform AlreadyTransform = AlreadyClickedButton->GetRenderTransform ( );
					FVector2D alreadyTranslation = AlreadyTransform.Translation;
					AlreadyTransform.Translation = FVector2D ( alreadyTranslation.X + 10.0f , alreadyTranslation.Y );
					AlreadyTransform.Scale = FVector2D ( 1.0f , 1.0f );
					AlreadyClickedButton->SetRenderTransform ( AlreadyTransform );
				}
				else if(AlreadyClickedButton == nullptr)  // 첫 클릭 처리
				{
					// 첫 번째로 클릭된 버튼 처리
					FWidgetTransform FirstClickTransform = NextClickButton->GetRenderTransform ( );
					FVector2D firstTranslation = FirstClickTransform.Translation;
					//FirstClickTransform.Translation = FVector2D ( firstTranslation.X + 10.0f , firstTranslation.Y );
					FirstClickTransform.Scale = FVector2D ( 1.1f , 1.0f );
					NextClickButton->SetRenderTransform ( FirstClickTransform );
				}
				AlreadyClickedButton = NextClickButton;
			}
		}
	}
}


void UJJH_MapSelectWidget::OnWeatherButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Visible );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );

	ChangeCategoryButton(0);
	ChooseBackground->SetContentColorAndOpacity( FLinearColor::Red );
// 
// 	WeatherButton->SetBackgroundColor( FLinearColor::Green);
// 	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
// 	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
// 	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnThemeButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility(ESlateVisibility::Visible );
	VFXHorizontal->SetVisibility(ESlateVisibility::Hidden);
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );

	ChangeCategoryButton ( 1 );
// 	ChooseBackground->SetBrushColor(FLinearColor::Blue);

// 	ThemeButton->SetBackgroundColor ( FLinearColor::Blue );
// 	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
// 	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
// 	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnEffectButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Visible );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Hidden );

	ChangeCategoryButton ( 2 );
/*	ChooseBackground->SetBrushColor ( FLinearColor::Red);*/

// 	EffectButton->SetBackgroundColor ( FLinearColor::Red );
// 	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
// 	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
// 	FloorButton->SetBackgroundColor ( FLinearColor::Gray );
}

void UJJH_MapSelectWidget::OnFloorButtonClicked ( )
{
	DayHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	ThemeHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	VFXHorizontal->SetVisibility ( ESlateVisibility::Hidden );
	FloorHorizontal->SetVisibility ( ESlateVisibility::Visible );

	ChangeCategoryButton ( 3 );
// 	ChooseBackground->SetBrushColor ( FLinearColor::Yellow );
// 
// 	FloorButton->SetBackgroundColor ( FLinearColor::Yellow );
// 	EffectButton->SetBackgroundColor ( FLinearColor::Gray );
// 	ThemeButton->SetBackgroundColor ( FLinearColor::Gray );
// 	WeatherButton->SetBackgroundColor ( FLinearColor::Gray );
}

// 낮밤 바꾸기
void UJJH_MapSelectWidget::OnNightButtonClicked ( )
{
	if(SM) 
	{
		SM->UpdateSunNightPosition(0);
	}
	ChangeBorder(0);
}

void UJJH_MapSelectWidget::OnAfternoonButtonClicked ( )
{
	if (SM)
	{
		SM->UpdateSunNightPosition (1);
	}
	ChangeBorder ( 1 );
}

void UJJH_MapSelectWidget::OnBatManSkyButtonClicked ( )
{
	if (SM)
	{
		SM->UpdateSunNightPosition (2);
	}
	ChangeBorder ( 2 );
}


//테마

void UJJH_MapSelectWidget::OnCyberpunkButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap (0);
	}
	ChangeBorder ( 10 );
	//안쓰고있긴해
}

void UJJH_MapSelectWidget::OnOceanThemeButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap ( 5 );
	}
	ChangeBorder ( 18 );
}

void UJJH_MapSelectWidget::OnSkyThemeButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap ( 6 );
	}
	ChangeBorder ( 19 );
}

void UJJH_MapSelectWidget::OnNaturalButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap (1);
	}
	ChangeBorder ( 11 );
}

void UJJH_MapSelectWidget::OnSpaceButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap(2);
	}
	ChangeBorder ( 12 );
}

void UJJH_MapSelectWidget::OnDystopiaButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap ( 3 );
	}
	ChangeBorder ( 13 );
}
void UJJH_MapSelectWidget::OnFairytaleButtonClicked ( )
{
	if (SM)
	{
		SM->ChangeMap ( 4 );
	}
	ChangeBorder ( 14 );
}


//지면 바꾸기
void UJJH_MapSelectWidget::OnFogButtonClicked ( )
{
	if (SM) SM->ChangeFloor(0);
	ChangeBorder ( 3 );
}

void UJJH_MapSelectWidget::OnGroundButtonClicked ( )
{
	if (SM) SM->ChangeFloor (1);
	ChangeBorder ( 4 );
}
void UJJH_MapSelectWidget::OnOceanButtonClicked ( )
{
	if (SM) SM->ChangeFloor ( 2 );
	ChangeBorder ( 5 );
}

void UJJH_MapSelectWidget::OnDystopiaGroundButtonClicked ( )
{
	if (SM) SM->ChangeFloor ( 3 );
	ChangeBorder ( 6 );
}

void UJJH_MapSelectWidget::OnBookButtonClicked ( )
{
	if (SM) SM->ChangeFloor ( 4 );
	ChangeBorder ( 15 );
}

void UJJH_MapSelectWidget::OnPresentButtonClicked ( )
{
	if (SM) SM->ChangeFloor ( 5 );
	ChangeBorder ( 20 );
}

//이펙트 바꾸기
void UJJH_MapSelectWidget::OnEffectButton1Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect (0);
	}
	ChangeBorder ( 7 );
}

void UJJH_MapSelectWidget::OnEffectButton2Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect (1);
	}
	ChangeBorder ( 8 );
}
void UJJH_MapSelectWidget::OnEffectButton3Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect ( 2 );
	}
	ChangeBorder ( 9 );
}
void UJJH_MapSelectWidget::OnEffectButton4Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect ( 3 );
	}
	ChangeBorder ( 16 );
}
void UJJH_MapSelectWidget::OnEffectButton5Clicked ( )
{
	if (SM)
	{
		SM->ChangeEffect ( 4 );
	}
	ChangeBorder ( 17 );
}



void UJJH_MapSelectWidget::OnCaptureButtonClicked ( )
{
	AJJH_SetupPlayerController* SPC = Cast<AJJH_SetupPlayerController> ( GetWorld ( )->GetFirstPlayerController ( ) );

	if (SM)
	{
		SM->TakeScreenshot();
		CaptureButton->SetVisibility(ESlateVisibility::Hidden);
		ReturnButton->SetVisibility ( ESlateVisibility::Hidden );
		ReturnToMenuButton->SetVisibility ( ESlateVisibility::Hidden );
		SetUpFinishBorder->SetVisibility ( ESlateVisibility::Hidden );
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
	SetUpFinishBorder->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::PlaySound2D ( GetWorld ( ) , PopUpSFV );
	StageNameText->SetText( StageName->GetText ( ) );
	SetupWidgetSwitcher->SetActiveWidgetIndex(2);

}
void UJJH_MapSelectWidget::SetImageWithCapturedImage ( )
{

	SetupWidgetSwitcher->SetActiveWidgetIndex ( 1 );
	PlayAnimation ( ShutterAnimation );
	ReturnToMenuButton->SetVisibility ( ESlateVisibility::Visible );
}

void UJJH_MapSelectWidget::OnBackButtonClicked ( )
{
	PlayAnimation(BackButtonAnimation);
}

void UJJH_MapSelectWidget::OnReturnButtonClicked ( )
{
	PlayAnimation ( BackButtonAnimation, 1.0f, 1, EUMGSequencePlayMode::PingPong);
}
void UJJH_MapSelectWidget::OnReturnToMenuButtonClicked ( )
{
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Project/Personal/KMK/Maps/KMK_Maps"));
}

void UJJH_MapSelectWidget::OnMakeStageCompleteButtonClicked ( )
{
	StageNameText_1->SetText( StageNameText->GetText());
	//맵 이름 저장
	if (SM)
	{
		SM->SetName(StageNameText->GetText().ToString());
	}
	else
	{
		return;
	}
	//이미지 저장
	//작업 마무리


	if (HttpActor)
	{
		HttpActor->ReqMultipartCapturedURL (SM->Stage, SM->FullFileName);
	}
}

void UJJH_MapSelectWidget::MakeSetUpFinishBorder_1Visible ( )
{
	SetUpFinishBorder->SetVisibility ( ESlateVisibility::Hidden );
	SetUpFinishBorder_1->SetVisibility ( ESlateVisibility::Visible );
	UGameplayStatics::PlaySound2D ( GetWorld ( ) , PopUpSFV );
}