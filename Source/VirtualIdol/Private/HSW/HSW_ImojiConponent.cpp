// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_ImojiConponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UHSW_ImojiConponent::UHSW_ImojiConponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
}


// Called when the game starts
void UHSW_ImojiConponent::BeginPlay()
{
	Super::BeginPlay();
	ImojiBilboard = Cast<UBillboardComponent>(GetChildComponent(0));
	ImojiBilboard->SetVisibility(false);
	ImojiBilboard->SetSprite(nullptr);

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
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( Imoji01Texture); // Get() 대신 Object 사용

}

void UHSW_ImojiConponent::Imoji02 ( )
{
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( Imoji02Texture );
}

void UHSW_ImojiConponent::Imoji03 ( )
{

	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( Imoji03Texture );
}

void UHSW_ImojiConponent::Imoji04 ( )
{
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( Imoji04Texture );
}
