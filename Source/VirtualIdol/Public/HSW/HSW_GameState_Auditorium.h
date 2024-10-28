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
	// virtual void BeginPlay ( ) override;

	UPROPERTY(EditDefaultsOnly )
	TArray<class APlayerState *> PlayerList;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = "GameState" )
	bool bFever;

// 	UFUNCTION(Server, Reliable)
// 	void ServerSetPlayerName(const FString& newName);
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPCChat(const FString& chat );

	UFUNCTION(Server, Reliable)
    void ServerRPCChat(const FString& Chat);

	UFUNCTION(Server, Reliable)
    void ServerRPC_ShowCountDown ();

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_ShowCountDown( );

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_FeverGauge( float feverValue );

	FTimerHandle startCountDownHandle;
	UPROPERTY(EditAnywhere )
	float durationTime = 6;
	float remainTime = 0.f;
	UPROPERTY()
	bool bTime = false;
};
