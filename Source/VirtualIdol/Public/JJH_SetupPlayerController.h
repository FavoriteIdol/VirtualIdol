// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JJH_SetupPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API AJJH_SetupPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

    virtual void BeginPlay() override;

private:
	//UFUNCTION ( BlueprintCallable , Category = "Screenshot" )
	//void TakeScreenshot ( );

 //   void OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Colors);

 //   UTexture2D* ConvertFColorArrayToTexture ( int32 Width , int32 Height , const TArray<FColor>& Image );
};
