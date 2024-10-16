﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HSW_ThirdPersonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class VIRTUALIDOL_API AHSW_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** FeverGauge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FeverGaugeAction;

public:
	// Sets default values for this character's properties
	AHSW_ThirdPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for movement input */
	void Move ( const FInputActionValue& Value );

	/** Called for looking input */
	void Look ( const FInputActionValue& Value );

	void OnMyFeverGauge ( const FInputActionValue& value );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom ( ) const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera ( ) const { return FollowCamera; }

public:

	UPROPERTY(EditDefaultsOnly , Category = FeverGauge )
	class UHSW_FeverGaugeWidget* FeverGauge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float CurrentGauge = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float FeverPoint = 0.02f;

	// MainWidget을 생성해서 기억하고싶다.
	UPROPERTY(EditDefaultsOnly, Category = MainUI)
	TSubclassOf<class UUserWidget> MainUIFactory;

	UPROPERTY(EditDefaultsOnly )
	class UHSW_MainWidget* MainUI;

	UFUNCTION( )
	void InitMainUI();

	// 이모지 ------------------------------------
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Imoji )
// 	class UHSW_ImojiConponent* ImojiComponent;
// 
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Imoji )
// 	class UBillboardComponent* ImojiBillboard;
// 
// 	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite, Category = Imoji )
// 	class UMaterial* OpacityMaterial;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Imoji )
	class UWidgetComponent* ImojiComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Imoji )
	TArray<class UTexture2D *> ImojiImageArray;

	UFUNCTION ( )
	void Imoji ( int index );

	void AppearImoji ();

	void DisappearImoji ();

	FTimerHandle TimerHandleImoji;

	UPROPERTY( )
	class UHSW_ImogiWidget* imojiWidget;

	UFUNCTION(BlueprintImplementableEvent, Category= Imogi )
	void FadeInImogiBlueprint( );

};
