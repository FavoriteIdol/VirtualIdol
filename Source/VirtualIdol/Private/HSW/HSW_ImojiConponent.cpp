// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_ImojiConponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"

// Sets default values for this component's properties
UHSW_ImojiConponent::UHSW_ImojiConponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	FString TexturePath = TEXT ( "'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_01'" );
	UTexture2D* LoadedTexture = Cast<UTexture2D> ( StaticLoadObject ( UTexture2D::StaticClass ( ) , nullptr , *TexturePath ) );
	ImojiBilboard->SetVisibility ( true);
	ImojiBilboard->SetSprite( LoadedTexture);
}

void UHSW_ImojiConponent::Imoji02 ( )
{
	FString TexturePath = TEXT ( "'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_02'" );
	UTexture2D* LoadedTexture = Cast<UTexture2D> ( StaticLoadObject ( UTexture2D::StaticClass ( ) , nullptr , *TexturePath ) );
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( LoadedTexture );
}

void UHSW_ImojiConponent::Imoji03 ( )
{
	FString TexturePath = TEXT ( "'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_03'" );
	UTexture2D* LoadedTexture = Cast<UTexture2D> ( StaticLoadObject ( UTexture2D::StaticClass ( ) , nullptr , *TexturePath ) );
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( LoadedTexture );
}

void UHSW_ImojiConponent::Imoji04 ( )
{
	FString TexturePath = TEXT ( "'/Game/Project/Personal/HSW/Resources/Imogi/Imoji_04'" );
	UTexture2D* LoadedTexture = Cast<UTexture2D> ( StaticLoadObject ( UTexture2D::StaticClass ( ) , nullptr , *TexturePath ) );
	ImojiBilboard->SetVisibility ( true );
	ImojiBilboard->SetSprite ( LoadedTexture );
}
