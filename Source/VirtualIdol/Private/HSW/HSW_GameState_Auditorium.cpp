// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_GameState_Auditorium.h"
#include "GameFramework/PlayerState.h"
#include "KMK/Audience_KMK.h"
#include "GameFramework/PlayerController.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "KMK/Virtual_KMK.h"
#include "HSW/HSW_AuditoriumGameMode.h"

//void AHSW_GameState_Auditorium::BeginPlay ( )
//{
//	// PlayerList = PlayerArray;
//}

void AHSW_GameState_Auditorium::MultiRPCChat_Implementation ( const FString& chat )
{
 // 각 클라이언트의 위젯에 메시지 전달
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn();
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter>(Pawn))
        {
            if (Character->IsLocallyControlled())
            {
                UAudience_KMK* MyWidget = Cast<UAudience_KMK>(Character->audienceWidget);
                if (MyWidget)
                {
                    MyWidget->CreateChatWidget(chat);
                }
            }
        }
         else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK>())
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK>();
            if (Vir && Pawn->IsLocallyControlled())
            {
                Vir->SetVirtualChat(chat);
            }
        }
    }
}

void AHSW_GameState_Auditorium::ServerRPCChat_Implementation ( const FString& Chat )
{
     AHSW_AuditoriumGameMode* gm = GetWorld()->GetAuthGameMode<AHSW_AuditoriumGameMode>();
    if (gm)
    {
        gm->BroadcastChatMessage(Chat);  // GameMode에 메시지 전달
    }
}
