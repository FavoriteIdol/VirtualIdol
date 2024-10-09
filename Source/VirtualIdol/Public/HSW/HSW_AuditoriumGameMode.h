// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HSW_AuditoriumGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AHSW_AuditoriumGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHSW_AuditoriumGameMode();

	virtual void Tick ( float DeltaTime ) override;

protected:
	virtual void BeginPlay () override;

public:
	UPROPERTY (EditDefaultsOnly)
	class AHSW_ThirdPersonCharacter* Player;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="GameState" )
	bool bFevered;

};
