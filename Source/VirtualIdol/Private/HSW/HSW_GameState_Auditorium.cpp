// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_GameState_Auditorium.h"
#include "GameFramework/PlayerState.h"
#include "KMK/Audience_KMK.h"
#include "GameFramework/PlayerController.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "KMK/Virtual_KMK.h"
#include "HSW/HSW_AuditoriumGameMode.h"
#include "KMK/AudienceServerComponent_KMK.h"
#include "HSW/HSW_FeverGaugeWidget.h"

//void AHSW_GameState_Auditorium::BeginPlay ( )
//{
//	// PlayerList = PlayerArray;
//}

void AHSW_GameState_Auditorium::MultiRPCChat_Implementation (const FString& nickName, const FString& chat )
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
                    MyWidget->CreateChatWidget(nickName, chat);
                }
            }
        }
         else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK>())
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK>();
            if (Vir && Pawn->IsLocallyControlled())
            {
                Vir->SetVirtualChat(nickName, chat);
            }
        }
    }
}

void AHSW_GameState_Auditorium::ServerRPCChat_Implementation (const FString& nickName, const FString& Chat )
{
     AHSW_AuditoriumGameMode* gm = GetWorld()->GetAuthGameMode<AHSW_AuditoriumGameMode>();
    if (gm)
    {
        gm->BroadcastChatMessage(nickName, Chat);  // GameMode에 메시지 전달
    }
}

void AHSW_GameState_Auditorium::MultiRPCPlaySound_Implementation ( USoundBase* wavFile )
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn ( );
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( Pawn ))
        {
            if (Character->IsLocallyControlled ( ))
            {
                Character->PlayMusic( wavFile );
            }
        }
        else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK> ( ))
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK> ( );
            if (Vir && Pawn->IsLocallyControlled ( ))
            {
                Vir->PlayMusic ( wavFile );
            }
        }
    }
}

void AHSW_GameState_Auditorium::ServerRPCPlaySound_Implementation ( USoundBase* wavFile )
{
    AHSW_AuditoriumGameMode* gm = GetWorld ( )->GetAuthGameMode<AHSW_AuditoriumGameMode> ( );
    if (gm)
    {
        gm->BroadcastPlayMusic ( wavFile );
    }
}

void AHSW_GameState_Auditorium::ServerRPC_ShowCountDown_Implementation ( )
{
    AHSW_AuditoriumGameMode* gm = GetWorld ( )->GetAuthGameMode<AHSW_AuditoriumGameMode> ( );
    if (gm)
    {
        gm->BroadcastCountDown();
    }
}

void AHSW_GameState_Auditorium::MultiRPC_ShowCountDown_Implementation ( )
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn ( );
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( Pawn ))
        {
            if (Character && Character->IsLocallyControlled ( ))
            {
                Character->FindComponentByClass<UAudienceServerComponent_KMK> ( )->StartCountDown ( );
                UAudience_KMK* MyWidget = Cast<UAudience_KMK> ( Character->audienceWidget );
                if (MyWidget)
                {
                    MyWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
                    
                }
            }
        }
        else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK> ( ))
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK> ( );
            if (Vir && Pawn->IsLocallyControlled ( ))
            {
                Vir->ShowCoundDownPanel();
            }
        }
    }
}

void AHSW_GameState_Auditorium::MultiRPC_FeverGauge_Implementation ( float feverValue )
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn ( );
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( Pawn ))
        {
            if (Character && Character->IsLocallyControlled ( ))
            {
                Character->CurrentGauge = feverValue;
                //Character->audienceWidget->FeverGauge->SetFeverGauge ( Character->CurrentGauge );
            }
        }
        else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK> ( ))
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK> ( );
            if (Vir && Pawn->IsLocallyControlled ( ))
            {
                Vir->currentGauge = feverValue;
                //Vir->virtualWidget->FeverGauge->SetFeverGauge ( Vir->currentGauge );
            }
        }
    }
}

void AHSW_GameState_Auditorium::ServerRPC_StopSound_Implementation ( )
{
    AHSW_AuditoriumGameMode* gm = GetWorld ( )->GetAuthGameMode<AHSW_AuditoriumGameMode> ( );
    if (gm)
    {
        //gm->BroadcastPlayMusic ( );
    }
}

void AHSW_GameState_Auditorium::MultiRPC_StopSound_Implementation ( )
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn ( );
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( Pawn ))
        {
            if (Character->IsLocallyControlled ( ))
            {
                //Character->PlayMusic ( wavFile );
            }
        }
        else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK> ( ))
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK> ( );
            if (Vir && Pawn->IsLocallyControlled ( ))
            {
                //Vir->PlayMusic ( wavFile );
            }
        }
    }
}

void AHSW_GameState_Auditorium::ServerRPC_SetInterviewee_Implementation ( )
{
    AHSW_AuditoriumGameMode* gm = GetWorld ( )->GetAuthGameMode<AHSW_AuditoriumGameMode> ( );
    if (gm)
    {
        // 인터뷰 끝나는 조건 나중에 바꾸기
        bIsInterviewing = !bIsInterviewing;

        if (PlayerArray.Num ( ) > 0 )
        {
            if (bIsInterviewing)
            {

            IntervieweeIndex = FMath::RandRange ( 1 , PlayerArray.Num ( ) - 1 );
            IntervieweePlayerState = PlayerArray[IntervieweeIndex];

            PreLocation = IntervieweePlayerState->GetPawn ( )->GetActorTransform ( );
            }
        }
        else
        {
            UE_LOG ( LogTemp , Warning , TEXT ( "플레이어가 없습니다." ) );
        }
        gm->BroadcastSetInterviewee ( bIsInterviewing, IntervieweePlayerState, PreLocation );

    }
}

void AHSW_GameState_Auditorium::MultiRPC_SetInterviewee_Implementation ( bool bInterview , APlayerState* interviewee , FTransform preLoc )
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        APawn* Pawn = PlayerState->GetPawn ( );
        if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( Pawn ))
        {
            if (Character->IsLocallyControlled ( ))
            {
                Character->SetInterviewee ( bInterview, interviewee, preLoc );
            }
        }
        else if (Pawn && Pawn->FindComponentByClass<UVirtual_KMK> ( ))
        {
            UVirtual_KMK* Vir = Pawn->FindComponentByClass<UVirtual_KMK> ( );
            if (Vir && Pawn->IsLocallyControlled ( ))
            {
               Vir->SetInterviewee ( bInterview, interviewee , preLoc );
            }
        }
    }
}

