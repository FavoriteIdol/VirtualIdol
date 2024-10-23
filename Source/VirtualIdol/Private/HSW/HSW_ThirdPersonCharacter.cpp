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
#include "Components/ArrowComponent.h"
#include "HSW_ThrowingObject.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "HSW_AnimInstance_Audience.h"
#include "Net/UnrealNetwork.h"
#include "HSW/HSW_PlayerController.h"
#include "HSW/HSW_GameState_Auditorium.h"
#include "GameFramework/PlayerState.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "KMK/Audience_KMK.h"

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
	// GetCharacterMovement()->UCharacterMovementComponent::SetMovementMode ( EMovementMode::MOVE_Flying);
	// GetCharacterMovement ( )->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement ( )->JumpZVelocity = 3500.f;
	GetCharacterMovement ( )->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement ( )->AirControl = 5.0f;
	GetCharacterMovement ( )->MaxWalkSpeed = 500.f;
	GetCharacterMovement ( )->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement ( )->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement ( )->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement( )->GravityScale=15.0f;

	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "CameraBoom" ) );
	CameraBoom->SetupAttachment ( RootComponent );
	CameraBoom->SetRelativeLocation(FVector(0.0f,0.0f,30.f));
	CameraBoom->TargetArmLength = 450; // The camera follows at this distance behind the character	
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
	ImojiComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UHSW_ImogiWidget> loadedImojiWidget ( TEXT ( "'/Game/Project/Personal/HSW/UI/WBP_Imogi.WBP_Imogi_C'" ) );

	if (loadedImojiWidget.Succeeded ( ))
	{
		ImojiComp->SetWidgetClass( loadedImojiWidget.Class);
		ImojiComp->SetDrawSize (FVector2D(100,100 ) );
		ImojiComp->SetRelativeLocation(FVector(0,0,150));
	}

	ThrowingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ThrowingArrow" ) );
	ThrowingArrow->SetupAttachment(RootComponent);
	ThrowingArrow->SetRelativeLocation(FVector(80,0,70));

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
	bReplicates = true;
	SetReplicateMovement ( true );
}

// Called when the game starts or when spawned
void AHSW_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled ( ) && HasAuthority() == false)
	{
		InitMainUI();
	}
	ImojiComp->SetVisibility(false);
	imojiWidget = Cast<UHSW_ImogiWidget> ( ImojiComp->GetWidget ( ) );

	FInputModeGameAndUI a;
	GetWorld()->GetFirstPlayerController()->SetInputMode(a);
	GetWorld ( )->GetFirstPlayerController ( )->bShowMouseCursor=true;

	gm = Cast<AHSW_AuditoriumGameMode>(GetWorld()->GetAuthGameMode() );
	gs = GetWorld()->GetGameState<AHSW_GameState_Auditorium>();
	if (gs == nullptr)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "GameState is null!" ) );
		return;
	}

	FName tag = TEXT ( "InterviewLocation" );
	for (TActorIterator<AActor> It ( GetWorld ( ) , AStaticMeshActor::StaticClass ( ) ); It; ++It)
	{
		AActor* Actor = *It;
		if (IsValid ( Actor ) && Actor->ActorHasTag ( tag ))
		{
			InterviewLocation = Actor->GetTransform ( );
		}
	}

#pragma region KMK
	pc = GetWorld()->GetFirstPlayerController();
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
	if (IsLocallyControlled ( ))
	{
        if (HasAuthority ( ))
        {
			GetMesh ( )->bRenderInMainPass = false;
			GetMesh ( )->bRenderInDepthPass = false;
            if (virtualWidgetFact && !audienceWidget)
            {
				audienceWidget = CreateWidget<UAudience_KMK> ( GetWorld ( ) , virtualWidgetFact );
				//audienceWidget->AddToViewport ( );
				audienceWidget->pc = this;
				audienceWidget->SetVirtualWBP();
            }
			
        }
        else
        {
			audienceWidget = CreateWidget<UAudience_KMK> ( GetWorld ( ) , audienceWidgetFact );
			//audienceWidget->AddToViewport ( );
			audienceWidget->pc = this;
        }
		if (gi)
		{
			if (audienceWidget && gi->playerMeshNum == 1)
			{
				audienceWidget->VipAuthority ( );
			}
		}
		UE_LOG ( LogTemp , Warning , TEXT ( "StartTalk" ) );
		GetController<APlayerController> ( )->StartTalking ( );
		FInputModeGameAndUI inputMode;
		GetController<APlayerController> ( )->SetInputMode(inputMode );
	}
#pragma endregion

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

void AHSW_ThirdPersonCharacter::OnRep_FeverGauge ( )
{
	
}

void AHSW_ThirdPersonCharacter::PrintFeverGaugeLogOnHead ( )
{
	DrawDebugString ( GetWorld ( ) , GetActorLocation ( ) +FVector(0,0,90 ) , TEXT ( "GO!!" ) , nullptr , FColor::Red , 0.5f , true , 1 );
}


void AHSW_ThirdPersonCharacter::InitMainUI ( )
{

	auto* pc_h = Cast<AHSW_PlayerController> ( Controller );
	if (pc_h == nullptr)
	{
		return;
	}

	if (IsLocallyControlled ( ))
	{
		if (pc_h->mainUIWidget)
		{
			if (pc_h->MainUI == nullptr)
			{
				pc_h->MainUI = CastChecked<UHSW_MainWidget> ( CreateWidget ( GetWorld ( ) , pc_h->mainUIWidget ) );
				if (pc_h->MainUI == nullptr)
				{
					//UE_LOG ( LogTemp , Error , TEXT ( "Failed to create MainUI widget." ) );
				}
				else
				{
					//UE_LOG ( LogTemp , Error , TEXT ( "Succeed to create MainUI widget." ) );
				}
			}

			this->MainUI = pc_h->MainUI;
			MainUI->AddToViewport ( );
		}
	}
}


void AHSW_ThirdPersonCharacter::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AHSW_ThirdPersonCharacter , bThrowing ); 
	DOREPLIFETIME ( AHSW_ThirdPersonCharacter , CurrentGauge );
	DOREPLIFETIME ( AHSW_ThirdPersonCharacter , bIsInterviewing );
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

		//Throwing
		EnhancedInputComponent->BindAction ( ThrowAction , ETriggerEvent::Started , this , &AHSW_ThirdPersonCharacter::OnMyThorwHold );
		EnhancedInputComponent->BindAction ( ThrowAction , ETriggerEvent::Completed , this , &AHSW_ThirdPersonCharacter::OnMyThorwPitch );

		//Interview
		EnhancedInputComponent->BindAction ( InterviewAction , ETriggerEvent::Started , this , &AHSW_ThirdPersonCharacter::OnMyInterview );
		
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
			//AddMovementInput ( UpDirection , MovementVector.Y );
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

// Imoji ==============================================================================================

void AHSW_ThirdPersonCharacter::Imoji ( int index )
{
	imojiWidget->ImogiImage->SetBrushFromTexture ( ImojiImageArray[index] );

	ServerRPCImoji(  );
}

void AHSW_ThirdPersonCharacter::ServerRPCImoji_Implementation ( )
{
	MulticastRPCImoji(  );
}

void AHSW_ThirdPersonCharacter::MulticastRPCImoji_Implementation ( )
{

	AppearImoji (  );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandleImoji );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandleImoji , this , &AHSW_ThirdPersonCharacter::DisappearImoji , 2.0f );
}

void AHSW_ThirdPersonCharacter::AppearImoji (  )
{
	UNiagaraComponent* AppearEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation ( GetWorld ( ) , EmojiEffect , ImojiComp->GetComponentLocation ( ) );
	AppearEffect->SetAutoDestroy ( true );
	AppearEffect->AttachToComponent ( ImojiComp , FAttachmentTransformRules::KeepWorldTransform );

	ImojiComp->SetVisibility(true);
	imojiWidget->PlayFadeInImoji();
}

void AHSW_ThirdPersonCharacter::DisappearImoji ( )
{
	imojiWidget->PlayFadeOutImoji ( );
}

// FeverGauge ======================================================================================
void AHSW_ThirdPersonCharacter::OnMyFeverGauge ( const FInputActionValue& value )
{
	if (!HasAuthority ( ) && IsLocallyControlled())
	{
		//UE_LOG ( LogTemp , Warning , TEXT ( "The Fever Gauge is being stacked...Send ===========" ) );
		ServerRPCFeverGauge (CurrentGauge);
		PrintFeverGaugeLogOnHead ( );
		PersonalGauge++;
		//MainUI->FeverGauge->SetFeverGauge ( CurrentGauge );
	}
	//OnRep_FeverGauge( );
	//UE_LOG(LogTemp, Warning, TEXT("The Fever Gauge is being stacked..." ) );
}

void AHSW_ThirdPersonCharacter::ServerRPCFeverGauge_Implementation ( float feverValue )
{
	if (feverValue < 1)
	{
		feverValue += FeverPoint;
	}

	if (feverValue >= 0.3 && feverValue < 0.65)
	{
		if (gs)
		{
			gs->bFever30 = true;
		}
	}
	else if (feverValue >= 0.65 && feverValue < 1)
	{
		if (gs)
		{
			gs->bFever30 = false;
			gs->bFever65 = true;
		}
	}
	else if (feverValue >= 1)
	{
		if (gs)
		{
			gs->bFever30 = false;
			gs->bFever65 = false;
			gs->bFever100 = true;
		}
	}

	MulticastRPCFeverGauge( feverValue );
	//MulticastRPCFeverGauge_Implementation ( CurrentGauge );
}

void AHSW_ThirdPersonCharacter::MulticastRPCFeverGauge_Implementation (float AddGauge )
{
	ShakeBodyBlueprint ( );
	//UE_LOG(LogTemp, Warning, TEXT("=========================================" ) );
	SetFeverGaugeMulti ( AddGauge );

}

void AHSW_ThirdPersonCharacter::PossessedBy ( AController* NewController )
{
	Super::PossessedBy ( NewController );

	if (IsLocallyControlled ( ))
	{
		InitMainUI ( );
	}

}

void AHSW_ThirdPersonCharacter::SetFeverGaugeMulti ( float feverValue )
{
	// 	auto* widget = Cast<AHSW_ThirdPersonCharacter>(GetOwner() );
	// 	if(widget) auto* wid = widget->MainUI;
	// 
		// 로컬 컨트롤을 하는 캐릭터가 나 자신이라 MainUI도 가지고 있으니 그대로 갱신
	if (IsLocallyControlled ( ) && MainUI)
	{
		CurrentGauge = feverValue;
		MainUI->FeverGauge->SetFeverGauge ( CurrentGauge );
		// UE_LOG ( LogTemp , Error , TEXT ( "LocalPlayer Gauge: %f" ), CurrentGauge );
		// UE_LOG ( LogTemp , Warning , TEXT ( "In" ) );
	}
	// 로컬 컨트롤을 하는 캐릭터가 내가 아닌 상황이라 나는 MainUI가 없다. 그러니 나의 MainUI를 갱신해주자
	else if (!IsLocallyControlled ( ) && MainUI == nullptr)
	{
		AHSW_ThirdPersonCharacter* localPlayer = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetCharacter ( ) );
		if (localPlayer != nullptr)
		{
			localPlayer->CurrentGauge = feverValue;
			localPlayer->MainUI->FeverGauge->SetFeverGauge ( localPlayer->CurrentGauge );
			UE_LOG ( LogTemp , Error , TEXT ( "Not LocalPlayer Gauge: %f" ) , localPlayer->CurrentGauge );
			//UE_LOG ( LogTemp , Warning , TEXT ( "In2" ) );
		}
	}

	//if (MainUI)
	//{
	//	MainUI->FeverGauge->SetFeverGauge ( CurrentGauge );
	//	UE_LOG ( LogTemp , Warning , TEXT ( "In" ) );
	//}
	//UE_LOG ( LogTemp , Warning , TEXT ( "out" ) );
// 	auto* widget = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->MainUI;
// 	if(widget) 
// 	{
// 	widget->FeverGauge->SetFeverGauge ( CurrentGauge );
// 	UE_LOG ( LogTemp , Error , TEXT ( "%f" ) , CurrentGauge );
// 	}

}

// 인터뷰 =================================================================================================

void AHSW_ThirdPersonCharacter::OnMyInterview ( const FInputActionValue& value )
{
	if(HasAuthority())	ServerRPCInterview( );
}
void AHSW_ThirdPersonCharacter::ServerRPCInterview_Implementation (  )
{
	bIsInterviewing = !bIsInterviewing;


	PlayerStates = gs->PlayerArray;
	if (PlayerStates.Num ( ) > 0 && bIsInterviewing)
	{
		IntervieweeIndex = FMath::RandRange ( 1 , PlayerStates.Num ( ) - 1 );
		IntervieweePlayerState = PlayerStates[IntervieweeIndex];

		PreLocation = IntervieweePlayerState->GetPawn ( )->GetActorTransform ( );

	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "플레이어가 없습니다." ) );
	}

	MulticastRPCInterview( bIsInterviewing );
}

void AHSW_ThirdPersonCharacter::MulticastRPCInterview_Implementation ( float bInterview )
{
	

	if (bInterview)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Interview is in progress." ) );
		// 멀티캐스트 확인용 임시로 사용할 쉐이크바뤼
		ShakeBodyBlueprint ( );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Interview is over." ) );
	}
	ChooseInterviwee( );

}

void AHSW_ThirdPersonCharacter::ChooseInterviwee ( )
{

	if (IntervieweeIndex && IntervieweePlayerState)
	{
		if (bIsInterviewing)
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "플레이어 수: %d" ) , PlayerStates.Num ( ) );
			UE_LOG ( LogTemp , Warning , TEXT ( "선택된 수: %d" ) , IntervieweeIndex );
			UE_LOG ( LogTemp , Warning , TEXT ( "선택된 플레이어: %s" ) , *IntervieweePlayerState->GetPlayerName ( ) );

			IntervieweePlayerState->GetPawn ( )->SetActorTransform( InterviewLocation );

			DrawDebugString ( GetWorld ( ) , IntervieweePlayerState->GetPawn ( )->GetActorLocation ( ) + FVector ( 0 , 0 , 90 ) , TEXT ( "Interviewee~" ) , nullptr , FColor::Red , 5 , true , 1 );
		}
		else
		{
			IntervieweePlayerState->GetPawn ( )->SetActorTransform ( PreLocation );
		}

	}
}

// 오브젝트 생성하기 ==========================================================================================

void AHSW_ThirdPersonCharacter::OnMyThorwHold ( const FInputActionValue& value )
{
	ServerRPCThrowHold( );
}

void AHSW_ThirdPersonCharacter::ServerRPCThrowHold_Implementation ( )
{
	FTransform t = ThrowingArrow->GetComponentTransform ( );

	MulticastRPCThrowHold ( t );

}

void AHSW_ThirdPersonCharacter::MulticastRPCThrowHold_Implementation ( FTransform t )
{
	// 	FTransform t = ThrowingArrow->GetComponentTransform ( );
	ThrowingObject = GetWorld ( )->SpawnActor<AHSW_ThrowingObject> ( ThrowingObjectFactory , t );
	if (ThrowingObject)
	{
		ThrowingObject->AttachToComponent ( ThrowingArrow , FAttachmentTransformRules::KeepWorldTransform );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Object did not spawn" ) );
	}
}

// 오브젝트 던지기 ====================================================================================================

void AHSW_ThirdPersonCharacter::OnMyThorwPitch ( const FInputActionValue& value )
{
	ServerRPCThrowPitch( );
}

void AHSW_ThirdPersonCharacter::ServerRPCThrowPitch_Implementation ( )
{
	MulticastRPCThrowPitch ( );
}

void AHSW_ThirdPersonCharacter::MulticastRPCThrowPitch_Implementation ( )
{

	ThrowingObject->DetachFromActor ( FDetachmentTransformRules::KeepWorldTransform );
	ThrowingObject->MeshComp->SetSimulatePhysics ( true );

	FVector ThrowingForce = ThrowingArrow->GetForwardVector ( ) * ThrowingSpeed;
	ThrowingObject->MeshComp->AddForce ( ThrowingForce );
	ThrowingObject = nullptr;


	// 재장전 애니메이션 재생
	auto* anim = Cast<UHSW_AnimInstance_Audience> ( GetMesh ( )->GetAnimInstance ( ) );
	if (anim)
	{
		anim->PlayThrowMontage ( );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Throw Montage is null" ) );
	}
}
#pragma region KMK
void AHSW_ThirdPersonCharacter::InitializeAudienceWidget ( TSubclassOf<class UAudience_KMK>  widgetFact )
{
	if (!widgetFact) // 위젯이 nullptr인 경우에만 생성
	{
		if (widgetFact)
		{
			audienceWidget = CreateWidget<UAudience_KMK> ( GetWorld ( ) , widgetFact );
			audienceWidget->AddToViewport ( );
			audienceWidget->pc = this;
		}
		else
		{
			UE_LOG ( LogTemp , Error , TEXT ( "AudienceWidgetClass is not set." ) );
		}
	}
}
#pragma endregion
