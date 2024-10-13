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

void UAudience_KMK::NativeConstruct ( )
{
    Super::NativeConstruct( );
    SetUpButtonInfo();
    gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
    if (Butt_Vip && Butt_Yes && Butt_No && PopUpPanel)
    {
        VisiblePanel(ESlateVisibility::Hidden);
        Butt_Vip->OnClicked.AddDynamic ( this , &UAudience_KMK::PressVipButt );
        Butt_Yes->OnClicked.AddDynamic ( this , &UAudience_KMK::PressYesButt );
        Butt_No->OnClicked.AddDynamic ( this , &UAudience_KMK::PressNoButt );
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
    if (Butt_Exit)
    {
        Butt_Exit->OnClicked.AddDynamic ( this , &UAudience_KMK::PressExitButt);
    }
    if (ExitPanel && Butt_Cancel && Butt_Out)
    {
        ExitPanel->SetVisibility(ESlateVisibility::Hidden);
        Butt_Cancel->OnClicked.AddDynamic ( this , &UAudience_KMK::PressCancelButt);
        Butt_Out->OnClicked.AddDynamic ( this , &UAudience_KMK::PressOutButt);
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
    if (Butt_StartConcert)
    {
        Butt_StartConcert->OnClicked.AddDynamic ( this , &UAudience_KMK::PressStartConcertButt );
    }
    if (CountDownPanel)
    {
        CountDownPanel->SetVisibility ( ESlateVisibility::Hidden );
    }
#pragma endregion

}

#pragma region PopUp

void UAudience_KMK::SetUpButtonInfo ( )
{
    for (int i = 0; i < buttonName.Num ( ); i++)
    {
        FName bName = FName(*FString::Printf(TEXT("Butt_") ) + buttonName[i]);
        FName bImgName = FName(*FString::Printf(TEXT("Image_Back_")  )+ buttonName[i] );
        FName iName = FName(*FString::Printf(TEXT("Image_") )+ buttonName[i] );
        FName tName = FName(*FString::Printf(TEXT("Text_")  )+ buttonName[i] );

        UButton* butt = Cast<UButton>(GetWidgetFromName(bName));
        UImage* bImg = Cast<UImage>(GetWidgetFromName(bImgName));
        UImage* img = Cast<UImage>(GetWidgetFromName(iName));
        UTextBlock* text = Cast<UTextBlock>(GetWidgetFromName(tName));

        if (butt && img && text)
        {
            FButtonInfo ButtonInfo;
            ButtonInfo.button = butt;
            ButtonInfo.image = img;
            ButtonInfo.backImage = bImg;
            ButtonInfo.text = text;
            ButtonsInfoArray.Add(ButtonInfo);
        }
    }
}

void UAudience_KMK::PressHiddenButt ( )
{
    OnOffFunction(Text_Hidden, 0, true);
}

void UAudience_KMK::PressModeButt ( )
{
    OnOffFunction(Text_Mode, 1 );
}

void UAudience_KMK::StartVoiceChat ( )
{
    UE_LOG(LogTemp, Warning, TEXT("StartTalk" ) );
    pc->GetController<APlayerController> ( )->StartTalking ( );
}

void UAudience_KMK::StopVoiceChat ( )
{
    UE_LOG ( LogTemp , Warning , TEXT ( "StopTalk" ) );
    pc->GetController<APlayerController> ( )->StopTalking ( );
}

void UAudience_KMK::PressMikeButt ( )
{
    OnOffFunction ( Text_Mike , 2 );
    if (bMikeOn)
    {
        StartVoiceChat ( );
        bMikeOn = false;
    }
    else
    {
        StopVoiceChat ( );
        bMikeOn = true;
    }
}

void UAudience_KMK::OnOffFunction (class UTextBlock* textBlocks, int32 num , bool bAllVisib )
{
    if (textBlocks->GetText().ToString() == currentText[num])
    {
        if(bAllVisib)OnOffInfo(FLinearColor::Yellow, ESlateVisibility::Hidden, num, changeText );
        else ChangeTextAndImage ( FLinearColor::Yellow , num , changeText );
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
    ButtonsInfoArray[num].text->SetText ( FText::FromString ( textArray[num] ) );
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
        ChangeTextAndImage (FLinearColor::Yellow, 3, changeText );
    }
    else
    {
        bChatOn = false;
        ChatGridPanel->SetVisibility(ESlateVisibility::Hidden);
        ChangeTextAndImage (FLinearColor::White, 3, currentText);
    }
}

void UAudience_KMK::PressEmotionButt ( )
{
    if (!bEmotion)
    {
        bEmotion = true;
        ChangeTextAndImage ( FLinearColor::Yellow , 4 , changeText );
    }
    else
    {
        bEmotion = false;
        ChangeTextAndImage ( FLinearColor::White , 4 , currentText );
    }
}


void UAudience_KMK::PressVipButt ( )
{
    VisiblePanel ( ESlateVisibility::Visible );
    if (Text_Vip->GetText().ToString() == currentText[5])
    {
        ChangeTextAndImage ( FLinearColor::White , 5 , changeText );
    }
}


void UAudience_KMK::OnOffInfo ( FLinearColor color,  ESlateVisibility bVisib, int32 num, TArray<FString> textArray  )
{
    for (int i = 0; i < ButtonsInfoArray.Num(); i++)
    {
        if (i == num)
        {
            ButtonsInfoArray[i].image->SetBrushTintColor(color);
            ButtonsInfoArray[i].backImage->SetVisibility(ESlateVisibility::Visible);
            ButtonsInfoArray[i].text->SetText(FText::FromString(textArray[i]));
        }
        else
        {
            if(bMyVip && i == ButtonsInfoArray.Num() - 1) continue;
            ButtonsInfoArray[i].button->SetVisibility ( bVisib );
            ButtonsInfoArray[i].backImage->SetVisibility ( bVisib );
            ButtonsInfoArray[i].image->SetVisibility ( bVisib );
            ButtonsInfoArray[i].text->SetVisibility ( bVisib );
        }
    }
}
void UAudience_KMK::VipAuthority ( )
{
    bMyVip = true;
    ButtonsInfoArray[5].button->SetVisibility ( ESlateVisibility::Hidden );
    ButtonsInfoArray[5].backImage->SetVisibility ( ESlateVisibility::Hidden );
    ButtonsInfoArray[5].image->SetVisibility ( ESlateVisibility::Hidden );
    ButtonsInfoArray[5].text->SetVisibility ( ESlateVisibility::Hidden );
}

void UAudience_KMK::PressStartConcertButt ( )
{
    gi->playerMeshNum = 2;
    Butt_StartConcert->SetVisibility(ESlateVisibility::Hidden);
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
    auto* player = Cast<ATP_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (player)
    {
        auto* server = player->GetComponentByClass<UAudienceServerComponent_KMK> ( );
        if (server)
        {
            if (!EditText_Chat->GetText ( ).IsEmpty ( ))
            {
                server->ServerRPCChat( EditText_Chat->GetText ( ).ToString ( ) );
                EditText_Chat->SetText ( FText::GetEmpty ( ) );
            }
        }
    }
}

void UAudience_KMK::CreateChatWidget ( const FString& chatText )
{
    auto* chat = CreateWidget<USendChat_KMK> ( this, chatWidgetFact );
    chat->Text_Chat->SetText ( FText::FromString ( chatText ) );
    SB_ChatLog->AddChild ( chat );
    SB_ChatLog->ScrollToEnd();
}
#pragma endregion


void UAudience_KMK::PressExitButt ( )
{
    ExitPanel->SetVisibility(ESlateVisibility::Visible);
}

void UAudience_KMK::PressOutButt ( )
{
    gi->ExitRoom ( );
}

void UAudience_KMK::PressCancelButt ( )
{
    ExitPanel->SetVisibility(ESlateVisibility::Hidden);
}

#pragma region Cash
void UAudience_KMK::PressObjectButt ( )
{
    // 오브젝트 생성
    UE_LOG(LogTemp, Warning, TEXT("111111" ));
}
void UAudience_KMK::PressObject1Butt ( )
{
    UE_LOG ( LogTemp , Warning , TEXT ( "22222" ) );
}
void UAudience_KMK::PressObject2Butt ( )
{
    UE_LOG ( LogTemp , Warning , TEXT ( "33333" ) );
}

void UAudience_KMK::CountDownText ( int32 time )
{
    Text_CoutDown->SetText(FText::AsNumber(time));
}

void UAudience_KMK::CountDownPanelVisible ( ESlateVisibility visiblePanel )
{
    if(CountDownPanel)CountDownPanel->SetVisibility(visiblePanel);
}

#pragma endregion


