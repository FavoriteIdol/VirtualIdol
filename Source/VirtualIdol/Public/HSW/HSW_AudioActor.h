// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSW_AudioActor.generated.h"

UCLASS()
class VIRTUALIDOL_API AHSW_AudioActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSW_AudioActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Audio" )
	class UAudioComponent* AmbientSoundComponent01;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    class USoundCue* SoundCue01;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Audio" )
	class UAudioComponent* AmbientSoundComponent02;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    class USoundCue* SoundCue02;
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySound0(float SoundVolume );

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySound1( float SoundVolume );

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySound2( float SoundVolume );
};
