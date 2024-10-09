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
void UAudience_KMK::NativeConstruct ( )
{
    Super::NativeConstruct( );
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
}

#pragma region PopUp

void UAudience_KMK::PressVipButt ( )
{
   
    VisiblePanel ( ESlateVisibility::Visible );
}

void UAudience_KMK::PressYesButt ( )
{
    gi->playerMeshNum = 1;
    VisiblePanel ( ESlateVisibility::Hidden );
}

void UAudience_KMK::PressNoButt ( )
{
    gi->playerMeshNum = 0;
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


