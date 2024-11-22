// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InterviewAction;

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
	
	void OnMyThorwHold ( const FInputActionValue& value );

	void OnMyThorwPitch ( const FInputActionValue& value );

	void OnMyInterview ( const FInputActionValue& value );


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom ( ) const { return CameraBoom; }
	//FORCEINLINE void SetCameraBoom ( float value) { CameraBoom->TargetArmLength = value; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera ( ) const { return FollowCamera; }

public:
	UPROPERTY(EditDefaultsOnly )
	class AHSW_GameState_Auditorium* gs;

	UPROPERTY(EditDefaultsOnly )
	class AHSW_AuditoriumGameMode* gm;

	FTransform StageLocation;

	UFUNCTION(Client, Reliable)
	void ClientPlayMusic( class UAudioComponent* selectedMusic );

	UPROPERTY( )
	class USoundBase* soundFile;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USoundBase* AmbientSound01;
	UPROPERTY( EditDefaultsOnly)
	class UAudioComponent* AmbientSoundComponent01;

	// 피버게이지 --------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly , Category = FeverGauge )
	class UHSW_FeverGaugeWidget* FeverGauge;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float CurrentGauge = 0.0f;

	UFUNCTION( )
	void OnRep_FeverGauge ();

	UFUNCTION( )
	void PrintFeverGaugeLogOnHead( );

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FeverGauge )
	float FeverPoint = 0.005f;

	void SetFeverGaugeMulti(float feverValue);

	UFUNCTION(BlueprintImplementableEvent, Category= Imogi )
	void ShakeBodyBlueprint( );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastFeverEffect( );

	UPROPERTY(Replicated )
	bool bFever;

	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	class UParticleSystem* FeverEffect_Particle;

	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	class UNiagaraSystem* FeverEffect_Niagara;

	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	class AActor* FeverEffect_Actor;

	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	TSubclassOf<class AActor> FeverEffectFactory;

	FTransform FeverEffectLocation;

	UPROPERTY( Replicated, EditDefaultsOnly , BlueprintReadWrite , Category = FeverGauge )
	float FeverBright = 1.0f;

	int32 PersonalGauge = 0;

	UPROPERTY( EditDefaultsOnly , BlueprintReadWrite , Category = FeverGauge )
	TArray<UMaterialInstance*> FeverCharactMat;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = FeverGauge )
	TArray< UMaterialInstanceDynamic*> FeverDynamicMats;

	UPROPERTY( )
	UMaterialInstanceDynamic* FeverDynamicMat;

    UFUNCTION ( )
    UMaterialInstanceDynamic* ChangeMyMeshMat ( int32 num = 0 );

	UFUNCTION(Server, Reliable, BlueprintCallable )
	void ServerFeverReset( );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastFeverReset( );

	// MainWidget을 생성해서 기억하고싶다.
// 	UPROPERTY(EditDefaultsOnly, Category = MainUI)
// 	TSubclassOf<class UUserWidget> MainUIFactory;

// 	UPROPERTY(EditDefaultsOnly )
// 	class UHSW_MainWidget* MainUI;
// 
//     UFUNCTION ( )
//     void InitMainUI ( );

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

	void AppearImoji ( );

	void DisappearImoji ();

	FTimerHandle TimerHandleImoji;

	UPROPERTY( )
	class UHSW_ImogiWidget* imojiWidget;

	UPROPERTY ( EditDefaultsOnly, Category = Imoji )
	class UNiagaraSystem* EmojiEffect;

	UFUNCTION(BlueprintImplementableEvent, Category= Imogi )
	void FadeInImogiBlueprint( );


	// 3D 오브젝트 --------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	class UArrowComponent* ThrowingArrow;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite )
	class AHSW_ThrowingObject* ThrowingObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite )
	TSubclassOf<class AHSW_ThrowingObject> ThrowingObjectFactory;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite )
	float ThrowingSpeed = 8000000.0f;

	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
	bool bThrowing;

	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
	FRotator ThrowingRotator;

	UPROPERTY( Replicated, EditDefaultsOnly , BlueprintReadWrite )
	int32 ThrowingObjectIndex = 0;

	// 인터뷰 ----------------------------------------

	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
	bool bIsInterviewing;

	UFUNCTION( )
	void ChooseInterviwee( );

	UPROPERTY ( Replicated , EditDefaultsOnly , BlueprintReadWrite )
	int32 IntervieweeIndex;
	
	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
	class APlayerState* IntervieweePlayerState;

	UPROPERTY( )
	TArray<class APlayerState*> PlayerStates;

	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadWrite )
	FTransform PreLocation;

	UFUNCTION(BlueprintImplementableEvent, Category= Interview )
	void MICSettingBlueprint( );

	UFUNCTION(BlueprintImplementableEvent, Category= Interview )
	void MICOnBlueprint( );

	UFUNCTION(BlueprintImplementableEvent, Category= Interview )
	void MICOffBlueprint( );

	UFUNCTION( )
	void SetInterviewee( bool bInterview, APlayerState* interviewee, FTransform preLoc );
	// 멀티플레이 --------------------------------------

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

	UFUNCTION(Server, Reliable )
	void ServerRPCThrowHold( FTransform t );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCThrowHold( FTransform t );

	UFUNCTION(Server, Reliable )
	void ServerRPCThrowPitch ( );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCThrowPitch();

	UFUNCTION(Server, Reliable )
	void ServerRPCFeverGauge( float feverValue, float brightValue );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCFeverGauge (float AddGauge, float brightValue );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCBrightness ( int index );

	virtual void PossessedBy ( AController* NewController ) override;

	UFUNCTION(Server, Reliable )
	void ServerRPCInterview();

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCInterview (  );

	UFUNCTION(Server, Reliable )
	void ServerRPCImoji( int index );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCImoji ( int index );

	void StartVoiceChat( );
	void CancleVoiceChat ( );

	UFUNCTION(Server, Reliable )
	void ServerRPCPlayMusic(  );

	UFUNCTION(NetMulticast, Reliable )
	void MulticastRPCPlayMusic ( );

	UFUNCTION( )
	void PlayMusic(USoundBase* wavFile);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<class USoundBase* > WavArray;


#pragma region KMK

	UPROPERTY( )
	class APlayerController* pc;

	UPROPERTY(EditAnywhere )
	TSubclassOf<class UAudience_KMK> audienceWidgetFact;
	UPROPERTY(EditAnywhere )
	TSubclassOf<class UAudience_KMK> virtualWidgetFact;

	UPROPERTY( )
	class UAudience_KMK* audienceWidget;

	UPROPERTY( )
	class UAudience_KMK* virtualWidget;
	
	
	UPROPERTY(VisibleAnywhere)
	class UAudienceServerComponent_KMK* serverComp;

	UFUNCTION( )
	void InitializeAudienceWidget( TSubclassOf<class UAudience_KMK>  widgetFact );

	UFUNCTION( )
	void SetChatWidget(const FString& nickName, const FString& text );
#pragma endregion
	int32 effectNum = -1;

};
