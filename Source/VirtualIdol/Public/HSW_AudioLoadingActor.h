// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK/HttpActor_KMK.h"
#include "HSW_AudioLoadingActor.generated.h"

UCLASS()
class VIRTUALIDOL_API AHSW_AudioLoadingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSW_AudioLoadingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY( )
	class USceneComponent* SceneComponent;

	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;

	UPROPERTY( )
	class UVirtual_KMK* VirtualCharacter;

	UPROPERTY( )
	class UMediaPlayer* MediaPlayer;

	UPROPERTY( )
	class UMediaSoundComponent* MediaSoundComp;

	UPROPERTY( )
	class UFileMediaSource* FileMediaSource;

	UFUNCTION( )
	void PlayWavFile( );

	UFUNCTION( )
	void OnPlayEnded (  );

	UFUNCTION( )
	void FindVirtualCharacter ( );
};
