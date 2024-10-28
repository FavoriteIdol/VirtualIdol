// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Dummy_KMK.h"
#include "Components/BillboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "KMK/DummyUI_KMK.h"
#include "Components/WidgetComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HSW/HSW_ThirdPersonCharacter.h"

// Sets default values
ADummy_KMK::ADummy_KMK()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADummy_KMK::BeginPlay()
{
	Super::BeginPlay();
	imojiComp = FindComponentByClass<UWidgetComponent>( );
	if (imojiComp)
	{
		widget = Cast<UDummyUI_KMK>(imojiComp->GetWidget());
	}

	FeverDynamicMat = UMaterialInstanceDynamic::Create ( FeverCharactMat , this );
	FaceDynamicMat = UMaterialInstanceDynamic::Create ( FaceMat , this );

	USkeletalMeshComponent* TempMesh = GetMesh ( );
	if (TempMesh)
	{
		TempMesh->SetMaterial ( 0 , FeverDynamicMat );
		TempMesh->SetMaterial ( 2 , FaceDynamicMat );
	}

}

// Called every frame
void ADummy_KMK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (state)
	{
	case DummyState::Idle:
		IdleFucn(DeltaTime);
		break;
	case DummyState::Jump:
		if(HasAuthority()) ServerRPC_Jump(DeltaTime);
		break;
	case DummyState::Move:
		MoveFucn(DeltaTime);
		break;
	case DummyState::Imoji:
		if(!isImoji) ImojiFucn(DeltaTime);
		break;
	case DummyState::Fever:
		if (HasAuthority ( )) ServerRPC_Shake(0.1);
		break;
	}
}

// Called to bind functionality to input
void ADummy_KMK::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy_KMK::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	DOREPLIFETIME ( ADummy_KMK , state );
	DOREPLIFETIME ( ADummy_KMK , isJump );
	DOREPLIFETIME ( ADummy_KMK , isImoji );
}

void ADummy_KMK::IdleFucn ( const float& DeltaTime )
{
	int32 rand = FMath::RandRange(0, 80);
	if(isJump) isJump = false;
	switch (rand)
	{
	case 0:
		state = DummyState::Jump;
		break;
	case 1:
		state = DummyState::Imoji;
		break;
	case 2:
		state = DummyState::Fever;
		break;
	case 3:
		state = DummyState::Fever;
		break;
	case 4:
		state = DummyState::Fever;
		break;
	case 5:
		state = DummyState::Fever;
		break;
	case 6:
		state = DummyState::Fever;
		break;
	default:
		break;
	}

	
	FaceTimer += DeltaTime;
	if (FaceRand == 0 && FaceTimer >= 3)
	{
		// 상큼한 표정
		FaceRand = FMath::RandRange ( 0 , 1 );
		FaceTimer = 0;
		SetFace(0.5);
	}
	else if (!(FaceRand == 0) && FaceTimer >= 2)
	{
		// 기본 표정
		FaceRand = FMath::RandRange ( 0 , 1);
		FaceTimer = 0;
		SetFace (1);
	}

}

void ADummy_KMK::JumpFunc ( const float& DeltaTime )
{
	Jump ( );

}

void ADummy_KMK::MoveFucn ( const float& DeltaTime )
{

}

void ADummy_KMK::ImojiFucn ( const float& DeltaTime )
{
	int32 rand = FMath::RandRange(0, imojiFact.Num() - 1);
	int32 rnd = FMath::RandRange(0, 1);
	isImoji = true;
	AppearImoji(rand, rnd);
}

void ADummy_KMK::AppearImoji ( int32 num, int32 bVisible)
{
	if(!widget) return;
	widget->SetImage(imojiFact[num] );
	widget->SetImageVisible(bVisible);
	GetWorld ( )->GetTimerManager ( ).ClearTimer ( timerHandle );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( timerHandle , this , &ADummy_KMK::DisappearImoji , 1.0f );
}

void ADummy_KMK::DisappearImoji ( )
{
	state = DummyState::Idle;
	isImoji = false;
}

void ADummy_KMK::SetBrightness ( float brightValue )
{
	if(FeverDynamicMat)
	FeverDynamicMat->SetScalarParameterValue ( TEXT ( "jswEmissivePower-A" ) , brightValue );
}

void ADummy_KMK::SetFace ( float faveValue)
{
	if (FeverDynamicMat)
	FaceDynamicMat->SetScalarParameterValue ( TEXT ( "jswFaceChange" ) , faveValue );
}

void ADummy_KMK::ServerRPC_Shake_Implementation ( float brightValue )
{
	Brightness += brightValue;
	MulticastRPC_Shake( Brightness );
}

void ADummy_KMK::MulticastRPC_Shake_Implementation ( float brightValue )
{
	int32 rand = FMath::RandRange ( 0 , 1 );
	if (rand == 0 )
	{
		state = DummyState::Idle;
	}
	else
	{
		ShakeBodyBlueprint( );
		SetBrightness(brightValue);
	}
}

void ADummy_KMK::ServerRPC_Jump_Implementation ( const float& DeltaTime )
{
	if (!isJump)
	{
		int32 rand = FMath::RandRange ( 0 , 40);
		if (rand == 0)
		{	
			MulticastRPC_Jump(DeltaTime );
			isJump = true;
		}
	}
	else
	{
		state = DummyState::Idle;
		isJump = false;
	}
}

void ADummy_KMK::MulticastRPC_Jump_Implementation ( const float& DeltaTime )
{
	// 더미끼리의 점프
	if(IsLocallyControlled ()) Jump( );
	//if(!HasAuthority()&&!IsLocallyControlled())	UE_LOG ( LogTemp , Warning , TEXT ( "Jump" ) );
	/*JumpFunc ( DeltaTime );*/
}
