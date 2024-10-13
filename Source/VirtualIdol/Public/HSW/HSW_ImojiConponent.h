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

	UPROPERTY( )
	class AHSW_ThirdPersonCharacter* Me;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite, Category = Imoji )
	class UStaticMeshComponent* ImojiMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Imoji )
	class UBillboardComponent* ImojiBillboard;

	UPROPERTY ( EditDefaultsOnly, BlueprintReadWrite )
	class UTexture2D* Imoji01Texture;
	UPROPERTY ( EditDefaultsOnly, BlueprintReadWrite )
	class UTexture2D* Imoji02Texture;
	UPROPERTY ( EditDefaultsOnly, BlueprintReadWrite )
	class UTexture2D* Imoji03Texture;
	UPROPERTY ( EditDefaultsOnly, BlueprintReadWrite )
	class UTexture2D* Imoji04Texture;

	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite )
	class UMaterial* OpacityMaterial;

	UFUNCTION (BlueprintCallable)
	void Imoji01( );
	UFUNCTION ( BlueprintCallable )
	void Imoji02 ( );
	UFUNCTION ( BlueprintCallable )
	void Imoji03 ( );
	UFUNCTION ( BlueprintCallable )
	void Imoji04 ( );

	FTimerHandle TimerHandle;

	void AppearImoji(UTexture2D* imojiTexture );

	void DisappearImoji ( );



// 
// protected:
// 	// Fade In/ Fade Out
// 	UPROPERTY ()
// 	UTimelineComponent* FadeInTimeline;
// 
// 	UPROPERTY ( )
// 	UTimelineComponent* FadeOutTimeline;
// 
// 	UPROPERTY ()
// 	UCurveFloat* FadeInCurve;
// 	UPROPERTY ( )
// 	UCurveFloat* FadeOutCurve;
// 
// 	UFUNCTION( )
// 	void TimelineCallback(float val );
// 
// 	UFUNCTION( )
// 	void TimelineFinishedCallback();
// 
// 	void PlayTimeline( );
// 
// 	UPROPERTY( )
// 	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;


};
