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
   Emoji,
   Fever,
   Throw,
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
   void EmojiFucn(const float& DeltaTime );

   void AppearImoji ( int32 num );
   void DisappearImoji ( );

   FTimerHandle timerHandle;

   UPROPERTY ( EditAnywhere )
   TArray<UTexture2D*> imojiFact;

   UPROPERTY( EditAnywhere)
   class UWidgetComponent* imojiComp;
   UPROPERTY( )
   class UDummyUI_KMK* widget;


   UPROPERTY(Replicated )
   bool isJump = false;

   UPROPERTY(Replicated )
   bool isImoji = false;

   float Brightness = 1;

   UPROPERTY( )
   class AHSW_ThirdPersonCharacter* ServerPlayer;

   UFUNCTION(BlueprintImplementableEvent, Category= Fever )
   void ShakeBodyBlueprint( );

   
   UFUNCTION( )
   void SetBrightness( float brightValue );

   UFUNCTION( )
   void SetFace(float faveValue );
   float FaceTimer;
   //UPROPERTY ( Replicated , EditDefaultsOnly , BlueprintReadWrite )
   //int32 IntervieweeIndex;
   int32 FaceRand = 1;

   float ImojiTimer = 4;

   bool bVisible = false;

   UFUNCTION(Server,Reliable )
   void ServerRPC_Shake( float brightValue );

   UFUNCTION(NetMulticast,Reliable )
   void MulticastRPC_Shake( float brightValue );


   UFUNCTION(Server,Reliable )
   void ServerRPC_Jump( const float& DeltaTime );

   UFUNCTION(NetMulticast,Reliable )
   void MulticastRPC_Jump( const float& DeltaTime );

   UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = FeverGauge )
   UMaterialInstance* FeverCharactMat;

   UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = FeverGauge )
   UMaterialInstanceDynamic* FeverDynamicMat;

   UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = FeverGauge )
   UMaterialInstance* FaceMat;

   UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = FeverGauge )
   UMaterialInstanceDynamic* FaceDynamicMat;

   // Throwing =======================================================

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
   class UArrowComponent* ThrowingArrow;

   UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite )
   class AHSW_ThrowingObject* ThrowingObject;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
   TSubclassOf<class AHSW_ThrowingObject> ThrowingObjectFactory;

   UPROPERTY(EditDefaultsOnly,BlueprintReadWrite )
   float ThrowingSpeed = 8000000.0f;

   UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
   FRotator ThrowingRotator;

   UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite )
   bool bCanThrow = false;

   UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite )
   bool bStart = false;


   UPROPERTY( Replicated, EditDefaultsOnly , BlueprintReadWrite )
   int32 ThrowingObjectIndex = 0;

   UFUNCTION( )
   void CreateThrowingObject( );

   UFUNCTION(NetMulticast, Reliable)
   void MulticastRPC_ThrowObject( );

   UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
   bool bThrowing;

   float ThrowTimer = 0.5;
};
