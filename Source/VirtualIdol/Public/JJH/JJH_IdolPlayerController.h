// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VrmVMCObject.h"
#include "JJH_IdolPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AJJH_IdolPlayerController : public APlayerController
{

	GENERATED_BODY()

public:

	bool bIsReceived = false;

	FVMCData MyData;

	UFUNCTION(Server, Unreliable )
	void ServerGetVMCData(const FVMCData& Data);

	UFUNCTION(NetMulticast, Unreliable)
    void MulticastSendVMCData(const FVMCData& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	bool bUseRemoteCenterPos = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	float ModelRelativeScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	bool bIgnoreLocalRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	FString ServerAddress = "0.0.0.0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	int Port = 39539;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton, meta = (PinShownByDefault))
	bool bApplyPerfectSync = true;

	bool bCreateServer = false;

	TArray<FTransform> RefSkeletonTransform_global;
};
