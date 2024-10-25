// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Dummy_KMK.h"
#include "Components/BillboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "KMK/DummyUI_KMK.h"
#include "Components/WidgetComponent.h"

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
		JumpFunc(DeltaTime);
		break;
	case DummyState::Move:
		MoveFucn(DeltaTime);
		break;
	case DummyState::Imoji:
		if(!isImoji) ImojiFucn(DeltaTime);
		break;
	}
}

// Called to bind functionality to input
void ADummy_KMK::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADummy_KMK::IdleFucn ( const float& DeltaTime )
{
	int32 rand = FMath::RandRange(0, 1);
	if(isJump)isJump = false;
	switch (rand)
	{
	case 0:
		state = DummyState::Jump;
		break;
	case 1:
		state = DummyState::Imoji;
		break;
	//case 2:
	//	state = DummyState::Jump;
	//	break;
	//case3:
	//	state = DummyState::Jump;
	//	break;
	}
}

void ADummy_KMK::JumpFunc ( const float& DeltaTime )
{
	if (!isJump)
	{
		Jump();
		isJump = true;
	}
	else
	{
		state = DummyState::Idle;
		isJump = false;
	}
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
