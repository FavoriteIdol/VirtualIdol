// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Virtual_KMK.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALIDOL_API UVirtual_KMK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVirtual_KMK();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UAudience_KMK* virtualWidget;

	UFUNCTION( )
	FString GetTimeDifference ( const FString& SetTime );

	bool bVis = false;
	FString s;
	UPROPERTY( EditAnywhere)
	FString setConcertTime = "22:10:00";

	UFUNCTION( )
	void SetVirtualChat(const FString& text );
	UFUNCTION(BlueprintCallable )
	void SetVirtualVisible(bool bVisit = false );
	UPROPERTY( )
	class USkeletalMeshComponent* meshComp;

	UFUNCTION( )
	void ShowCoundDownPanel( );

	UFUNCTION(BlueprintCallable )
	void CallGMShowServer( );

	FTimerHandle startCountDownHandle;
	UPROPERTY(EditAnywhere )
	float durationTime = 6;
	float remainTime = 0.f;
	UPROPERTY()
	bool bTime = false;

	UFUNCTION( )
	void StartCountDown( );

	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	TArray< TSubclassOf<class AActor>> appearFact;
	UPROPERTY( )
	float currentGauge = 0;

	UFUNCTION( )
	void PlayMusic(USoundBase* wavFile);

	UFUNCTION( )
	void SetInterviewee( bool bInterview, APlayerState* interviewee, FTransform preLoc );

	FTransform StageLocation;
	FTransform PreLocation;

	void StartVoiceChat ( );
	void CancleVoiceChat ( );

	UPROPERTY( )
	class APlayerController* pc;
};
