// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JJH_SelectManager.generated.h"

UCLASS()
class VIRTUALIDOL_API AJJH_SelectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJJH_SelectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//태양 위치 바꾸기
    UFUNCTION(BlueprintCallable, Category = "Time")
    void UpdateSunNightPosition(bool isNight);

	class ADirectionalLight* Sun;
	
	UPROPERTY(EditAnywhere)
	FRotator AfternoonSunRotation;
	
	UPROPERTY (EditAnywhere )
	FRotator NightSunRotation;


	//레벨 바꾸기
	UFUNCTION ()
	void ChangeMap(int32 index);

	//이펙트 바꾸기
	UFUNCTION( )
	void ChangeEffect(int32 index);

	UPROPERTY(EditAnywhere)
	UParticleSystem* Effect1;	
	UPROPERTY ( EditAnywhere )
	UParticleSystem* Effect2;
};
