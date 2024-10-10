// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HSW_ImojiConponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALIDOL_API UHSW_ImojiConponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSW_ImojiConponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	class UBillboardComponent* ImojiBilboard;

	UFUNCTION ()
	void Imoji01( );
	UFUNCTION ( )
	void Imoji02 ( );
	UFUNCTION ( )
	void Imoji03 ( );
	UFUNCTION ( )
	void Imoji04 ( );
};
