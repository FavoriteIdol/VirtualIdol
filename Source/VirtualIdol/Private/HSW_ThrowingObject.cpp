// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_ThrowingObject.h"

// Sets default values
AHSW_ThrowingObject::AHSW_ThrowingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHSW_ThrowingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSW_ThrowingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

