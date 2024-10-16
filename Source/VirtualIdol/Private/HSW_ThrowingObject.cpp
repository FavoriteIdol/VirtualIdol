// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_ThrowingObject.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

// Sets default values
AHSW_ThrowingObject::AHSW_ThrowingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp" ) );
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeScale3D(FVector(0.25,0.25,0.25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh ( TEXT ( "'/Engine/VREditor/BasicMeshes/SM_Ball_01'" ) );
	if (LoadedMesh.Succeeded ( ))
	{
		MeshComp->SetStaticMesh( LoadedMesh.Object );
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> LoadedMaterial ( TEXT ( "'/Engine/VREditor/BasicMeshes/MI_Ball_01'" ) );
	if (LoadedMaterial.Succeeded ( ))
	{
		MeshComp->SetMaterial(0, LoadedMaterial.Object);
	}

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> LoadedPhysicalMaterial ( TEXT ( "'/Game/Project/Personal/HSW/Blueprints/PM_Ball'" ) );

	if (LoadedPhysicalMaterial.Succeeded ( ))
	{
		MeshComp->SetPhysMaterialOverride( LoadedPhysicalMaterial.Object);
	}


	MeshComp->BodyInstance.bNotifyRigidBodyCollision = true;
	MeshComp->CanCharacterStepUpOn = ECB_No;
	MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic" ));

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