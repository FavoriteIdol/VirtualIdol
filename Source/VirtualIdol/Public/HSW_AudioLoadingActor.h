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

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

public:
	UPROPERTY( )
	class USceneComponent* SceneComponent;

	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;

	UPROPERTY( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	class UVirtual_KMK* VirtualCharacter;

	UPROPERTY( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	class UMediaPlayer* MediaPlayer;

	UPROPERTY( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	class UMediaSoundComponent* MediaSoundComp;

	UPROPERTY( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	class UFileMediaSource* FileMediaSource;

	UFUNCTION( Server , Reliable )
	void ServerRPC_PlayWaveFile( );

	UFUNCTION( NetMulticast, Reliable )
	void MultiRPC_PlayWavFile( );

	UFUNCTION( )
	void OnPlayEnded (  );

	UFUNCTION( )
	void FindVirtualCharacter ( );

};
