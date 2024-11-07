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
#include "JJH/JJH_SelectManager.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"

void UStartWidget_KMK::NativeConstruct ( )
{	
    Super::NativeConstruct();
	// GI 찾기
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance() );
	httpActor = Cast<AHttpActor_KMK>(UGameplayStatics::GetActorOfClass(GetWorld() , httpFact));
	selectManager = Cast<AJJH_SelectManager>(UGameplayStatics::GetActorOfClass(GetWorld() , selectFact));
	loadMatInst = UMaterialInstanceDynamic::Create ( loadMatFact, this );
	if (Image_Load)
	{
		Image_Load->SetBrushFromMaterial(loadMatInst);
		loadMatInst->SetScalarParameterValue ( TEXT ( "Gauge" ) , 0 );
	}
	if (gi->bLogin)
	{
		selectManager->DeleteStage ( );
		StartSwitcher->SetActiveWidgetIndex ( 1 );
		ChangeMyProfile ( );
	}
	else
	{
		gi->bLogin = true;
	}
#pragma region LoginPanel
	if (Butt_Login && Butt_FailLogin && FailLoginPanel)
	{
		Butt_Login->OnClicked.AddDynamic(this, &UStartWidget_KMK::OnMyLogin);
		Butt_FailLogin->OnClicked.AddDynamic(this, &UStartWidget_KMK::OnFailLogin);
		FailLoginPanel->SetVisibility(ESlateVisibility::Hidden);
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
		Butt_StartConcert->bIsEnabled = false;
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

#pragma region Create Ticket
	if (Butt_CreateTicket && Butt_CreateTicket1)
	{
		Butt_CreateTicket->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressCreateTicket );
		Butt_CreateTicket1->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressCreateTicket );
	}
#pragma endregion
#pragma region Set Particle
	if (Butt_AppearEffect && Butt_FeverEffect)
	{
		Butt_AppearEffect->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressAppearEffect );
		Butt_FeverEffect->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressFeverEffect );
	}
	if (Butt_Select &&Butt_Select1 && Butt_Upload)
	{
		Butt_Select->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressSelectButt );
		Butt_Select1->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressSelect1Butt );
		Butt_Upload->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressUpload );
    }	
	if (SetDayPanel && SetTicketPanel && StageChargePanel)
	{
		SetTitleText(TEXT("공연 설정" ) );

		SetPanelVisible ( SetDayPanel , SetTicketPanel  , StageChargePanel );
	}
#pragma endregion
#pragma region Pay Moneny
	if (Butt_PayMoney)
	{
		Butt_PayMoney->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressMoneyPay );
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
	if (Butt_Back1 && Butt_Back2 && Butt_Back && Butt_Back3)
	{
		Butt_Back->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back1->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back2->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back3->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
	}
#pragma endregion
#pragma region Session
	if (gi)
	{
		gi->OnSearchSignatureCompleteDelegate.AddDynamic(this, &UStartWidget_KMK::CreateRoomWidget );
		gi->SetStartWidget(this);
	}

#pragma endregion

	SetButtEnable( );
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
			concertInfo.ticketPrice = a;
			Text_Price->SetText(FText::AsNumber(a));
		}
	}
	if (!bCreateTicket)
	{
		matNum += 0.005f;
		ChangeLoadMat( matNum);
	}
	
}

#pragma region BackFunction
void UStartWidget_KMK::GoBack ( )
{
	selectManager->DeleteStage ( );
    StartSwitcher->SetActiveWidgetIndex (1);
	ClearSB( );
}
#pragma endregion


#pragma region LoginPanel

void UStartWidget_KMK::OnMyLogin ( )
{
	if(!httpActor) return;
    // 아이디와 패스워드가 없으면 실행 안되게 하기
	if (!EditText_ID->GetText ( ).IsEmpty ( ) && !EditText_PW->GetText ( ).IsEmpty ( ))
	{
		// 서버에 정보값 보내기
		httpActor->ReqLogin(EditText_ID->GetText().ToString(), EditText_PW->GetText().ToString());
		
		//StartSwitcher->SetActiveWidgetIndex ( 1 );
	}
}

void UStartWidget_KMK::OnFailLogin ( )
{
	FailLoginPanel->SetVisibility(ESlateVisibility::Hidden);
}

#pragma endregion

#pragma region FourButtPanel
void UStartWidget_KMK::ChangeMyProfile ( )
{
	Text_MyCash->SetText(FText::AsNumber(gi->myCash));
	Text_MyNick->SetText(FText::FromString(gi->GetMyInfo().userName));
}

void UStartWidget_KMK::SetButtEnable ( bool bEnable /*= false*/ )
{
	Butt_StartConcert->SetIsEnabled(bEnable);
	isButtEanble = bEnable;
}

// 무대 꾸미기 레벨로 이동
void UStartWidget_KMK::CreateStagePanel ( )
{
	UGameplayStatics::OpenLevel(GetWorld( ) , TEXT("JJH_SetupMap" ));
}
// 공연 일정 잡는 판넬로 변경
void UStartWidget_KMK::SettingStagePanel ( )
{
	StartSwitcher->SetActiveWidgetIndex ( 3 );
	Butt_UserStage->SetVisibility(ESlateVisibility::Visible);
	Butt_MyStage->SetVisibility(ESlateVisibility::Visible);
	Butt_Star->SetVisibility(ESlateVisibility::Visible);
	httpActor->ReqCheckStage(this);
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
	Butt_UserStage->SetVisibility(ESlateVisibility::Hidden);
	Butt_MyStage->SetVisibility(ESlateVisibility::Hidden);
	Butt_Star->SetVisibility(ESlateVisibility::Hidden);
	// 무대 조회하기
	httpActor->ReqCheckAllOpenConcert();
	FindRoom();
}

#pragma endregion

#pragma region Select Stage

void UStartWidget_KMK::PressUserStageButt ( )
{
	ClearSB( );
	// BE에서 스테이지 정보값 조회
	httpActor->ReqCheckStage(this);
}

void UStartWidget_KMK::PressMyStageButt ( )
{
	ClearSB( );
	// BE에서 스테이지 정보값 조회 
	httpActor->ReqCheckMyStage(this);
}

void UStartWidget_KMK::CreateStageWidget (const struct FStageInfo& stageInfo, UTexture2D* image )
{
	// 이펙트를 고르기 위한
	auto* wid = Cast<URoomWidget_KMK> ( CreateWidget ( GetWorld ( ) , roomWidgetFact ) );
	if (wid)
	{
		wid->SetStageText ( stageInfo, image );
		gi->sm = selectManager;
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
		Text_Effect1->SetText(FText::FromString(TEXT("공연 설정을 완료해주세요")));
		bool bNext = BEditTextEmpty ( );
		if(!bNext) 
		{
			return;
		}
		SetTitleText ( TEXT ( "티켓" ) );
		
		SetPanelVisible( SetTicketPanel , SetDayPanel, StageChargePanel );
		selectNum++;
	}
	else if (selectNum == 1)
	{
		Text_Effect1->SetText(FText::FromString(TEXT("VIP 가격 설정을 완료해주세요")));
		if(EditText_Price->GetText().IsEmpty()) 
		{
			EffectPopUp1->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		SetTitleText ( TEXT ( "이펙트 설정" ) );
		SetPanelVisible ( StageChargePanel , SetTicketPanel , SetDayPanel );
		selectNum++;
	}
	else
	{
		SetPanelVisible ( StageChargePanel , SetDayPanel , SetTicketPanel  );
		ClearAllText( );
		StageChargePanel->SetVisibility(ESlateVisibility::Visible);
		selectNum = 0;
	}

}


void UStartWidget_KMK::ChangeLoadMat ( float num )
{
	if(matNum < 1)loadMatInst->SetScalarParameterValue ( TEXT ( "Gauge" ) , num );
	else matNum = 0;
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
		
		concertInfo.name = EditText_StageName->GetText().ToString();

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
		FString mon = ChangeString(EditText_Mon->GetText().ToString());
		FString day = ChangeString(EditText_Day->GetText().ToString());

		concertInfo.concertDate = year + TEXT("-") + mon + TEXT("-") + day;

		Text_StartHour->SetText(EditText_SHour->GetText());
		Text_StartMin->SetText(EditText_SMin->GetText());
		FString sHour =  ChangeString(EditText_SHour->GetText().ToString());
		FString sMin = ChangeString(EditText_SMin->GetText().ToString());
		concertInfo.startTime = sHour + TEXT(":" ) + sMin + TEXT(":" ) + TEXT("00");

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
	else 
	{
		EffectPopUp1->SetVisibility(ESlateVisibility::Visible);
		return false;
	}
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

void UStartWidget_KMK::ChangeImageStage ( UTexture2D* texture )
{
	Image_Stage->SetBrushFromTexture(texture);
	//Image_SetStage->SetBrushFromTexture(texture);
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
	TMap<FString, FString> data;
	data.Add(TEXT("prompt" ), EditMultiText_Ticket->GetText().ToString());
	// 이 부분 정보는 BE에서 끌어와야함
    FString year = TEXT ( "20" ) + EditText_Year->GetText ( ).ToString ( );
    FString mon = ChangeString ( EditText_Day->GetText ( ).ToString ( ) );
    FString day = ChangeString ( EditText_Day->GetText ( ).ToString ( ) );
	
    FString sH = EditText_SHour->GetText ( ).ToString ( );
    FString sM = EditText_SMin->GetText ( ).ToString ( );

	FString concertString = TEXT("공연 명 : ") + EditText_StageName->GetText ( ).ToString ( ) + TEXT ( "\n" ) 
						TEXT("공연 날짜 : " ) + year + TEXT ( "년" )+ mon + TEXT("월") + day + TEXT("일") + TEXT("\n") + TEXT("공연 시간 : " ) + sH +TEXT("시") + sM +TEXT("분");
	data.Add(TEXT("description"), *concertString);
	// 티켓 만들기
	httpActor->ReqTicket(data);
	// EditMultiText_Ticket->SetText ( FText::GetEmpty ( ) );
}

void UStartWidget_KMK::CreateTicketMaterial ( UTexture2D* texture)
{
	Image_FinalStageImage->SetBrushFromTexture(texture);
	Image_FinalConcert->SetBrushFromTexture(texture);
}

void UStartWidget_KMK::PressUpload ( )
{
    TArray<FString> SelectedFiles;
    FString FileTypes = TEXT("이미지 파일 (*.png;*.jpg;*.jpeg)|*.png;*.jpg;*.jpeg|모든 파일 (*.*)|*.*");

    // 파일 탐색기를 열고 사용자에게 파일 선택을 요청
    bool bFileSelected =  OpenFileExample(SelectedFiles, TEXT("업로드할 이미지를 선택하세요"), FileTypes, true);

    if (bFileSelected && SelectedFiles.Num() > 0)
    {
        for (const FString& FilePath : SelectedFiles)
        {
            UE_LOG(LogTemp, Warning, TEXT("Selected File: %s"), *FilePath);
            FString FileName = FPaths::GetCleanFilename(FilePath);  // "Example.wav"
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("File Selected Failed!!!"))
    }
}
// 파일 열기
bool UStartWidget_KMK::OpenFileExample(TArray<FString>& FileNames, FString DialogueTitle, FString FileTypes, bool multiselect)
{

   IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
   bool bOpened = false;
   FString DefaultPath = FPaths::ProjectContentDir(); // 기본 경로를 프로젝트 콘텐츠 폴더로 설정

   if (DesktopPlatform)
   {
      uint32 SelectionFlag = multiselect ? EFileDialogFlags::Multiple : EFileDialogFlags::None;
      bOpened = DesktopPlatform->OpenFileDialog(
         NULL,
         DialogueTitle,
         DefaultPath,
         TEXT(""),
         FileTypes,
         SelectionFlag,
         FileNames
      );
   }
   return bOpened;
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

	concertInfo.ticketPrice = FCString::Atoi(*EditText_Price->GetText().ToString());

	EditText_Price->SetText( FText::GetEmpty ( ) );
	EditMultiText_Ticket->SetText( FText::GetEmpty ( ) );

	concertInfo.appearedVFX = particleNum;
	concertInfo.feverVFX = feverNum;

	particleNum = 0;
	feverNum = 0;
	
	bCreateTicket =false;
}


void UStartWidget_KMK::SetPanelVisible ( class UCanvasPanel* visiblePanel , class UCanvasPanel* hiddenPanel0 , class UCanvasPanel* hiddenPanel1  )
{
	hiddenPanel0->SetVisibility ( ESlateVisibility::Hidden );
	hiddenPanel1->SetVisibility ( ESlateVisibility::Hidden );
	visiblePanel->SetVisibility ( ESlateVisibility::Visible );
}

void UStartWidget_KMK::PressAppearEffect ( )
{
	bFever = false;
}

void UStartWidget_KMK::PressFeverEffect ( )
{
	bFever = true;
}

void UStartWidget_KMK::PressSelect1Butt ( )
{
	if (particleNum < 0 || feverNum < 0)
	{
		EffectPopUp->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	concertInfo.appearedVFX = particleNum;
	concertInfo.feverVFX = feverNum;
	StartSwitcher->SetActiveWidgetIndex(2);
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
	if (EditText_ScaleNum->GetText ( ).IsEmpty ( ))
	{
		Text_Effect1->SetText(FText::FromString(TEXT("수용인원 설정을 완료해주세요")));
		EffectPopUp->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	concertInfo.peopleScale = FCString::Atoi(*EditText_ScaleNum->GetText().ToString());
	StagePayPanel->SetVisibility ( ESlateVisibility::Visible );
	Text_Pay->SetVisibility ( ESlateVisibility::Visible );
	Butt_PayMoney->SetVisibility ( ESlateVisibility::Visible );
	StageScalePanel->SetVisibility ( ESlateVisibility::Hidden );
	Text_FinalCount->SetText ( EditText_ScaleNum->GetText() );
	Text_FinalPay->SetText ( Text_Price->GetText() );
	FString s = Text_FinalPay->GetText().ToString();
	gi->myCash -= FCString::Atoi(*s);
	Butt_Next->SetVisibility ( ESlateVisibility::Hidden );
	Butt_CreateTicket1->SetVisibility(ESlateVisibility::Visible);
	Text_Price->SetText(FText::GetEmpty ( ) );
	EditText_ScaleNum->SetText ( FText::GetEmpty ( ) );
}

void UStartWidget_KMK::PressMoneyPay ( )
{
	//if (!bCreateTicket)
	//{
	//	Text_Effect1->SetVisibility(ESlateVisibility::Hidden);
	//	MultiText_PopUp->SetVisibility(ESlateVisibility::Visible);
	//	EffectPopUp1->SetVisibility(ESlateVisibility::Visible);
	//	return;
	//}
	//else
	//{
	//	Text_Effect1->SetVisibility(ESlateVisibility::Visible);
	//	MultiText_PopUp->SetVisibility(ESlateVisibility::Hidden);
	//	EffectPopUp1->SetVisibility(ESlateVisibility::Hidden);
	//	Image_Load->SetVisibility(ESlateVisibility::Hidden);
	//}
	concertInfo.stageId = gi->stageNum;
	UE_LOG(LogTemp, Warning, TEXT("%d" ), concertInfo.stageId);
	
	httpActor->ReqSetMyConcert(concertInfo);
}

void UStartWidget_KMK::PressOkayButt ( )
{
	Text_MyCash->SetText(FText::AsNumber(gi->myCash));

	ClearSB ( );
	StartSwitcher->SetActiveWidgetIndex ( 1 );
	ResetWidget( );
	httpActor->ReqCheckMyConcert();
}

void UStartWidget_KMK::ResetWidget ( )
{
	SetTitleText ( TEXT ( "공연 설정" ) );
	SetPanelVisible ( SetDayPanel , SetTicketPanel , StageChargePanel );

	StageChargePanel->SetVisibility(ESlateVisibility::Hidden);
	StageScalePanel->SetVisibility ( ESlateVisibility::Visible );
	StagePayPanel->SetVisibility ( ESlateVisibility::Hidden );
	Text_Pay->SetVisibility ( ESlateVisibility::Hidden );
	Butt_PayMoney->SetVisibility ( ESlateVisibility::Hidden );
	PayPopUpPanel->SetVisibility ( ESlateVisibility::Hidden );
	Butt_Next->SetVisibility ( ESlateVisibility::Visible );
	Butt_CreateTicket1->SetVisibility(ESlateVisibility::Hidden);
}

#pragma endregion

#pragma endregion

#pragma region Entry Panel
// vip 접속
void UStartWidget_KMK::PressYesButt ( )
{
	if (gi && gi->roomNum >= 0)
	{
		httpActor->ReqCheckIdStage(gi->GetConcertInfo().stageId );
		
		if (TEXT_VIP->GetText ( ).ToString ( ).Contains ( TEXT ( "VIP" ) ))
        {
			gi->JoinRoom(gi->roomNum , 1);
        }
		else
		{
			gi->JoinRoom(gi->roomNum );
		}
		//ChangeAudienceMesh(0);
	}
}
// 일반 접속
void UStartWidget_KMK::PressNormalEntry ( )
{
	TEXT_VIP->SetText(FText::FromString(TEXT("일반 입장하시겠습니까?" )));
	MultiText_VIP->SetText(FText::FromString(TEXT("VIP 입장을 이용해보세요!\nVIP 입장시 더 많은 혜택을 누릴 수 있습니다" )));
	VIPPopUpPanel->SetVisibility(ESlateVisibility::Visible);
}
void UStartWidget_KMK::PressNoButt ( )
{
	VIPPopUpPanel->SetVisibility(ESlateVisibility::Hidden);
}

// 세션 확인할 수 있는 UI 띄우기
void UStartWidget_KMK::PressVipEntry ( )
{
	TEXT_VIP->SetText(FText::FromString(TEXT("VIP로 입장하시겠습니까?" )));
	MultiText_VIP->SetText(FText::FromString(TEXT("VIP 입장시 앞열 관람, 티켓 콜렉션 수집이 가능하며,\n커스텀 응원봉으로 입장 등의 혜택이 있습니다" )));
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


