// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VrmVMCObject.h"
#include "JJH_IdolGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AJJH_IdolGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	class UVrmVMCObject* VrmVMCObject;

	FVMCData MyData;

	UFUNCTION(Server, Unreliable )
	void ServerGetVMCData(const FVMCData& Data);
	UFUNCTION(NetMulticast, Reliable)
    void MulticastSendVMCData( const FVMCData& Data );

	void FindVrmVMCObject( );

	virtual void BeginPlay ( ) override;
};
