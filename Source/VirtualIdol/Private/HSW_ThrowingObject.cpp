// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_ThrowingObject.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "HSW/HSW_ThirdPersonCharacter.h"


// Sets default values
AHSW_ThrowingObject::AHSW_ThrowingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent( SphereComp );
	SphereComp->SetGenerateOverlapEvents(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp" ) );
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeScale3D(FVector(0.25,0.25,0.25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadedMesh ( TEXT ( "'/Game/Project/Personal/JSW/01_FBX_JSW/jswHeart'" ) );
	if (LoadedMesh.Succeeded ( ))
	{
		MeshComp->SetStaticMesh( LoadedMesh.Object );
	}
	
	
// 	static ConstructorHelpers::FObjectFinder<UMaterial> LoadedMaterial ( TEXT ( "'/Engine/VREditor/BasicMeshes/MI_Ball_01'" ) );
// 	if (LoadedMaterial.Succeeded ( ))
// 	{
// 		MeshComp->SetMaterial(0, LoadedMaterial.Object);
// 	}

// 	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> LoadedPhysicalMaterial ( TEXT ( "'/Game/Project/Personal/HSW/Blueprints/PM_Ball'" ) );
// 
// 	if (LoadedPhysicalMaterial.Succeeded ( ))
// 	{
// 		MeshComp->SetPhysMaterialOverride ( LoadedPhysicalMaterial.Object );
// 	}
// 
// 
// 	MeshComp->BodyInstance.bNotifyRigidBodyCollision = true;
// 	MeshComp->CanCharacterStepUpOn = ECB_No;
// 	MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic" ));

}

// Called when the game starts or when spawned
void AHSW_ThrowingObject::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic ( this , &AHSW_ThrowingObject::OnMyObjectBeginverlap );

	GetWorldTimerManager ( ).SetTimer ( TimerHandleDestroy , FTimerDelegate::CreateLambda ( [&]
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "TimeOver Destroyed" ) );
			this->Destroy ( );
		} ) , 15.f , false );
}

// Called every frame
void AHSW_ThrowingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHSW_ThrowingObject::ChangeMesh ( int32 meshIndex )
{
	MeshComp->SetStaticMesh( MeshArray[meshIndex] );
}

void AHSW_ThrowingObject::DestroyObject ( )
{
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandleDestroy	, this , &AHSW_ThrowingObject::Destroy, DisappearTime );

	//GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandleDestroy , this , this->Destroy() , DisappearTime );

	GetWorldTimerManager ( ).SetTimer ( TimerHandleDestroy , FTimerDelegate::CreateLambda ( [&]
		{
			this->Destroy ( );
		} ) , DisappearTime , false );
}

void AHSW_ThrowingObject::OnMyObjectBeginverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (!( OtherActor->IsA<AHSW_ThrowingObject> ( ) )) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s" ), *OtherActor->GetName() );
		DestroyObject( );
	}
}
