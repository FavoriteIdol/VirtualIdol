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
	// 피버게이지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float CurrentGauge = 0.0f;

	UPROPERTY (EditDefaultsOnly)
	class AHSW_ThirdPersonCharacter* Player;

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

	UPROPERTY( Replicated )
	bool bFever;
	
	UPROPERTY ( Replicated, EditDefaultsOnly , Category = Fever )
	class UParticleSystem* FeverEffect_Particle;

	UPROPERTY ( Replicated, EditDefaultsOnly , Category = Fever )
	class UNiagaraSystem* FeverEffect_Niagara;

	UPROPERTY ( Replicated , EditDefaultsOnly , Category = Fever )
	FTransform FeverEffectLocation;

	UFUNCTION(NetMulticast, Reliable )
	void Multicast_FeverEffect( );

	UFUNCTION( )
	void BroadcastChatMessage ( const FString& Chat );

	UFUNCTION( )
	void BroadcastCountDown( );
};
