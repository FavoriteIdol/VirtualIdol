// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK/HttpActor_KMK.h"
#include "JJH_SelectManager.generated.h"

UCLASS()
class VIRTUALIDOL_API AJJH_SelectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJJH_SelectManager();


	class AJJH_SetupPlayerController* JJHPC;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//태양 위치 바꾸기
    UFUNCTION(BlueprintCallable, Category = "Time")
    void UpdateSunNightPosition(int32 index);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> SkyFactory;

	//레벨 바꾸기
	UFUNCTION ()
	void ChangeMap(int32 index);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ThemeFactory;

	void FindActorAndDestroy (FName tag);
	//이펙트 바꾸기
	UFUNCTION( )
	void ChangeEffect(int32 index);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> VFXFactory;

	//지면 바꾸기

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> FloorFactory;

	void ChangeFloor (int32 index );


	//스크린샷
	UPROPERTY ( EditAnywhere )
	TSubclassOf<class UJJH_MapSelectWidget> MapSelectWidgetFactory;
	class UJJH_MapSelectWidget* MapSelectWidget;

	class USceneCaptureComponent2D* CaptureComponent2D;

	void SetName ( const FString Text );

	void TakeScreenshot ( );

	UTexture2D* RenderTargetToTexture2D ( UTextureRenderTarget2D* InRenderTarget );

	void SaveImage ( UTextureRenderTarget2D* RenderTarget2 );

	// 렌더 타겟 생성
	class UTextureRenderTarget2D* RenderTarget;

	//백엔드 보내기 위한 구조체
	FStageInfo Stage;
	//파일 경로
	FString FullFileName;


	//레벨 배열
	TArray<FName> Levels = {
		FName ( "LV_Theme-Island_JSW" ),
		FName ( "BP_Theme-City" ),
		FName ( "BP_Theme-Dystopia_JSW" ),
		FName ( "LV_Forest_JSW" ),
		FName ( "LV_Theme-Winter_JSW" )
	};

	FName LevelToLoad;
	TArray<FName> LevelsToUnload;


#pragma region LoadStage
	void CreateStage(const struct FStageInfo& info );
	UFUNCTION( )
	void DeleteStage( );
#pragma endregion

};
