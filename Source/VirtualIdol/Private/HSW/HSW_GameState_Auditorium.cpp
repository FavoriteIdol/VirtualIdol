// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_GameState_Auditorium.h"

void AHSW_GameState_Auditorium::BeginPlay ( )
{

}

void AHSW_GameState_Auditorium::Tick ( float DeltaSeconds )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "GameState Tick" ) );
	if (bFever30)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Fever 30%% !!!!!!! " ) );
	}
	else if (bFever65)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Fever 65%% !!!!!!! " ) );
	}
	else if (bFever100)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Fever 100%% !!!!!!! " ) );
	}
}
