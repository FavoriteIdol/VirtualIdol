// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_AuditoriumGameMode.h"

AHSW_AuditoriumGameMode::AHSW_AuditoriumGameMode ( )
{
	// 디폴트 폰 클래스 설정
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass ( TEXT ( "/Game/Project/Personal/HSW/Blueprints/HSW_BP_ThirdPerson" ) );
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
