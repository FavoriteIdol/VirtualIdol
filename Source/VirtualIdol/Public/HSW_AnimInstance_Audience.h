// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HSW_AnimInstance_Audience.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UHSW_AnimInstance_Audience : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation ( ) override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float Horizental;	//Direction

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float Vertical;		//Speed

	UPROPERTY()
	class AHSW_ThirdPersonCharacter* Me;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool IsInAir;


};
