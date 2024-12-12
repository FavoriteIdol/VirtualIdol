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
#include "HSW/HSW_SongUnit.h"

void UAudience_KMK::NativeConstruct ( )
{
    Super::NativeConstruct( );
    SetUpButtonInfo();
    gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
    // 월드에 배치된 httpActor 찾기
    httpActor = Cast<AHttpActor_KMK> ( UGameplayStatics::GetActorOfClass ( GetWorld ( ) , httpFact ) );
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
    if (Butt_MP3 && Butt_Model)
    {
        Butt_MP3->OnClicked.AddDynamic ( this , &UAudience_KMK::PressButtMp3 );
        Butt_Model->OnClicked.AddDynamic ( this , &UAudience_KMK::PressButtModel );
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
    // 
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
        FWidgetTransform Transform = Butt_Hidden->RenderTransform; // 현재 Transform 가져오기
        Transform.Angle = 180.0f; // 각도를 0으로 설정
        Butt_Hidden->SetRenderTransform ( Transform ); // 수정된 Transform 적용
        bHide = true;
    }
    else 
    {
        ButtPanel->SetVisibility(ESlateVisibility::Visible);
        FWidgetTransform Transform = Butt_Hidden->RenderTransform; // 현재 Transform 가져오기
        Transform.Angle = 0.0f; // 각도를 0으로 설정
        Butt_Hidden->SetRenderTransform ( Transform ); // 수정된 Transform 적용
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
    //if (bMyAuth)
    //{
    //    VipAuthority ( );
    //}
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
        HB_CamPanel->SetVisibility(ESlateVisibility::Hidden );
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
                httpActor->ReqTranslateChat( EditText_Chat->GetText ( ).ToString ( ), server);

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
                httpActor->ReqTranslateChat ( EditText_Chat->GetText ( ).ToString ( ) , gs );
                
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

void UAudience_KMK::ObjectButtEnable ( )
{
    Butt_Object0->SetIsEnabled ( false );
    Butt_Object1->SetIsEnabled ( false );
    Butt_Object2->SetIsEnabled ( false );
}

void UAudience_KMK::PressObjectButt ( )
{
    gi->myCash -= 500;
    Text_MyCash->SetText(FText::AsNumber(gi->myCash));
    if (gi->myCash <= 0)
    {
        gi->myCash = 0;
        ObjectButtEnable ( );
        return;
    }
    // 오브젝트 생성
    Player->ThrowingObjectIndex = 0;

    Player->OnMyThorwHold();
    Player->OnMyThorwPitch ( );
    UE_LOG(LogTemp,Warning,TEXT("ObjectButton_0" ) );
}
void UAudience_KMK::PressObject1Butt ( )
{
    gi->myCash -= 1000;
    Text_MyCash->SetText ( FText::AsNumber ( gi->myCash ) );
    if (gi->myCash <= 0)
    {
        gi->myCash = 0;
        ObjectButtEnable ( );
        return;
    }


    Player->ThrowingObjectIndex = 1;
    Player->OnMyThorwHold ( );
    Player->OnMyThorwPitch ( );
    UE_LOG ( LogTemp , Warning , TEXT ( "ObjectButton_1" ) );
}
void UAudience_KMK::PressObject2Butt ( )
{
    gi->myCash -= 5000;
    Text_MyCash->SetText ( FText::AsNumber ( gi->myCash ) );
    if (gi->myCash <= 0)
    {
        gi->myCash = 0;
        ObjectButtEnable ( );
        return;
    }

    Player->ThrowingObjectIndex = 2;
    Player->OnMyThorwHold ( );
    Player->OnMyThorwPitch ( );
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


void UAudience_KMK::SetImojiBox ( )
{
    ImojiBox->SetVisibility ( ESlateVisibility::Hidden );
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


void UAudience_KMK::SetConcertName ( const FString& text )
{
    if(TEXT_ConcertName) TEXT_ConcertName->SetText(FText::FromString(text));
}


void UAudience_KMK::ChangeVirtualWidget ( )
{
    WS_Concert->SetActiveWidgetIndex(1);
    SetVirtualWBP();
    TEXT_Min1->SetVisibility(ESlateVisibility::Hidden);
    FeverGauge->SetVisibility(ESlateVisibility::Visible);
}

#pragma endregion
#pragma region Upload Mp3
void UAudience_KMK::PressButtMp3 ( )
{
    TArray<FString> SelectedFiles;

    FString FileTypes = TEXT ( "Audio Files (*.mp3;*.wav)|*.mp3;*.wav|All Files (*.*)|*.*" );

    // 파일 탐색기를 열고 사용자에게 파일 선택을 요청
    bool bFileSelected = OpenFileExample ( SelectedFiles , TEXT ( "Select an Image to Upload" ) , FileTypes , true );

    if (bFileSelected && SelectedFiles.Num ( ) > 0)
    {
        for (const FString& FilePath : SelectedFiles)
        {
            UE_LOG ( LogTemp , Warning , TEXT ( "Selected File: %s" ) , *FilePath );
            FString FileName = FPaths::GetCleanFilename ( FilePath );  // "Example.wav"

            USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural> ( );
            if (SoundWave)
            {
                UAudioComponent* AudioComponent = NewObject<UAudioComponent> ( GetWorld ( ) , UAudioComponent::StaticClass ( ) );
                
                if (AudioComponent && SoundWave)
                {
                    AudioComponent->bAutoActivate = false;
                    AudioComponent->SetSound ( SoundWave );
                }
            }
            else
            {
                UE_LOG ( LogTemp , Error , TEXT ( "Failed to load audio: %s" ) , *FilePath );
            }
        }
    }
    else
    {
        UE_LOG ( LogTemp , Warning , TEXT ( "File Selected Failed!!!" ) )
    }

}

// 파일 열기
bool UAudience_KMK::OpenFileExample ( TArray<FString>& FileNames , FString DialogueTitle , FString FileTypes , bool multiselect )
{

    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get ( );
    bool bOpened = false;
    FString DefaultPath = FPaths::ProjectContentDir ( ); // 기본 경로를 프로젝트 콘텐츠 폴더로 설정

    if (DesktopPlatform)
    {
        uint32 SelectionFlag = multiselect ? EFileDialogFlags::Multiple : EFileDialogFlags::None;
        bOpened = DesktopPlatform->OpenFileDialog (
           NULL ,
           DialogueTitle ,
           DefaultPath ,
           TEXT ( "" ) ,
           FileTypes ,
           SelectionFlag ,
           FileNames
        );
    }
    return bOpened;
}
// 오디오 파일을 불러와 USoundWave로 변환
USoundWaveProcedural* UAudience_KMK::LoadWavFromFile ( const FString& FilePath )
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

        AmplifyPCM16 ( PCMData , soundGain );  // 16비트 데이터 증폭
        ApplyHighPassFilter16 ( PCMData , 100.0f , SampleRate );
        ApplyLowPassFilter16 ( PCMData , 5000.0f , SampleRate );

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

        AmplifyPCM24 ( PCMData , soundGain );  // 24비트 데이터 증폭
        ApplyHighPassFilter24 ( PCMData , 100.0f , SampleRate );
        ApplyLowPassFilter24 ( PCMData , 5000.0f , SampleRate );

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

        AmplifyPCM32 ( PCMData , soundGain );  // 32비트 데이터 증폭
        ApplyHighPassFilter32 ( PCMData , 100.0f , SampleRate );
        ApplyLowPassFilter32 ( PCMData , 5000.0f , SampleRate );

        SoundWave->QueueAudio ( reinterpret_cast<const uint8*>( PCMData.GetData ( ) ) , PCMData.Num ( ) * sizeof ( float ) );
    }
    SoundWave->bLooping = false;
    SoundWave->bProcedural = true;
    SoundWave->SoundGroup = SOUNDGROUP_Default;
    SoundWave->bStreaming = false;

    return SoundWave;
}

void UAudience_KMK::AddSongList ( const FWavFileInfo& SongInfo )
{
    auto* songUnit = CreateWidget<UHSW_SongUnit> ( this , SongUnitFact );
    songUnit->SongInfo = SongInfo;
    songUnit->SetSongTitle();
    SB_ChatLog->AddChild ( SB_SongList_1 );
    SB_ChatLog->AddChild ( SB_SongList_2 );
}

#pragma endregion
#pragma region Modeling

void UAudience_KMK::PressButtModel ( )
{

}
#pragma endregion
#pragma region Filter
void ApplyHighPassFilter16 ( TArray<int16>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = static_cast<float>( PCMData[i] );
        float filteredSample = alpha * ( previousSample + currentSample - previousSample );
        PCMData[i] = static_cast<int16>( filteredSample );
        previousSample = currentSample;
    }
}

void ApplyLowPassFilter16 ( TArray<int16>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = static_cast<float>( PCMData[i] );
        previousSample = previousSample + alpha * ( currentSample - previousSample );
        PCMData[i] = static_cast<int16>( previousSample );
    }
}

void ApplyHighPassFilter24 ( TArray<int32>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = static_cast<float>( PCMData[i] >> 8 );  // 24비트를 32비트로 변환 시 상위 바이트 제거
        float filteredSample = alpha * ( previousSample + currentSample - previousSample );
        PCMData[i] = static_cast<int32>( filteredSample ) << 8;  // 다시 24비트로 변환
        previousSample = currentSample;
    }
}

void ApplyLowPassFilter24 ( TArray<int32>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = static_cast<float>( PCMData[i] >> 8 );  // 24비트를 32비트로 변환
        previousSample = previousSample + alpha * ( currentSample - previousSample );
        PCMData[i] = static_cast<int32>( previousSample ) << 8;  // 다시 24비트로 변환
    }
}

void ApplyHighPassFilter32 ( TArray<float>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = PCMData[i];
        float filteredSample = alpha * ( previousSample + currentSample - previousSample );
        PCMData[i] = filteredSample;
        previousSample = currentSample;
    }
}

void ApplyLowPassFilter32 ( TArray<float>& PCMData , float CutoffFrequency , int32 SampleRate )
{
    const float RC = 1.0f / ( CutoffFrequency * 2.0f * PI );
    const float dt = 1.0f / static_cast<float>( SampleRate );
    const float alpha = dt / ( RC + dt );

    float previousSample = 0.0f;

    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        float currentSample = PCMData[i];
        previousSample = previousSample + alpha * ( currentSample - previousSample );
        PCMData[i] = previousSample;
    }
}
void AmplifyPCM16 ( TArray<int16>& PCMData , float Gain )
{
    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        // 샘플 증폭
        int32 AmplifiedSample = static_cast<int32>( PCMData[i] * Gain );

        // 클리핑 방지
        PCMData[i] = FMath::Clamp ( AmplifiedSample , -32768 , 32767 );
    }
}
void AmplifyPCM24 ( TArray<int32>& PCMData , float Gain )
{
    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        // 24비트 샘플 증폭
        int64 AmplifiedSample = static_cast<int64>( ( PCMData[i] >> 8 ) * Gain );  // 24비트를 32비트로 확장한 후 증폭

        // 클리핑 방지 (24비트에서 최대 최소 값은 23비트로 결정됨)
        AmplifiedSample = FMath::Clamp ( AmplifiedSample , -8388608 , 8388607 );  // 24비트 한계

        PCMData[i] = static_cast<int32>( AmplifiedSample ) << 8;  // 다시 24비트로 변환
    }
}
void AmplifyPCM32 ( TArray<float>& PCMData , float Gain )
{
    for (int32 i = 0; i < PCMData.Num ( ); ++i)
    {
        // 샘플 증폭
        PCMData[i] *= Gain;

        // 클리핑 방지
        PCMData[i] = FMath::Clamp ( PCMData[i] , -1.0f , 1.0f );
    }
}

#pragma endregion