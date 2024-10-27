// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dummy_KMK.generated.h"
UENUM ( )
enum class DummyState
{
	Idle,
	Jump,
	Move,
	Imoji,
	Fever
};
UCLASS()
class VIRTUALIDOL_API ADummy_KMK : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy_KMK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

	UPROPERTY(Replicated )
	DummyState state = DummyState::Idle;

	void IdleFucn(const float& DeltaTime );
	void JumpFunc(const float& DeltaTime );
	void MoveFucn(const float& DeltaTime );
	void ImojiFucn(const float& DeltaTime );

	void AppearImoji ( int32 num, int32 bVisible );
	void DisappearImoji ( );

	FTimerHandle timerHandle;

	UPROPERTY ( EditAnywhere )
	TArray<UTexture2D*> imojiFact;

	UPROPERTY( EditAnywhere)
	class UWidgetComponent* imojiComp;
	UPROPERTY( )
	class UDummyUI_KMK* widget;
	bool isJump = false;
	bool isImoji = false;

	UFUNCTION(BlueprintImplementableEvent, Category= Imogi )
	void ShakeBodyBlueprint( );

	//UPROPERTY ( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	//int32 IntervieweeIndex;

	UFUNCTION(Server,Reliable )
	void ServerRPC_Shake( float brightValue );

	UFUNCTION(NetMulticast,Reliable )
	void MulticastRPC_Shake( float brightValue );

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = FeverGauge )
	UMaterialInstance* FeverCharactMat;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = FeverGauge )
	UMaterialInstanceDynamic* FeverDynamicMat;

};
