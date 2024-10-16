// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_ImojiConponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "Materials/Material.h"

// Sets default values for this component's properties
UHSW_ImojiConponent::UHSW_ImojiConponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// 이모지 텍스쳐 찾기
	static ConstructorHelpers::FObjectFinder<UTexture2D> LoadedImoji01Texture ( TEXT ( "Texture2D'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_01.Imoji_01'" ) );
	if (LoadedImoji01Texture.Succeeded ( ))
	{
		Imoji01Texture = LoadedImoji01Texture.Object;
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load texture Imoji_01" ) );
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> LoadedImoji02Texture ( TEXT ( "Texture2D'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_02.Imoji_02'" ) );
	if (LoadedImoji02Texture.Succeeded ( ))
	{
		Imoji02Texture = LoadedImoji02Texture.Object;
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load texture Imoji_02" ) );
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> LoadedImoji03Texture ( TEXT ( "Texture2D'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_03.Imoji_03'" ) );
	if (LoadedImoji03Texture.Succeeded ( ))
	{
		Imoji03Texture = LoadedImoji03Texture.Object;
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load texture Imoji_03" ) );
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> LoadedImoji04Texture ( TEXT ( "Texture2D'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_04.Imoji_04'" ) );
	if (LoadedImoji04Texture.Succeeded ( ))
	{
		Imoji04Texture = LoadedImoji04Texture.Object;
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load texture Imoji_04" ) );
	}




// 	// FadeIn, Out에 쓸 Curve 정의
// 	static ConstructorHelpers::FObjectFinder<UCurveFloat> LoadedFadeInCurve(TEXT("'/Game/Project/Personal/HSW/Curves/HSW_C_FadeInCurve'" ) );
// 	if (LoadedFadeInCurve.Succeeded ( ))
// 	{
// 		FadeInCurve = LoadedFadeInCurve.Object;
// 	}
// 	else
// 	{
// 		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load Fade In Curve" ) );
// 	}
// 
// 	static ConstructorHelpers::FObjectFinder<UCurveFloat> LoadedFadeOutCurve ( TEXT ( "'/Game/Project/Personal/HSW/Curves/HSW_C_FadeOutCurve'" ) );
// 	if (LoadedFadeOutCurve.Succeeded ( ))
// 	{
// 		FadeOutCurve = LoadedFadeOutCurve.Object;
// 	}
// 	else
// 	{
// 		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to load Fade Out Curve" ) );
// 	}
}


// Called when the game starts
void UHSW_ImojiConponent::BeginPlay()
{
	Super::BeginPlay();

	// 빌보드 컴포넌트
	ImojiBillboard = Cast<UBillboardComponent> ( GetChildComponent ( 0 )->GetChildComponent ( 0 ) );
	ImojiBillboard->SetVisibility ( false );

	Me = Cast<AHSW_ThirdPersonCharacter> ( GetOwner ( ) );


// 	// 타임라인
// 	FOnTimelineFloat onTimelineCallback;
// 	FOnTimelineEventStatic onTimelineFinishedCallback;
// 
// 	if (FadeInCurve != NULL)
// 	{
// 		FadeInTimeline = NewObject<UTimelineComponent>(this , FName ( "FadeInTimelineAnimation" ) );
// 
// 	}

}


// Called every frame
void UHSW_ImojiConponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHSW_ImojiConponent::Imoji01 ( )
{	
	//GEngine->AddOnScreenDebugMessage ( -1 , 2.0f , FColor::Red , TEXT ( "Imoji01" ) );
	AppearImoji ( Imoji01Texture );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandle );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , this , &UHSW_ImojiConponent::DisappearImoji,  2.0f );

// 	if (FindFunction ( TEXT ( "FadeInImogiEvent" ) ))
// 	{
// 		ProcessEvent ( FindFunction ( TEXT ( "FadeInImogiEvent" ) ) , nullptr );
// 	}

}

void UHSW_ImojiConponent::Imoji02 ( )
{
	AppearImoji ( Imoji02Texture );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandle );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , this , &UHSW_ImojiConponent::DisappearImoji , 2.0f );
}

void UHSW_ImojiConponent::Imoji03 ( )
{
	AppearImoji ( Imoji03Texture );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandle );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , this , &UHSW_ImojiConponent::DisappearImoji , 2.0f );
}

void UHSW_ImojiConponent::Imoji04 ( )
{
	AppearImoji(Imoji04Texture );

	GetWorld ( )->GetTimerManager ( ).ClearTimer ( TimerHandle );
	GetWorld ( )->GetTimerManager ( ).SetTimer ( TimerHandle , this , &UHSW_ImojiConponent::DisappearImoji , 2.0f );
}

void UHSW_ImojiConponent::AppearImoji ( UTexture2D* imojiTexture )
{
	ImojiBillboard->SetSprite ( imojiTexture );
	ImojiBillboard->SetVisibility(true);
	//Me->FadeInImogiBlueprint ( );
}

void UHSW_ImojiConponent::DisappearImoji ( )
{
	ImojiBillboard->SetSprite (nullptr );
}


