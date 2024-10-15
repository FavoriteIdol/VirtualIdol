// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/StartWidget_KMK.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Engine/TimerHandle.h"
#include "KMK/RoomWidget_KMK.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Engine/Texture2D.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"
#include "Components/ScrollBox.h"
#include "KMK/HttpActor_KMK.h"

void UStartWidget_KMK::NativeConstruct ( )
{	
    Super::NativeConstruct();
	// GI 찾기
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance() );
	if (gi->bLogin)
	{
		StartSwitcher->SetActiveWidgetIndex ( 1 );
	}
	else
	{
		gi->bLogin = true;
	}
#pragma region LoginPanel
	if (Butt_Login)
	{
		Butt_Login->OnClicked.AddDynamic(this, &UStartWidget_KMK::OnMyLogin);
	}
#pragma endregion

#pragma region FourButtPanel
	if (Butt_CreateStage)
	{
		Butt_CreateStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::CreateStagePanel );
	}
	if (Butt_ReadyConcert)
	{
		Butt_ReadyConcert->OnClicked.AddDynamic ( this , &UStartWidget_KMK::SettingStagePanel );
	}
	if (Butt_StartConcert)
	{
		Butt_StartConcert->OnClicked.AddDynamic ( this , &UStartWidget_KMK::StartConcertPanel );
	}
	if (Butt_ComeInStage)
	{
		Butt_ComeInStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::ComeInStagePanel );
	}
#pragma endregion
#pragma region Select Stage
	if (Butt_UserStage && Butt_MyStage)
	{
		Butt_UserStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressUserStageButt );
		Butt_MyStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressMyStageButt );
	}
#pragma endregion

#pragma region  Setting StagePanel
	if (Image_Particle && Image_Fever)
	{
		Image_Particle->SetBrushFromMaterial ( EffectParticles[0] );
		Image_Fever->SetBrushFromMaterial ( FeversParticles[0] );
	}
#pragma region Create Ticket
	if (Butt_CreateTicket)
	{
		Butt_CreateTicket->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressCreateTicket );
	}
#pragma endregion
#pragma region Set Particle
	if (Butt_Right && Butt_FRight)
	{
		Butt_Right->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressRightButt );
		Butt_FRight->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressFRightButt );
	}
	if (Butt_Left && Butt_FLeft)
	{
		Butt_Left->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressLeftButt );
		Butt_FLeft->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressFLeftButt );
	}
	if (Butt_Select)
	{
		Butt_Select->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressSelectButt );
    }	
	if (SetDayPanel && SetTicketPanel && SetEffectPanel && StageChargePanel)
	{
		SetTitleText(TEXT("공연 설정" ) );
		SetPanelVisible ( SetDayPanel , SetTicketPanel , SetEffectPanel , StageChargePanel );
	}
#pragma endregion
#pragma region Pay Moneny
	if (Butt_PayMoney)
	{
		Butt_PayMoney->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressPayMoney );
	}
	if (Butt_Next)
	{
		Butt_Next->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressNextButt );
	}
	if (StagePayPanel && Text_Pay && Butt_PayMoney && PayPopUpPanel)
	{
		StagePayPanel->SetVisibility(ESlateVisibility::Hidden);
		Text_Pay->SetVisibility(ESlateVisibility::Hidden);
		Butt_PayMoney->SetVisibility(ESlateVisibility::Hidden);
		PayPopUpPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Butt_Okay)
	{
		Butt_Okay->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressOkayButt );
	}
#pragma endregion

#pragma endregion

#pragma region Entry
	if (Butt_Yes && Butt_VipEntry && VIPPopUpPanel)
	{
		Butt_VipEntry->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressVipEntry );
		Butt_Yes->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressYesButt );
		VIPPopUpPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Butt_NormalEntry && Butt_No)
	{
		Butt_NormalEntry->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressNormalEntry );
		Butt_No->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressNoButt );
	}	
	if (Butt_Back1 && Butt_Back2 && Butt_Back)
	{
		Butt_Back->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back1->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back2->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
	}
#pragma endregion
#pragma region Session
	if (gi)
	{
		gi->OnSearchSignatureCompleteDelegate.AddDynamic(this, &UStartWidget_KMK::CreateRoomWidget );
		gi->SetStartWidget(this);
	}

#pragma endregion


}

void UStartWidget_KMK::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	if (StageScalePanel->Visibility == ESlateVisibility::SelfHitTestInvisible)
	{
		if (!EditText_ScaleNum->GetText ( ).IsEmpty ( ))
		{
			FString s = EditText_ScaleNum->GetText().ToString();
			int a = FCString::Atoi(*s) * concertPrice;
			Text_Price->SetText(FText::AsNumber(a));
		}
	}
}

#pragma region BackFunction
void UStartWidget_KMK::GoBack ( )
{
    StartSwitcher->SetActiveWidgetIndex (1);
	ClearSB( );
}
#pragma endregion


#pragma region LoginPanel

void UStartWidget_KMK::OnMyLogin ( )
{
    // 아이디와 패스워드가 없으면 실행 안되게 하기
	if (!EditText_ID->GetText ( ).IsEmpty ( ) && !EditText_PW->GetText ( ).IsEmpty ( ))
	{
		// 서버에 정보값 보내기

		// 선택지로 변경
		StartSwitcher->SetActiveWidgetIndex ( 1 );
	}
}

#pragma endregion

#pragma region FourButtPanel
// 무대 꾸미기 레벨로 이동
void UStartWidget_KMK::CreateStagePanel ( )
{
	UGameplayStatics::OpenLevel(GetWorld( ) , TEXT("JJH_SetupMap" ));
}
// 공연 일정 잡는 판넬로 변경
void UStartWidget_KMK::SettingStagePanel ( )
{
	StartSwitcher->SetActiveWidgetIndex ( 3 );
	for (int i = 0; i < 6; i++)
	{
		CreateStageWidget ( FString::FromInt ( i ) );
	}
}
// 공연 시작 : 세션 생성
void UStartWidget_KMK::StartConcertPanel ( )
{
	if (gi)
	{
		gi->playerMeshNum = -1;
		gi->CreateMySession(TEXT("조준혁"), 30);
	}
}

void UStartWidget_KMK::ComeInStagePanel ( )
{	
	// 세션 찾기
	FindRoom();
}

#pragma endregion

#pragma region Select Stage

void UStartWidget_KMK::PressUserStageButt ( )
{
	ClearSB( );
	for (int i = 0; i < 5; i++)
	{
		CreateStageWidget ( FString::FromInt ( i ) );
	}
}

void UStartWidget_KMK::PressMyStageButt ( )
{
	ClearSB( );
	for (int i = 0; i < 2; i++)
	{
		CreateStageWidget ( FString::FromInt ( i ) );
	}

}

void UStartWidget_KMK::CreateStageWidget ( const FString& createName )
{
	auto* wid = Cast<URoomWidget_KMK> ( CreateWidget ( GetWorld ( ) , roomWidgetFact ) );
	if (wid)
	{
		wid->SetStageText ( createName );
		SB_FindStage->AddChild(wid);
	}
}

void UStartWidget_KMK::ClearSB ( )
{
	if (SB_FindStage->GetChildrenCount ( ) > 0)
	{
		SB_FindStage->ClearChildren();
	}
}

#pragma endregion


#pragma region Setting StagePanel

void UStartWidget_KMK::PressSelectButt ( )
{
	if (selectNum == 0)
	{
		bool bNext = BEditTextEmpty ( );
		if(!bNext) return;
		SetTitleText ( TEXT ( "티켓" ) );
		SetPanelVisible( SetTicketPanel , SetDayPanel, SetEffectPanel, StageChargePanel );
		selectNum++;
	}
	else if (selectNum == 1)
	{
		if(EditText_Price->GetText().IsEmpty()) return;

		SetTitleText ( TEXT ( "이펙트 설정" ) );
		SetPanelVisible ( SetEffectPanel , SetTicketPanel , SetDayPanel , StageChargePanel );
		selectNum++;
	}
	else
	{
		SetPanelVisible ( StageChargePanel , SetDayPanel , SetTicketPanel , SetEffectPanel );
		ClearAllText( );
		StageChargePanel->SetVisibility(ESlateVisibility::Visible);
		selectNum = 0;
	}

}

void UStartWidget_KMK::SetTitleText ( const FString& title )
{
	if (Text_Title)
	{
		Text_Title->SetText ( FText::FromString ( title ) );
	}
}

bool UStartWidget_KMK::BEditTextEmpty ( )
{
	// 모든 텍스트가 채워진 경우
	if (!EditText_StageName->GetText ( ).IsEmpty ( ) 
	&& !EditText_Year->GetText ( ).IsEmpty ( ) && !EditText_Mon->GetText ( ).IsEmpty ( ) && !EditText_Day->GetText ( ).IsEmpty ( )
	&& !EditText_SHour->GetText ( ).IsEmpty ( ) && !EditText_SMin->GetText ( ).IsEmpty ( )
	&& !EditText_H->GetText ( ).IsEmpty ( ) && !EditText_M->GetText ( ).IsEmpty ( ))
	{
		Text_FinalName->SetText(EditText_StageName->GetText());
		Text_FinalStageName->SetText(EditText_StageName->GetText());
		
		concertInfo.concertName = EditText_StageName->GetText().ToString();
		bool bEditDigit 
            = EditTextDigit ( EditText_Year->GetText ( ).ToString ( ) ) && EditTextDigit ( EditText_Mon->GetText ( ).ToString ( ) )
            && EditTextDigit ( EditText_Day->GetText ( ).ToString ( ) ) && EditTextDigit ( EditText_SHour->GetText ( ).ToString ( ) )
            && EditTextDigit ( EditText_SMin->GetText ( ).ToString ( ) ) && EditTextDigit ( EditText_H->GetText ( ).ToString ( ) )
            && EditTextDigit ( EditText_M->GetText ( ).ToString ( ) );
		if(!bEditDigit) return  false;

		Text_FinalYear->SetText(EditText_Year->GetText());
		Text_FinalMon->SetText(EditText_Mon->GetText());
		Text_FinalDay->SetText(EditText_Day->GetText());

		FString year = TEXT("20") + EditText_Year->GetText ( ).ToString ( );
		FString mon = ChangeString(EditText_Day->GetText().ToString());
		FString day = ChangeString(EditText_Day->GetText().ToString());

		concertInfo.concertDay = year + TEXT("-") + mon + TEXT("-") + day;

		Text_StartHour->SetText(EditText_SHour->GetText());
		Text_StartMin->SetText(EditText_SMin->GetText());
		FString sHour =  ChangeString(EditText_SHour->GetText().ToString());
		FString sMin = ChangeString(EditText_SMin->GetText().ToString());
		concertInfo.concertTime = sHour + TEXT(":" ) + sMin + TEXT(":" ) + TEXT("00");

		FString sH = EditText_SHour->GetText().ToString();
		FString sM = EditText_SMin->GetText().ToString();
		FString bH = EditText_H->GetText().ToString();
		FString bM = EditText_M->GetText().ToString();

		int h = FCString::Atoi(*sH) + FCString::Atoi(*bH);
		int m = FCString::Atoi(*sM) + FCString::Atoi(*bM);

		sH = ChangeString(FString::FromInt(h));
		sM = ChangeString(FString::FromInt(m));
		concertInfo.endTime = sH + TEXT ( ":" ) + sM + TEXT ( ":" ) + TEXT ( "00" );

		return true;
	}
	else return false;
}

FString UStartWidget_KMK::ChangeString ( const FString& editText )
{
    FString s = editText;
    if (s.Len ( ) == 1)
    {
        s = TEXT ( "0" ) + s;
    }
	return s;
}

bool UStartWidget_KMK::EditTextDigit ( const FString& editText )
{
	for (char c : editText)
	{
		if (!isdigit ( c ))
		{
			return false;
		}
	}
	return true;
}

void UStartWidget_KMK::PressCreateTicket ( )
{
	if(!EditMultiText_Ticket->GetText().IsEmpty()) UE_LOG(LogTemp, Warning, TEXT("Create!" ) );
	EditMultiText_Ticket->SetText ( FText::GetEmpty ( ) );
}

void UStartWidget_KMK::ClearAllText ( )
{
	
	EditText_StageName->SetText( FText::GetEmpty ( ) );
	EditText_Year->SetText( FText::GetEmpty ( ) );
	EditText_Mon->SetText( FText::GetEmpty ( ) );
	EditText_Day->SetText( FText::GetEmpty ( ) );
	EditText_SHour->SetText( FText::GetEmpty ( ) );
	EditText_SMin->SetText( FText::GetEmpty ( ) );
	EditText_H->SetText( FText::GetEmpty ( ) );
	EditText_M->SetText( FText::GetEmpty ( ) );

	EditText_Price->SetText( FText::GetEmpty ( ) );
	EditMultiText_Ticket->SetText( FText::GetEmpty ( ) );

	Image_Particle->SetBrushFromMaterial ( EffectParticles[0] );
	Image_Fever->SetBrushFromMaterial ( FeversParticles[0] );

	concertInfo.appearEffectNum = particleNum;
	concertInfo.feverEffectNum = feverNum;
	UE_LOG(LogTemp, Warning, TEXT("%d, %d" ), concertInfo.appearEffectNum, concertInfo.feverEffectNum );

	particleNum = 0;
	feverNum = 0;
}

void UStartWidget_KMK::PressRightButt ( )
{
	if (EffectParticles.IsEmpty ( )) return;
	particleNum++;
	if (particleNum > EffectParticles.Num ( ) - 1) particleNum = 0;
	Image_Particle->SetBrushFromMaterial ( EffectParticles[particleNum] );
}

void UStartWidget_KMK::PressLeftButt ( )
{
	if (EffectParticles.IsEmpty ( )) return;
	particleNum--;
	if (particleNum < 0) particleNum = EffectParticles.Num ( ) - 1;
	Image_Particle->SetBrushFromMaterial ( EffectParticles[particleNum] );
}

void UStartWidget_KMK::PressFRightButt ( )
{
	if (FeversParticles.IsEmpty ( )) return;
	feverNum++;
	if (feverNum > FeversParticles.Num ( ) - 1) feverNum = 0;
	Image_Fever->SetBrushFromMaterial ( FeversParticles[feverNum] );
}

void UStartWidget_KMK::PressFLeftButt ( )
{
	if (FeversParticles.IsEmpty ( )) return;
	feverNum--;
	if (feverNum < 0) feverNum = FeversParticles.Num ( ) - 1;
	Image_Fever->SetBrushFromMaterial ( FeversParticles[feverNum] );
}

void UStartWidget_KMK::SetPanelVisible ( class UCanvasPanel* visiblePanel , class UCanvasPanel* hiddenPanel0 , class UCanvasPanel* hiddenPanel1 , class UCanvasPanel* hiddenPanel2 )
{
	hiddenPanel0->SetVisibility ( ESlateVisibility::Hidden );
	hiddenPanel1->SetVisibility ( ESlateVisibility::Hidden );
	hiddenPanel2->SetVisibility ( ESlateVisibility::Hidden );
	visiblePanel->SetVisibility ( ESlateVisibility::Visible );
}

void UStartWidget_KMK::AddIndex ( int32 num , TArray<class UMaterial*>  meshArray , UImage* image )
{
	if (meshArray.IsEmpty ( )) return;
	num++;
	if (num > meshArray.Num ( ) - 1) num = 0;
	image->SetBrushFromMaterial ( meshArray[num] );
}

void UStartWidget_KMK::MinusIndex ( int32 num , TArray<class UMaterial*>  meshArray , UImage* image )
{
	if (meshArray.IsEmpty ( )) return;
	num--;
	if (num < 0) num = meshArray.Num ( ) - 1;
	image->SetBrushFromMaterial ( meshArray[num] );
}

#pragma region Final Setting

void UStartWidget_KMK::PressNextButt ( )
{
	if(EditText_ScaleNum->GetText().IsEmpty()) return;
	StagePayPanel->SetVisibility ( ESlateVisibility::Visible );
	Text_Pay->SetVisibility ( ESlateVisibility::Visible );
	Butt_PayMoney->SetVisibility ( ESlateVisibility::Visible );
	StageScalePanel->SetVisibility ( ESlateVisibility::Hidden );
	Butt_Next->SetVisibility ( ESlateVisibility::Hidden );
	Text_FinalCount->SetText ( EditText_ScaleNum->GetText() );
	Text_FinalPay->SetText ( Text_Price->GetText() );
	Text_Price->SetText(FText::GetEmpty ( ) );
	EditText_ScaleNum->SetText ( FText::GetEmpty ( ) );
}
void UStartWidget_KMK::PressPayMoney ( )
{
	PayPopUpPanel->SetVisibility(ESlateVisibility::Visible);
	// Text_FinalStageName->SetText()
}

void UStartWidget_KMK::PressOkayButt ( )
{
	ClearSB ( );
	StartSwitcher->SetActiveWidgetIndex ( 1 );
	ResetWidget( );
}

void UStartWidget_KMK::ResetWidget ( )
{
	SetTitleText ( TEXT ( "공연 설정" ) );
	SetPanelVisible ( SetDayPanel , SetTicketPanel , SetEffectPanel , StageChargePanel );

	StageChargePanel->SetVisibility(ESlateVisibility::Hidden);
	StageScalePanel->SetVisibility ( ESlateVisibility::Visible );
	StagePayPanel->SetVisibility ( ESlateVisibility::Hidden );
	Text_Pay->SetVisibility ( ESlateVisibility::Hidden );
	Butt_PayMoney->SetVisibility ( ESlateVisibility::Hidden );
	PayPopUpPanel->SetVisibility ( ESlateVisibility::Hidden );
}

#pragma endregion

#pragma endregion

#pragma region Entry Panel
// vip 접속
void UStartWidget_KMK::PressYesButt ( )
{
	if (gi && roomNum >= 0)
	{
		gi->JoinRoom(roomNum, 1);

		//ChangeAudienceMesh(0);
	}
}
// 일반 접속
void UStartWidget_KMK::PressNormalEntry ( )
{
	if (gi && roomNum >= 0)
	{
		gi->JoinRoom(roomNum);
		//ChangeAudienceMesh(1);
	}
}
void UStartWidget_KMK::PressNoButt ( )
{
	VIPPopUpPanel->SetVisibility(ESlateVisibility::Hidden);
}

// 세션 확인할 수 있는 UI 띄우기
void UStartWidget_KMK::PressVipEntry ( )
{
	VIPPopUpPanel->SetVisibility(ESlateVisibility::Visible);
}



// 세션 찾기
void UStartWidget_KMK::FindRoom( )
{
	// VIP 입장 판넬 띄우기
	StartSwitcher->SetActiveWidgetIndex ( 3 );

	ClearChild();
	if (gi)
	{
		gi->FindOtherSession();
	}
}
// 세션을 만들면 wbp를 생성해서 그리드에 배치
void UStartWidget_KMK::CreateRoomWidget (const struct FRoomInfo& info )
{
	// wbp 생성
	auto* wid = Cast<URoomWidget_KMK>(CreateWidget(GetWorld(), roomWidgetFact));
	if (wid)
	{
		wid->SetImageAndText(info);
		roomCount++;
		SB_FindStage->AddChild(wid);
	}
}
// 그리드 삭제
void UStartWidget_KMK::ClearChild ( )
{
	if (SB_FindStage->GetChildrenCount ( ) > 0)
	{
		roomCount = 0;
		SB_FindStage->ClearChildren();
	}
}


#pragma endregion


