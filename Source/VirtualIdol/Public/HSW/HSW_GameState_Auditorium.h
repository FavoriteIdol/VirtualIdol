// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HSW_GameState_Auditorium.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AHSW_GameState_Auditorium : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay ( ) override;

	UPROPERTY(EditDefaultsOnly )
	TArray<class APlayerState *> PlayerList;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = "GameState" )
	bool bFever;

// 	UFUNCTION(Server, Reliable)
// 	void ServerSetPlayerName(const FString& newName);
};
