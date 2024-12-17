// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Dummy_KMK.h"
#include "Components/BillboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "KMK/DummyUI_KMK.h"
#include "Components/WidgetComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Components/ArrowComponent.h"
#include "HSW_ThrowingObject.h"
#include "HSW_AnimInstance_Audience.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ADummy_KMK::ADummy_KMK ( )
{
   // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;

   ThrowingArrow = CreateDefaultSubobject<UArrowComponent> ( TEXT ( "ThrowingArrow" ) );
   ThrowingArrow->SetupAttachment ( RootComponent );
   ThrowingArrow->SetRelativeLocation ( FVector ( 80 , 0 , 70 ) );
}

// Called when the game starts or when spawned
void ADummy_KMK::BeginPlay ( )
{
   Super::BeginPlay ( );
   imojiComp = FindComponentByClass<UWidgetComponent> ( );
   if (imojiComp)
   {
      widget = Cast<UDummyUI_KMK> ( imojiComp->GetWidget ( ) );
   }

   FeverDynamicMat = UMaterialInstanceDynamic::Create ( FeverCharactMat , this );
   FaceDynamicMat = UMaterialInstanceDynamic::Create ( FaceMat , this );

   USkeletalMeshComponent* TempMesh = GetMesh ( );
   if (TempMesh)
   {
      TempMesh->SetMaterial ( 0 , FeverDynamicMat );
      TempMesh->SetMaterial ( 2 , FaceDynamicMat );
   }
   
   ThrowingObjectIndex = FMath::RandRange ( 0 , 2 );

   GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

// Called every frame
void ADummy_KMK::Tick ( float DeltaTime )
{
    Super::Tick ( DeltaTime );

//    UE_LOG ( LogTemp , Warning , TEXT ( "IsFalling: %s" ) , GetCharacterMovement ( )->IsFalling ( ) ? TEXT ( "True" ) : TEXT ( "False" ) );
//     UE_LOG ( LogTemp , Warning , TEXT ( "CanJump: %s" ) , CanJump ( ) ? TEXT ( "True" ) : TEXT ( "False" ) );
//    UE_LOG ( LogTemp , Warning , TEXT ( "MovementMode: %d" ) , (int32)GetCharacterMovement ( )->MovementMode );
//    UE_LOG ( LogTemp , Warning , TEXT ( "JumpZVelocity: %f" ) , GetCharacterMovement ( )->JumpZVelocity );
//    UE_LOG ( LogTemp , Warning , TEXT ( "GravityScale: %f" ) , GetCharacterMovement ( )->GravityScale );
//   UE_LOG ( LogTemp , Warning , TEXT ( "MaxWalkSpeed: %f" ) , GetCharacterMovement ( )->MaxWalkSpeed );
/*   UE_LOG ( LogTemp , Warning , TEXT ( "CanEverJump: %s" ) , GetCharacterMovement ( )->CanEverJump ( ) ? TEXT ( "True" ) : TEXT ( "False" ) );*/

    switch (state)
    {
    case DummyState::Idle:
        IdleFucn ( DeltaTime );
        break;
    case DummyState::Jump:
       //JumpFunc( DeltaTime );
       if (HasAuthority ( )) ServerRPC_Jump ( DeltaTime );
       break;
    case DummyState::Move:
       MoveFucn ( DeltaTime );
       break;
    case DummyState::Emoji:
       EmojiFucn ( DeltaTime );
       break;
    case DummyState::Fever:
       if (HasAuthority ( )) ServerRPC_Shake ( 0.03 );
       break;
    case DummyState::Throw:
       CreateThrowingObject( );
       break;
    }

    FaceTimer += DeltaTime;
    ImojiTimer += DeltaTime;
    ThrowTimer += DeltaTime;

    if (FaceRand == 0 && FaceTimer >= 3)
    {
       // 상큼한 표정
       FaceRand = FMath::RandRange ( 0 , 1 );
       FaceTimer = 0;
       SetFace ( 0.5 );
    }
    else if (!( FaceRand == 0 ) && FaceTimer >= 2)
    {
       // 기본 표정
       FaceRand = FMath::RandRange ( 0 , 1 );
       FaceTimer = 0;
       SetFace ( 1 );
    }
}

// Called to bind functionality to input
void ADummy_KMK::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
   Super::SetupPlayerInputComponent ( PlayerInputComponent );

}

void ADummy_KMK::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
   Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

   DOREPLIFETIME ( ADummy_KMK , state );
   DOREPLIFETIME ( ADummy_KMK , isJump );
   DOREPLIFETIME ( ADummy_KMK , isImoji );
   DOREPLIFETIME ( ADummy_KMK , bCanThrow ); 
   DOREPLIFETIME ( ADummy_KMK , ThrowingObject ); 
   DOREPLIFETIME ( ADummy_KMK , bThrowing );
}

void ADummy_KMK::IdleFucn ( const float& DeltaTime )
{
    int32 rand = 0;
    
    if (!bCanThrow)
    {
        if(!bStart)
        {
            rand = FMath::RandRange ( 5 , 80 );
        }
        else 
        {
            rand = FMath::RandRange ( 0 , 80 );
        }
    }
    else 
    {
        rand = FMath::RandRange ( 0 , 81 );
    }

   if (isJump) isJump = false;

   // 이모지가 True인데, rand가 이모지 값인 경우
   if (( rand == 6 ) && isImoji == true)
   {
   // rand를 점프로 바꾼다.
      rand = 5;
   }

   switch (rand)
   {
   case 0:
      state = DummyState::Fever;
      break;
   case 1:
      state = DummyState::Fever;
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
      state = DummyState::Jump;
      break;
   case 6:
      state = DummyState::Emoji;
      break;
   case 81:
      state = DummyState::Throw;
      break;
   default:
      break;
   }


}

void ADummy_KMK::JumpFunc ( const float& DeltaTime )
{
    if (!isJump)
    {
        int32 rand = FMath::RandRange ( 0 , 40 );
        if (rand == 0)
        {
            //UE_LOG ( LogTemp , Warning , TEXT ( "ServerRPC_Jump: Call MultiRPC Jump" ) );
            Jump();
            isJump = true;
        }
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

void ADummy_KMK::EmojiFucn ( const float& DeltaTime )
{
   if (ImojiTimer >= 2.5f)
   {
      ImojiTimer = 0;

      int32 rand = FMath::RandRange ( 0 , imojiFact.Num ( ) - 1 );
      isImoji = true;
      AppearImoji ( rand );
   }
   else
   {
      state = DummyState::Idle;

   }
}

void ADummy_KMK::AppearImoji ( int32 num )
{
   if (!widget) return;
   widget->SetImage ( imojiFact[num] );
   widget->SetImageVisible ( true );

   GetWorld ( )->GetTimerManager ( ).ClearTimer ( timerHandle );
   GetWorld ( )->GetTimerManager ( ).SetTimer ( timerHandle , this , &ADummy_KMK::DisappearImoji , 1.f );
}

void ADummy_KMK::DisappearImoji ( )
{
   widget->SetImageVisible ( false );
   state = DummyState::Idle;
   isImoji = false;
}

void ADummy_KMK::SetBrightness ( float brightValue )
{
   if (FeverDynamicMat)
      FeverDynamicMat->SetScalarParameterValue ( TEXT ( "jswEmissivePower-A" ) , brightValue );
}

void ADummy_KMK::SetFace ( float faveValue )
{
   if (FeverDynamicMat)
      FaceDynamicMat->SetScalarParameterValue ( TEXT ( "jswFaceChange" ) , faveValue );
}

void ADummy_KMK::ServerRPC_Shake_Implementation ( float brightValue )
{
   Brightness += brightValue;
   MulticastRPC_Shake ( Brightness );
}

void ADummy_KMK::MulticastRPC_Shake_Implementation ( float brightValue )
{
   int32 rand = FMath::RandRange ( 0 , 1 );
   if (rand == 0)
   {
      state = DummyState::Idle;
   }
   else
   {
      ShakeBodyBlueprint ( );
      SetBrightness ( brightValue );
   }
}

void ADummy_KMK::ServerRPC_Jump_Implementation ( const float& DeltaTime )
{
   //UE_LOG(LogTemp,Warning,TEXT("ServerRPC_Jump" ) );
   if (!isJump)
   {
      int32 rand = FMath::RandRange ( 0 , 40 );
      if (rand == 0)
      {
         //UE_LOG ( LogTemp , Warning , TEXT ( "ServerRPC_Jump: Call MultiRPC Jump" ) );
         MulticastRPC_Jump ( DeltaTime );
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
   //UE_LOG ( LogTemp , Warning , TEXT ( "Jump!!" ) );
   Jump ( );
   //if(!HasAuthority()&&!IsLocallyControlled())   UE_LOG ( LogTemp , Warning , TEXT ( "Jump" ) );
   /*JumpFunc ( DeltaTime );*/
}

void ADummy_KMK::CreateThrowingObject ( )
{
   //bThrowing = true;
   if (ThrowTimer > 0.5)
   {
      FTransform t = ThrowingArrow->GetComponentTransform ( );
      ThrowingObject = GetWorld ( )->SpawnActor<AHSW_ThrowingObject> ( ThrowingObjectFactory , t );
      if (ThrowingObject)
      {
         ThrowingObject->ChangeMesh ( ThrowingObjectIndex );
         ThrowingObject->MeshComp->SetSimulatePhysics ( true );

         if (ThrowingObjectIndex == 1)
         {
            ThrowingObject->MeshComp->SetMassOverrideInKg(NAME_None,200.f,true);

         }
         else if (ThrowingObjectIndex == 2)
         {
            ThrowingObject->MeshComp->SetMassOverrideInKg ( NAME_None , 200.f , true );

         }

         FVector ThrowingForce = ThrowingArrow->GetForwardVector ( ) * ThrowingSpeed;
         ThrowingObject->MeshComp->AddForce ( ThrowingForce );
         ThrowingObject = nullptr;

         ShakeBodyBlueprint ( );
         // 재장전 애니메이션 재생
         auto* anim = Cast<UHSW_AnimInstance_Audience> ( GetMesh ( )->GetAnimInstance ( ) );
         if (anim)
         {
            anim->PlayThrowMontage ( );
         }
      }
   }
   state = DummyState::Idle;
//    if (HasAuthority ( ) && !bThrowing)
//    {
//       bThrowing = true;
//       FTransform t = ThrowingArrow->GetComponentTransform ( );
//       ThrowingObject = GetWorld ( )->SpawnActor<AHSW_ThrowingObject> ( ThrowingObjectFactory , t );
//       if (ThrowingObject)
//       {
//          ThrowingObject->ChangeMesh ( ThrowingObjectIndex );
//          ThrowingObject->AttachToComponent ( ThrowingArrow , FAttachmentTransformRules::KeepWorldTransform );
// 
//          state = DummyState::Idle;
//          MulticastRPC_ThrowObject( );
//       }
//    }
}

void ADummy_KMK::MulticastRPC_ThrowObject_Implementation ()
{
   ThrowingObject->DetachFromActor ( FDetachmentTransformRules::KeepWorldTransform );
   ThrowingObject->MeshComp->SetSimulatePhysics ( true );

   FVector ThrowingForce = ThrowingArrow->GetForwardVector ( ) * ThrowingSpeed;
   ThrowingObject->MeshComp->AddForce ( ThrowingForce );
   ThrowingObject = nullptr;

   ShakeBodyBlueprint ( );
   // 재장전 애니메이션 재생
   auto* anim = Cast<UHSW_AnimInstance_Audience> ( GetMesh ( )->GetAnimInstance ( ) );
   if (anim)
   {
      anim->PlayThrowMontage ( );
   }

}
