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
    UFUNCTION(BlueprintCallable, Category = "Screenshot")
    void TakeScreenshot();

    int32 SavedWidth;
    int32 SavedHeight;
    class TArray<FColor> SavedImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* IMG_Screenshot;

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UJJH_MapSelectWidget> MapSelectWidgetFactory;
    class UJJH_MapSelectWidget* MapSelectWidget;
private:
    void OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Colors);

    UTexture2D* ConvertFColorArrayToTexture ( int32 Width , int32 Height , const TArray<FColor>& Image );
};
