// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSW_ThrowingObject.generated.h"

UCLASS()
class VIRTUALIDOL_API AHSW_ThrowingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSW_ThrowingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite )
	bool bCanOverlap;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Imoji )
	TArray<class UStaticMesh *> MeshArray;

	UFUNCTION( )
	void ChangeMesh( int32 meshIndex );

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float DisappearTime = 3.0f;

	UFUNCTION( )
	void DestroyObject( );

	FTimerHandle TimerHandleDestroy;

	UFUNCTION()
	void OnMyObjectBeginverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
