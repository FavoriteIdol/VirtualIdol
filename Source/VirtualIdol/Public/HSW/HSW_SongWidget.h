// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HSW_SongWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UHSW_SongWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	class AHttpActor_KMK* httpActor;

	UPROPERTY( )
	class UVirtual_KMK* VirtualCharacter;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_CurrentSongTitle;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Play;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Next;

	UFUNCTION( )
	void MusicStart( );
	UFUNCTION( )
	void SetMusic( );
	UFUNCTION( )
	void FindVirtualCharacter( );

	

};
