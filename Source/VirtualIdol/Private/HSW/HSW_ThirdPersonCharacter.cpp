// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "HSW/HSW_MainWidget.h"
#include "Components/ProgressBar.h"
#include "HSW/HSW_FeverGaugeWidget.h"
#include "HSW/HSW_AuditoriumGameMode.h"
#include "HSW/HSW_ImojiConponent.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "HSW/HSW_ImogiWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

// Sets default values
AHSW_ThirdPersonCharacter::AHSW_ThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent ( )->InitCapsuleSize ( 42.f , 96.0f );

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement ( )->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement ( )->RotationRate = FRotator ( 0.0f , 500.0f , 0.0f ); // ...at this rotation rate
	GetCharacterMovement()->bUseControllerDesiredRotation=true;
	//GetCharacterMovement()->UCharacterMovementComponent::SetMovementMode ( EMovementMode::MOVE_Flying);
	GetCharacterMovement ( )->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement ( )->JumpZVelocity = 700.f;
	GetCharacterMovement ( )->AirControl = 0.35f;
	GetCharacterMovement ( )->MaxWalkSpeed = 500.f;
	GetCharacterMovement ( )->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement ( )->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement ( )->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "CameraBoom" ) );
	CameraBoom->SetupAttachment ( RootComponent );
	CameraBoom->SetRelativeLocation(FVector(0.0f,0.0f,30.f));
	CameraBoom->TargetArmLength = 250.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "FollowCamera" ) );
	FollowCamera->SetupAttachment ( CameraBoom , USpringArmComponent::SocketName ); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Imoji Component
// 	ImojiComponent = CreateDefaultSubobject<UHSW_ImojiConponent>(TEXT("ImojiComponent" ));
// 	ImojiComponent->SetupAttachment(GetMesh());
// 	ImojiComponent->SetRelativeLocation(FVector(0,0,230.f));

// 	ImojiBillboard = CreateDefaultSubobject<UBillboardComponent> ( TEXT ( "ImojiBilboard" ) );
// 	ImojiBillboard->SetVisibility ( true );
// 	ImojiBillboard->bHiddenInGame = false;
// 	ImojiBillboard->SetupAttachment ( GetMesh() );
// 	ImojiBillboard->SetRelativeLocation(FVector(0,0,230.f));

	ImojiComp = CreateDefaultSubobject<UWidgetComponent> ( TEXT ( "ImojiWidget" ) );
	ImojiComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FClassFinder<UHSW_ImogiWidget> loadedImojiWidget ( TEXT ( "'/Game/Project/Personal/HSW/UI/WBP_Imogi.WBP_Imogi_C'" ) );

	if (loadedImojiWidget.Succeeded ( ))
	{
		ImojiComp->SetWidgetClass( loadedImojiWidget.Class);
		ImojiComp->SetDrawSize (FVector2D(100,20 ) );
		ImojiComp->SetRelativeLocation(FVector(0,0,230));
		ImojiComp->SetRelativeRotation( FRotator ( 0 , 90 , 0 ) );
	}

// 	static ConstructorHelpers::FObjectFinder<UMaterial> LoadedOpacityMaterial ( TEXT ( "Material'/Game/Project/Personal/HSW/Resources/Imogi/M_Imoji_Opacity'" ) );
// 	if (LoadedOpacityMaterial.Succeeded ( ))
// 	{
// 		OpacityMaterial = LoadedOpacityMaterial.Object;
// 
// 		if (OpacityMaterial)
// 		{
// 			ImojiMesh->SetMaterial ( 0 , OpacityMaterial );
// 		}
// 	}
}

// Called when the game starts or when spawned
void AHSW_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitMainUI();
	ImojiComp->SetVisibility(false);
	
}

// Called every frame
void AHSW_ThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ImojiComp && ImojiComp->GetVisibleFlag ( ))
	{
		// 카메라 위치 
		FVector CamLoc = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0 )->GetCameraLocation();
		FVector Direction = CamLoc - ImojiComp->GetComponentLocation();
		Direction.Z=0;

		ImojiComp->SetWorldRotation(Direction.GetSafeNormal().ToOrientationRotator());
	}
}

void AHSW_ThirdPersonCharacter::InitMainUI ( )
{
	
	MainUI = Cast<UHSW_MainWidget> ( CreateWidget ( GetWorld ( ) , MainUIFactory ) );
	if (MainUI)
	{
		MainUI->AddToViewport ( );
	}
}

void AHSW_ThirdPersonCharacter::Imoji ( int index )
{
	UHSW_ImogiWidget* imojiWidget = Cast<UHSW_ImogiWidget>( ImojiComp->GetWidget());
	imojiWidget->ImogiImage->SetBrushFromTexture( ImojiImageArray[index] ) ;

	AppearImoji( );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandleImoji );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandleImoji , this , &AHSW_ThirdPersonCharacter::DisappearImoji , 2.0f );
}

void AHSW_ThirdPersonCharacter::AppearImoji ( )
{
	ImojiComp->SetVisibility(true);
}

void AHSW_ThirdPersonCharacter::DisappearImoji ( )
{
	ImojiComp->SetVisibility ( false );
}

// Called to bind functionality to input
void AHSW_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent ( PlayerInputComponent );

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController> ( GetController ( ) ))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( PlayerController->GetLocalPlayer ( ) ))
		{
			Subsystem->AddMappingContext ( DefaultMappingContext , 0 );
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent> ( PlayerInputComponent )) {

		// Jumping
		EnhancedInputComponent->BindAction ( JumpAction , ETriggerEvent::Started , this , &ACharacter::Jump );
		EnhancedInputComponent->BindAction ( JumpAction , ETriggerEvent::Completed , this , &ACharacter::StopJumping );

		// Moving
		EnhancedInputComponent->BindAction ( MoveAction , ETriggerEvent::Triggered , this , &AHSW_ThirdPersonCharacter::Move );

		// Looking
		EnhancedInputComponent->BindAction ( LookAction , ETriggerEvent::Triggered , this , &AHSW_ThirdPersonCharacter::Look );

		//FeverGauge
		EnhancedInputComponent->BindAction ( FeverGaugeAction , ETriggerEvent::Started , this , &AHSW_ThirdPersonCharacter::OnMyFeverGauge );

	}

}

void AHSW_ThirdPersonCharacter::Move ( const FInputActionValue& Value )
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D> ( );

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation ( );
		const FRotator PitchRotation (Rotation.Pitch , 0, 0 );
		const FRotator YawRotation ( 0 , Rotation.Yaw , 0 );
		const FRotator RollRotation(0, 0 , Rotation.Roll );

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::X );

		// get right vector 
		const FVector RightDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::Y );

		// get up vector
		const FVector UpDirection = FRotationMatrix ( PitchRotation ).GetUnitAxis ( EAxis::X );
		
		// add movement 
		AddMovementInput ( ForwardDirection , MovementVector.Y );
		AddMovementInput ( RightDirection , MovementVector.X );

		if (MovementVector.Y > 0)
		{
// 			FVector CurrentLocation = GetActorLocation();
// 			CurrentLocation += FVector ( 0 , 0 , 1 ) * 200.f * GetWorld()->GetDeltaSeconds();
// 			SetActorLocation(CurrentLocation);
// 			UE_LOG ( LogTemp , Warning , TEXT ( "MovementVector: %s" ) , *MovementVector.ToString ( ) );
			AddMovementInput ( UpDirection , MovementVector.Y );
		}
	}
}

void AHSW_ThirdPersonCharacter::Look ( const FInputActionValue& Value )
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D> ( );

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput ( LookAxisVector.X );
		AddControllerPitchInput ( LookAxisVector.Y );
	}
}

void AHSW_ThirdPersonCharacter::OnMyFeverGauge ( const FInputActionValue& value )
{
	if (CurrentGauge <= 1)
	{
		CurrentGauge += FeverPoint;
		MainUI->FeverGauge->SetFeverGauge( CurrentGauge );
	}
	else if (CurrentGauge > 1)
	{
		AHSW_AuditoriumGameMode* gameMode = Cast<AHSW_AuditoriumGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->bFevered = true;
		}
	}
}

