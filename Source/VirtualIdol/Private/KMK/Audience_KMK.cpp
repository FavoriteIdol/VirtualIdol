// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Audience_KMK.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableText.h"
#include "KMK/SendChat_KMK.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/ScrollBox.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "KMK/AudienceServerComponent_KMK.h"
#include "Components/TextBlock.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Components/Image.h"
#include "Components/GridPanel.h"
#include "DesktopPlatformModule.h"
#include "Sound/SoundWaveProcedural.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Audio.h"
#include "Components/AudioComponent.h"
#include "Components/VerticalBoxSlot.h"
#include "KMK/SingWidget_KMK.h"
#include "Components/VerticalBox.h"
#include "Sound/SoundEffectSource.h"
#include <chrono>
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "HSW/HSW_PlayerController.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Components/HorizontalBox.h"
#include "HSW/HSW_ImojiConponent.h"
#include "HSW/HSW_FeverGaugeWidget.h"
#include "JJH/JJH_SetupGameModeBase.h"
#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_GameState_Auditorium.h"
#include "HSW_ThrowingObject.h"

void UAudience_KMK::NativeConstruct ( )
{
    Super::NativeConstruct( );
    SetUpButtonInfo();
    gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
    if (Text_MyCash)
    {
        Text_MyCash->SetText(FText::AsNumber(gi->myCash));
    }
    if (Butt_Yes && Butt_No && PopUpPanel && FeverGauge)
    {
        VisiblePanel(ESlateVisibility::Hidden);
        Butt_Yes->OnClicked.AddDynamic ( this , &UAudience_KMK::PressYesButt );
        Butt_No->OnClicked.AddDynamic ( this , &UAudience_KMK::PressNoButt );

    }
    if (Image_SImoji)
    {
        SetSImojiVisible(ESlateVisibility::Hidden);
    }
#pragma region Chat
    if (Butt_Send)
    {
        Butt_Send->OnClicked.AddDynamic ( this , &UAudience_KMK::PressSendButt);
    }
#pragma endregion
#pragma region ButtonInfoSetting
     if (Butt_Hidden && Butt_Mode && Butt_Mike && Butt_Chat && Butt_Emotion)
    {
        Butt_Hidden->OnClicked.AddDynamic ( this , &UAudience_KMK::PressHiddenButt);
        Butt_Mode->OnClicked.AddDynamic ( this , &UAudience_KMK::PressModeButt);
        Butt_Mike->OnClicked.AddDynamic ( this , &UAudience_KMK::PressMikeButt);
        Butt_Chat->OnClicked.AddDynamic ( this , &UAudience_KMK::PressChatButt);
        Butt_Emotion->OnClicked.AddDynamic ( this , &UAudience_KMK::PressEmotionButt);
    }
#pragma endregion
#pragma region Exit
    if (Butt_Exit && Butt_Exit1)
    {
        Butt_Exit->OnClicked.AddDynamic ( this , &UAudience_KMK::PressExitButt);
        Butt_Exit1->OnClicked.AddDynamic ( this , &UAudience_KMK::PressExit1Butt);
    }
    if (ExitPanel && ExitPanel1 && Butt_Cancel && Butt_Cancel1 && Butt_Out && Butt_Out1)
    {
        ExitPanel->SetVisibility(ESlateVisibility::Hidden);
        ExitPanel1->SetVisibility(ESlateVisibility::Hidden);
        Butt_Cancel->OnClicked.AddDynamic ( this , &UAudience_KMK::PressCancelButt);
        Butt_Cancel1->OnClicked.AddDynamic ( this , &UAudience_KMK::PressCancelButt);
        Butt_Out->OnClicked.AddDynamic ( this , &UAudience_KMK::PressOutButt);
        Butt_Out1->OnClicked.AddDynamic ( this , &UAudience_KMK::PressOutButt);
    }
    if (ChatGridPanel)
    {
        ChatGridPanel->SetVisibility(ESlateVisibility::Hidden);
    }
#pragma endregion
#pragma region Charge
    if (Butt_Object0 && Butt_Object1 && Butt_Object2)
    {
        Butt_Object0->OnClicked.AddDynamic ( this , &UAudience_KMK::PressObjectButt);
        Butt_Object1->OnClicked.AddDynamic ( this , &UAudience_KMK::PressObject1Butt);
        Butt_Object2->OnClicked.AddDynamic ( this , &UAudience_KMK::PressObject2Butt);
    }
#pragma endregion
#pragma region StartConcert
    if (CountDownPanel)
    {
        CountDownPanel->SetVisibility ( ESlateVisibility::Hidden );
    }
#pragma endregion
#pragma region BeforeConcerForVirtual
    if (Butt_MP3 && Butt_Model)
    {
        Butt_Model->OnClicked.AddDynamic ( this , &UAudience_KMK::PressButtModel);
    }
#pragma endregion
#pragma region Imoji
    if (ImojiBox)
    {
        ImojiBox->SetVisibility(ESlateVisibility::Hidden);
    }
    if(Btn_Imoji_1 && Btn_Imoji_2 && Btn_Imoji_3 && Btn_Imoji_4)
    {
        Btn_Imoji_1->OnClicked.AddDynamic ( this, &UAudience_KMK::OnMyImoji01 );
        Btn_Imoji_2->OnClicked.AddDynamic ( this , &UAudience_KMK::OnMyImoji02 );
	    Btn_Imoji_3->OnClicked.AddDynamic ( this , &UAudience_KMK::OnMyImoji03 );
	    Btn_Imoji_4->OnClicked.AddDynamic ( this , &UAudience_KMK::OnMyImoji04 );
    }


	Player = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn() );
	if(Player)ImojiComponent = Player->GetComponentByClass<UHSW_ImojiConponent>();
#pragma endregion



}


void UAudience_KMK::NativePreConstruct ( )
{
    Super::NativePreConstruct();


}

#pragma region PopUp

void UAudience_KMK::SetUpButtonInfo ( )
{
    for (int i = 1; i < buttonName.Num ( ); i++)
    {
        FName bName = FName(*FString::Printf(TEXT("Butt_") ) + buttonName[i]);
        // FName bImgName = FName(*FString::Printf(TEXT("Image_Back_")  )+ buttonName[i] );
        FName iName = FName(*FString::Printf(TEXT("Image_") )+ buttonName[i] );
        FName tName = FName(*FString::Printf(TEXT("Text_")  )+ buttonName[i] );

        UButton* butt = Cast<UButton>(GetWidgetFromName(bName));
        // UImage* bImg = Cast<UImage>(GetWidgetFromName(bImgName));
        UImage* img = Cast<UImage>(GetWidgetFromName(iName));
        UTextBlock* text = Cast<UTextBlock>(GetWidgetFromName(tName));

        if (butt && img && text)
        {
            FButtonInfo ButtonInfo;
            ButtonInfo.button = butt;
            ButtonInfo.image = img;
            //ButtonInfo.backImage = bImg;
            ButtonInfo.text = text;
            ButtonsInfoArray.Add(ButtonInfo);
        }
        
    }
}
void UAudience_KMK::PressHiddenButt ( )
{
    if (!bHide)
    {
        ButtPanel->SetVisibility(ESlateVisibility::Hidden);
        bHide = true;
    }
    else 
    {
        ButtPanel->SetVisibility(ESlateVisibility::Visible);
        bHide = false;
    }
}


void UAudience_KMK::PressModeButt ( )
{
    OnOffFunction(Text_Mode, 0 );

    if (bMondeOn)
    {
        bMondeOn = false;
        SetVisibleActor (bMondeOn);
        ButtonsInfoArray[0].image->SetBrushFromTexture ( changeImageArray[0] );
    }
    else
    {
        bMondeOn = true;
        SetVisibleActor (bMondeOn );
        ButtonsInfoArray[0].image->SetBrushFromTexture ( normalImageArray[0] );
    }
}
void UAudience_KMK::SetVisibleActor ( bool bActive )
{
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("OnOff" ), actors );
    APawn* myPC = GetWorld()->GetFirstPlayerController()->GetPawn();
    for (AActor* c : actors)
    {
        if (c != myPC)
        {
            c->SetActorHiddenInGame(bActive);
        }
    }
}

void UAudience_KMK::StartVoiceChat ( )
{
    UE_LOG(LogTemp, Warning, TEXT("StartTalk" ) );
    pc->GetController<AHSW_PlayerController>( )->StartTalking ( );
}

void UAudience_KMK::StopVoiceChat ( )
{
    UE_LOG ( LogTemp , Warning , TEXT ( "StopTalk" ) );
    pc->GetController<AHSW_PlayerController>( )->StopTalking ( );
}

void UAudience_KMK::PressMikeButt ( )
{
    OnOffFunction ( Text_Mike , 1 );
    if (bMikeOn)
    {
        StartVoiceChat ( );
        ButtonsInfoArray[1].image->SetBrushFromTexture( changeImageArray[1]);
        bMikeOn = false;
    }
    else
    {
        StopVoiceChat ( );
        ButtonsInfoArray[1].image->SetBrushFromTexture ( normalImageArray[1] );
        bMikeOn = true;
    }
}

void UAudience_KMK::OnOffFunction (class UTextBlock* textBlocks, int32 num , bool bAllVisib )
{
    if (textBlocks->GetText().ToString() == currentText[num + 1])
    {
        if(bAllVisib)OnOffInfo(FLinearColor::Yellow, ESlateVisibility::Hidden, num, changeText );
        else ChangeTextAndImage ( FLinearColor::Yellow , num, changeText );
    }
    else
    {
        if (bAllVisib) OnOffInfo(FLinearColor::White, ESlateVisibility::Visible, num, currentText);
        else ChangeTextAndImage( FLinearColor::White, num, currentText);
    }
}

void UAudience_KMK::ChangeTextAndImage ( FLinearColor color , int32 num , TArray<FString> textArray , bool bMyAuth )
{
    ButtonsInfoArray[num].image->SetBrushTintColor ( color );
    ButtonsInfoArray[num].text->SetText ( FText::FromString ( textArray[num + 1] ) );
    if (bMyAuth)
    {
        VipAuthority ( );
    }
}

void UAudience_KMK::SetVirtualWBP ( )
{
    ChatGridPanel->SetVisibility ( ESlateVisibility::Visible );
    ChatPanel->SetVisibility ( ESlateVisibility::Visible );
}

void UAudience_KMK::PressChatButt ( )
{
    if (!bChatOn)
    {
        ChatGridPanel->SetVisibility(ESlateVisibility::Visible);
        bChatOn = true;
        ChangeTextAndImage (FLinearColor::Yellow, 2, changeText );
    }
    else
    {
        bChatOn = false;
        ChatGridPanel->SetVisibility(ESlateVisibility::Hidden);
        ChangeTextAndImage (FLinearColor::White, 2, currentText);
    }
}

void UAudience_KMK::PressEmotionButt ( )
{
    if (!bEmotion)
    {
        bEmotion = true;
        ChangeTextAndImage ( FLinearColor::Yellow , 3 , changeText );
        ImojiBox->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        bEmotion = false;
        ChangeTextAndImage ( FLinearColor::White , 3 , currentText );
        ImojiBox->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UAudience_KMK::OnOffInfo ( FLinearColor color,  ESlateVisibility bVisib, int32 num, TArray<FString> textArray  )
{
    for (int i = 1; i < ButtonsInfoArray.Num(); i++)
    {
        if (i == num)
        {
            ButtonsInfoArray[i].image->SetBrushTintColor(color);
            //ButtonsInfoArray[i].backImage->SetVisibility(ESlateVisibility::Visible);
            ButtonsInfoArray[i].text->SetText(FText::FromString(textArray[i]));
        }
        else
        {
            if(bMyVip && i == ButtonsInfoArray.Num() - 1) continue;
            ButtonsInfoArray[i].button->SetVisibility ( bVisib );
            //ButtonsInfoArray[i].backImage->SetVisibility ( bVisib );
            ButtonsInfoArray[i].image->SetVisibility ( bVisib );
            ButtonsInfoArray[i].text->SetVisibility ( bVisib );
        }
    }
}
void UAudience_KMK::VipAuthority ( )
{
    bMyVip = true;

    ButtonsInfoArray[5].button->SetVisibility ( ESlateVisibility::Hidden );
    // ButtonsInfoArray[5].backImage->SetVisibility ( ESlateVisibility::Hidden );
    ButtonsInfoArray[5].image->SetVisibility ( ESlateVisibility::Hidden );
    ButtonsInfoArray[5].text->SetVisibility ( ESlateVisibility::Hidden );
}

void UAudience_KMK::PressStartConcertButt ( )
{
    gi->playerMeshNum = 2;
    // Butt_StartConcert->SetVisibility(ESlateVisibility::Hidden);
    pc->FindComponentByClass<UAudienceServerComponent_KMK> ( )->ServerRPC_StartConcert( );
}

void UAudience_KMK::PressYesButt ( )
{
    gi->playerMeshNum = 1;
    pc->FindComponentByClass<UAudienceServerComponent_KMK>( )->ServerRPC_ChangeMyMesh(1);
    ChangeTextAndImage (FLinearColor::Yellow, 5, changeText, true );
    VisiblePanel ( ESlateVisibility::Hidden );
}

void UAudience_KMK::PressNoButt ( )
{
    gi->playerMeshNum = 0;
    pc->FindComponentByClass<UAudienceServerComponent_KMK>( )->ServerRPC_ChangeMyMesh(0);
    ChangeTextAndImage (FLinearColor::White, 5, currentText );
    VisiblePanel(ESlateVisibility::Hidden);
}

void UAudience_KMK::VisiblePanel ( ESlateVisibility visible )
{
    PopUpPanel->SetVisibility ( visible );
}
#pragma endregion
#pragma region Chat
// 플레이어에 서버에 보내는 RPC 함수 작성해야함
void UAudience_KMK::PressSendButt ( )
{
    auto* player = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (player && !player->HasAuthority())
    {
        auto* server = player->GetComponentByClass<UAudienceServerComponent_KMK> ( );
        if (server)
        {
            if (!EditText_Chat->GetText ( ).IsEmpty ( ))
            {
                server->ServerRPCChat(gi->GetMyInfo().userName, EditText_Chat->GetText ( ).ToString ( ) );

                EditText_Chat->SetText ( FText::GetEmpty ( ) );
            }
        }
    }
    else
    {
        AHSW_GameState_Auditorium* gs = GetWorld()->GetGameState<AHSW_GameState_Auditorium>();
        if (gs)
        {
            if (!EditText_Chat->GetText ( ).IsEmpty ( ))
            {
                gs->ServerRPCChat (gi->GetMyInfo().userName, EditText_Chat->GetText ( ).ToString ( ) );
                EditText_Chat->SetText ( FText::GetEmpty ( ) );
            }
        }
    }
}

void UAudience_KMK::CreateChatWidget (const FString& nickname, const FString& chatText )
{
    auto* chat = CreateWidget<USendChat_KMK> ( this, chatWidgetFact );
    chat->SetChatText(nickname, chatText);
    SB_ChatLog->AddChild ( chat );
    SB_ChatLog->ScrollToEnd();
}
#pragma endregion


void UAudience_KMK::PressExitButt ( )
{
    ExitPanel->SetVisibility(ESlateVisibility::Visible);
}

void UAudience_KMK::PressExit1Butt ( )
{
    ExitPanel1->SetVisibility(ESlateVisibility::Visible);
}

void UAudience_KMK::PressOutButt ( )
{
    gi->ExitRoom ( );
}

void UAudience_KMK::PressCancelButt ( )
{
    ExitPanel->SetVisibility(ESlateVisibility::Hidden);
    ExitPanel1->SetVisibility(ESlateVisibility::Hidden);
}

#pragma region Cash
void UAudience_KMK::PressObjectButt ( )
{
    gi->myCash -= 500;
    Text_MyCash->SetText(FText::AsNumber(gi->myCash));
    // 오브젝트 생성
    Player->ThrowingObjectIndex = 0;
    UE_LOG(LogTemp,Warning,TEXT("ObjectButton_0" ) );
}
void UAudience_KMK::PressObject1Butt ( )
{
    gi->myCash -= 1000;
    Text_MyCash->SetText(FText::AsNumber(gi->myCash));
    Player->ThrowingObjectIndex = 1;
    UE_LOG ( LogTemp , Warning , TEXT ( "ObjectButton_1" ) );
}
void UAudience_KMK::PressObject2Butt ( )
{
    gi->myCash -= 5000;
    Text_MyCash->SetText(FText::AsNumber(gi->myCash));
    Player->ThrowingObjectIndex = 2;
    UE_LOG ( LogTemp , Warning , TEXT ( "ObjectButton_2" ) );
}

void UAudience_KMK::CountDownText (const FString& time )
{
    Text_CoutDown->SetText(FText::FromString(time));
}

void UAudience_KMK::CountDownPanelVisible ( ESlateVisibility visiblePanel )
{
    if(CountDownPanel)CountDownPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UAudience_KMK::SetCountDownTextVisible ( )
{
    if(TEXT_Min1) TEXT_Min1->SetVisibility(ESlateVisibility::Hidden);
    if(FeverGauge) FeverGauge->SetVisibility(ESlateVisibility::Visible);
}

void UAudience_KMK::BeforeStartConcertCount (const FString& time )
{
    if(TEXT_Min) TEXT_Min->SetVisibility(ESlateVisibility::Visible);
    TEXT_Min->SetText(FText::FromString(time));
}

#pragma endregion
#pragma region Audience
void UAudience_KMK::SetSImojiVisible ( ESlateVisibility visible, int32 index )
{
    Image_SImoji->SetVisibility(visible);
    if (Image_SImoji->GetVisibility ( ) == ESlateVisibility::Visible)
    {
        Image_SImoji->SetBrushFromTexture(sImojiArray[index] );
    }
}


void UAudience_KMK::OnMyImoji01 ( )
{
	Player->Imoji (0);
    SetSImojiVisible(ESlateVisibility::Visible, 0);
}

void UAudience_KMK::OnMyImoji02 ( )
{
	Player->Imoji ( 1 );
    SetSImojiVisible(ESlateVisibility::Visible, 1);
}

void UAudience_KMK::OnMyImoji03 ( )
{
	Player->Imoji ( 2 );
    SetSImojiVisible(ESlateVisibility::Visible, 2);
}

void UAudience_KMK::OnMyImoji04 ( )
{
	Player->Imoji ( 3 );
    SetSImojiVisible(ESlateVisibility::Visible, 3);
}
#pragma endregion

#pragma region Before Concert

void UAudience_KMK::ChangeTextClock ( const FString& text )
{
    TEXT_Min->SetText(FText::FromString(text));
    TEXT_Min1->SetText(FText::FromString(text));
}


void UAudience_KMK::PressButtModel ( )
{

}
// 파일 열기
bool UAudience_KMK::OpenFileExample(TArray<FString>& FileNames, FString DialogueTitle, FString FileTypes, bool multiselect)
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

void UAudience_KMK::SetConcertName ( const FString& text )
{
    if(TEXT_ConcertName) TEXT_ConcertName->SetText(FText::FromString(text));
}

// 오디오 파일을 불러와 USoundWave로 변환
USoundWaveProcedural* UAudience_KMK::LoadWavFromFile(const FString& FilePath)
{
    TArray<uint8> RawFileData;

    // 파일 데이터를 메모리로 로드
    if (!FFileHelper::LoadFileToArray ( RawFileData , *FilePath ))
    {
        UE_LOG ( LogTemp , Error , TEXT ( "Failed to load file: %s" ) , *FilePath );
        return nullptr;
    }

    // WAV 파일 헤더 정보 추출
    FWaveModInfo WaveInfo;
    if (!WaveInfo.ReadWaveInfo ( RawFileData.GetData ( ) , RawFileData.Num ( ) ))
    {
        UE_LOG ( LogTemp , Error , TEXT ( "Invalid WAV format: %s" ) , *FilePath );
        return nullptr;
    }

    int32 BitsPerSample = *WaveInfo.pBitsPerSample;
    int32 SampleRate = *WaveInfo.pSamplesPerSec;

    USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural> ( );
    if (!SoundWave)
    {
        UE_LOG ( LogTemp , Error , TEXT ( "Failed to create SoundWave object." ) );
        return nullptr;
    }

    SoundWave->NumChannels = *WaveInfo.pChannels;
    SoundWave->SetSampleRate ( SampleRate );

    uint32 BytesPerSample = ( *WaveInfo.pChannels ) * ( BitsPerSample / 8 );
    uint32 TotalSamples = WaveInfo.SampleDataSize / BytesPerSample;
    SoundWave->Duration = static_cast<float>( TotalSamples ) / static_cast<float>( SampleRate );

    // 비트 깊이에 따른 필터 적용

    // 비트 깊이에 따른 필터 적용 및 증폭
    if (BitsPerSample == 16)
    {
        const int16* PCMDataStart = reinterpret_cast<const int16*>( WaveInfo.SampleDataStart );
        TArray<int16> PCMData;
        PCMData.Append ( PCMDataStart , WaveInfo.SampleDataSize / sizeof ( int16 ) );

        SoundWave->QueueAudio ( reinterpret_cast<const uint8*>( PCMData.GetData ( ) ) , PCMData.Num ( ) * sizeof ( int16 ) );
    }
    else if (BitsPerSample == 24)
    {
        const uint8* PCMDataStart = reinterpret_cast<const uint8*>( WaveInfo.SampleDataStart );
        if (!PCMDataStart)
        {
            UE_LOG ( LogTemp , Error , TEXT ( "Failed to cast SampleDataStart to uint8 for file: %s" ) , *FilePath );
            return nullptr;
        }

        TArray<int32> PCMData;
        PCMData.Reserve ( WaveInfo.SampleDataSize / 3 );

        // 24비트 PCM 데이터를 32비트로 변환
        for (uint32 i = 0; i < WaveInfo.SampleDataSize; i += 3)
        {
            int32 Sample = ( PCMDataStart[i] << 16 ) | ( PCMDataStart[i + 1] << 8 ) | PCMDataStart[i + 2];
            PCMData.Add ( Sample );
        }

        // 다시 24비트로 저장 (필터링된 데이터를 3바이트로 다시 압축)
        TArray<uint8> FilteredPCMData;
        FilteredPCMData.Reserve ( PCMData.Num ( ) * 3 );
        for (int32 Sample : PCMData)
        {
            FilteredPCMData.Add ( ( Sample >> 16 ) & 0xFF );
            FilteredPCMData.Add ( ( Sample >> 8 ) & 0xFF );
            FilteredPCMData.Add ( Sample & 0xFF );
        }

        // SoundWave에 24비트 데이터를 다시 3바이트 단위로 큐잉
        SoundWave->QueueAudio ( FilteredPCMData.GetData ( ) , FilteredPCMData.Num ( ) );
    }
    else if (BitsPerSample == 32)
    {
        const float* PCMDataStart = reinterpret_cast<const float*>( WaveInfo.SampleDataStart );
        TArray<float> PCMData;
        PCMData.Append ( PCMDataStart , WaveInfo.SampleDataSize / sizeof ( float ) );

        SoundWave->QueueAudio ( reinterpret_cast<const uint8*>( PCMData.GetData ( ) ) , PCMData.Num ( ) * sizeof ( float ) );
    }
    SoundWave->bLooping = false;
    SoundWave->bProcedural = true;
    SoundWave->SoundGroup = SOUNDGROUP_Default;
    SoundWave->bStreaming = false;

    return SoundWave;
}

void UAudience_KMK::ChangeVirtualWidget ( )
{
    WS_Concert->SetActiveWidgetIndex(1);
    SetVirtualWBP();
    TEXT_Min1->SetVisibility(ESlateVisibility::Hidden);
    FeverGauge->SetVisibility(ESlateVisibility::Visible);
}

#pragma endregion