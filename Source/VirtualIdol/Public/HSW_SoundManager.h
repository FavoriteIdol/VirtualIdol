// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSW_SoundManager.generated.h"

UCLASS()
class VIRTUALIDOL_API AHSW_SoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSW_SoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

	// 멤버 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<class USoundBase* > WavArray;
	UPROPERTY(Replicated )
	class USoundBase* CurrentSongWav;
	UPROPERTY(Replicated )
	class UAudioComponent* CurrentSongComp;

	// 기능
	UFUNCTION(Server, Reliable )
	void ServerRPC_SetMusic( USoundBase* wavToSet );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPC_SetMusic( USoundBase* wavToSet );


	UFUNCTION(Server, Reliable )
	void ServerRPC_PlayMusic( UAudioComponent* MusicToPlay );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPC_PlayMusic( UAudioComponent* MusicToPlay );

	void PlaySound( USoundBase* MusicToPlay );
};
