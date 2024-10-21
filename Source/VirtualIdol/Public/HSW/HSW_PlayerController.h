// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HSW_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AHSW_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AHSW_AuditoriumGameMode* gm;

	UFUNCTION(Server, Reliable )
	void ServerRPCResapwnPlayer ();

public:
	UPROPERTY(EditDefaultsOnly, Category = UI )
	TSubclassOf<class UHSW_MainWidget> mainUIWidget;

	// mainUIWidget으로 부터 만들어진 인스턴스
	UPROPERTY()
	class UHSW_MainWidget* MainUI;
};
