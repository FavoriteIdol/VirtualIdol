// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_PlayerController.h"
#include "HSW/HSW_AuditoriumGameMode.h"

void AHSW_PlayerController::BeginPlay ( )
{
	if (HasAuthority ( ))
	{
		gm = Cast<AHSW_AuditoriumGameMode>(GetWorld()->GetAuthGameMode() );
	}	
}

void AHSW_PlayerController::ServerRPCResapwnPlayer_Implementation ( )
{
	auto* player = GetPawn();
	UnPossess();
	player->Destroy( );

	gm->RestartPlayer(this);
}
