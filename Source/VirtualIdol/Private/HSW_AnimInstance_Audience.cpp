// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_AnimInstance_Audience.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHSW_AnimInstance_Audience::NativeInitializeAnimation ( )
{
	Super::NativeInitializeAnimation ( );

	Me = Cast<AHSW_ThirdPersonCharacter> ( TryGetPawnOwner ( ) );
}

void UHSW_AnimInstance_Audience::NativeUpdateAnimation ( float DeltaSeconds )
{

	Super::NativeUpdateAnimation ( DeltaSeconds );

	if (Me == nullptr)
	{
		return;
	}

	FVector vel = Me->GetVelocity( );
	FVector right = Me->GetActorRightVector();
	FVector forward = Me->GetActorForwardVector();

	Horizental = FVector::DotProduct( vel, right);
	Vertical = FVector::DotProduct ( vel , forward );

	IsInAir = Me->GetCharacterMovement ( )->IsFalling ( );
}

void UHSW_AnimInstance_Audience::PlayThrowMontage ( )
{
	if (ThrowMontage)
	{	
		Montage_Play( ThrowMontage );
		Montage_JumpToSection(FName("Start") , ThrowMontage );
	}
}
