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

	float CurrentGauge;
// 	UFUNCTION(Server, Reliable)
// 	void ServerSetPlayerName(const FString& newName);
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPCChat(const FString& nickName,const FString& chat );

	UFUNCTION(Server, Reliable)
    void ServerRPCChat(const FString& nickName,const FString& Chat);

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPCPlaySound(USoundBase* wavFile);

	UFUNCTION(Server, Reliable)
	void ServerRPCPlaySound( USoundBase* wavFile);

	UFUNCTION(Server, Reliable)
    void ServerRPC_ShowCountDown ();

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_ShowCountDown( );

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_FeverGauge( float feverValue );

	UFUNCTION(Server, Reliable)
	void ServerRPC_StopSound ();

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_StopSound( );

	FTimerHandle startCountDownHandle;
	UPROPERTY(EditAnywhere )
	float durationTime = 6;
	float remainTime = 0.f;
	UPROPERTY()
	bool bTime = false;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPC_SetInterviewee ( );

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_SetInterviewee( bool bInterview , APlayerState* interviewee , FTransform preLoc );

	bool bIsInterviewing;
	int32 IntervieweeIndex;
	class APlayerState* IntervieweePlayerState;
	FTransform PreLocation;
};
