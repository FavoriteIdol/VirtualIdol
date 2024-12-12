// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK/HttpActor_KMK.h"
#include "HSW_SongUnit.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UHSW_SongUnit : public UUserWidget
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
	class UTextBlock* Text_SongTitle;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Play;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Stop;

	FWavFileInfo SongInfo;

	UFUNCTION( )
	void MusicStart( );
	UFUNCTION( )
	void MusicStop( );
	UFUNCTION( )
	void FindVirtualCharacter( );
	UFUNCTION( )
	void SetSongTitle( );

	

};
