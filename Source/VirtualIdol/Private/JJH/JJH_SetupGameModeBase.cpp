// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_SetupGameModeBase.h"
#include "HSW/HSW_GameState_Auditorium.h"

void AJJH_SetupGameModeBase::ServerRPCChat_Implementation (const FString& nickName, const FString& Chat )
{
    AHSW_GameState_Auditorium* gs = GetGameState<AHSW_GameState_Auditorium>();
    if (gs)
    {
        gs->MultiRPCChat(nickName, Chat);
    }
}
