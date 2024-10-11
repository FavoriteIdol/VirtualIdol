// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY ( )
	class UTexture2D* Imoji01Texture;
	UPROPERTY ( )
	class UTexture2D* Imoji02Texture;
	UPROPERTY ( )
	class UTexture2D* Imoji03Texture;
	UPROPERTY ( )
	class UTexture2D* Imoji04Texture;

	UFUNCTION ()
	void Imoji01( );
	UFUNCTION ( )
	void Imoji02 ( );
	UFUNCTION ( )
	void Imoji03 ( );
	UFUNCTION ( )
	void Imoji04 ( );

	FTimerHandle TimerHandle;

	void AppearImoji(UTexture2D* imojiTexture );

	void DisappearImoji ( );


protected:
	// Fade In/ Fade Out
	UPROPERTY ()
	UTimelineComponent* MyTimeline;

	UPROPERTY ()
	UCurveFloat* FloatCurve;

	UFUNCTION( )
	void TimelineCallback(float val );

	UFUNCTION( )
	void TimelineFinishedCallback();

	void PlayTimeline( );

	UPROPERTY( )
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;


};
