﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_SelectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "JJH/JJH_MapSelectWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "Components/Image.h"

// Sets default values
AJJH_SelectManager::AJJH_SelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJJH_SelectManager::BeginPlay()
{
	Super::BeginPlay();

	MapSelectWidget = CreateWidget<UJJH_MapSelectWidget> ( GetWorld() , MapSelectWidgetFactory );
	if (MapSelectWidget) MapSelectWidget->AddToViewport ( );

	// SceneCaptureComponent2D를 생성
	CaptureComponent2D = NewObject<USceneCaptureComponent2D> ( this );
	CaptureComponent2D->RegisterComponent ( );
	CaptureComponent2D->SetWorldLocation (GetWorld()->GetFirstPlayerController()->GetPawn ( )->GetActorLocation ( )); // 플레이어 위치 기준으로 캡처

	// 렌더 타겟 생성
	RenderTarget = NewObject<UTextureRenderTarget2D> ( );
	RenderTarget->InitAutoFormat ( 1920 , 1080 ); // 원하는 해상도 설정
	RenderTarget->ClearColor = FLinearColor::Black; // 초기화 색상 설정

	// CaptureComponent에 렌더 타겟 설정
	CaptureComponent2D->TextureTarget = RenderTarget;
	CaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR; // 최종 색상 캡처


}

// Called every frame
void AJJH_SelectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJJH_SelectManager::UpdateSunNightPosition ( bool isNight )
{
    FindActorAndDestroy ( TEXT ( "Skybox" ) );
	if (isNight)
	{
		GetWorld()->SpawnActor<AActor>(SkyFactory[0], GetActorTransform ( ) );
		Stage.sky = 0;
	}
	else
	{
        GetWorld()->SpawnActor<AActor>( SkyFactory[1] , GetActorTransform ());
		Stage.sky = 1;
	}
}

void AJJH_SelectManager::ChangeMap ( int32 index )
{
	//케이스 나누어서 하기
    FindActorAndDestroy(TEXT("Theme"));
	Stage.theme = index;
    GetWorld ( )->SpawnActor<AActor>(ThemeFactory[index] , GetActorTransform ( ) );

}

void AJJH_SelectManager::FindActorAndDestroy(FName tag)
{
    TArray<AActor*> ActorToDestroy;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), tag , ActorToDestroy);
    if (!ActorToDestroy.IsEmpty ())
    {
		for (AActor* Actor : ActorToDestroy)
		{
			if (Actor) Actor->Destroy ( );
		}
    }
}

void AJJH_SelectManager::ChangeEffect ( int32 index )
{
    FindActorAndDestroy ( TEXT ( "VFX" ) );
	Stage.specialEffect = index;
    GetWorld ( )->SpawnActor<AActor> (VFXFactory[index] , GetActorTransform ( ) );

}

void AJJH_SelectManager::ChangeFloor (int32 index)
{
    FindActorAndDestroy ( TEXT ( "Floor" ) );
	Stage.terrain = index;
    GetWorld ( )->SpawnActor<AActor> (FloorFactory[index] , GetActorTransform ( ) );
}

void AJJH_SelectManager::SetName (const FString Text )
{
	Stage.name = Text;
}

#pragma region ScreenShot 
void AJJH_SelectManager::TakeScreenshot ( )
{
	if (CaptureComponent2D && RenderTarget)
	{
		// 화면을 캡처
		CaptureComponent2D->CaptureScene ( );

		// 렌더 타겟을 텍스처로 변환
		UTexture2D* CaptureTexture = RenderTargetToTexture2D ( RenderTarget );

		// 파일명 생성 (현재 날짜와 시간)
		FString FileName = FDateTime::Now ( ).ToString ( TEXT ( "%Y%m%d_%H%M%S" ) );

		// 캡처한 텍스처를 파일로 저장
		SaveImage ( RenderTarget );

		// 캡처한 이미지를 위젯에 적용
		if (CaptureTexture && MapSelectWidget)
		{
			if (MapSelectWidget->CapturedImage)
			{
				MapSelectWidget->CapturedImage->SetBrushFromTexture ( CaptureTexture , true );
			}

			if (MapSelectWidget->CapturedImage_1)
			{
				MapSelectWidget->CapturedImage_1->SetBrushFromTexture ( CaptureTexture , true );
			}
		}
	}

}

UTexture2D* AJJH_SelectManager::RenderTargetToTexture2D ( UTextureRenderTarget2D* InRenderTarget )
{

	// 입력된 RenderTarget이 유효하지 않으면 함수 종료 (nullptr 반환)
	if (!InRenderTarget) return nullptr;

	// RenderTarget의 리소스를 가져옴 (렌더링된 장면의 데이터를 얻기 위해)
	FTextureRenderTargetResource* RenderTargetResource = InRenderTarget->GameThread_GetRenderTargetResource ( );

	// 화면 픽셀 데이터를 저장할 배열을 생성
	TArray<FColor> OutBMP;

	// 캡처할 영역을 설정 (전체 텍스처 영역을 대상으로 설정)
	FIntRect SourceRect ( 0 , 0 , InRenderTarget->SizeX , InRenderTarget->SizeY );

	// 렌더 타겟에서 픽셀 데이터를 읽어와서 OutBMP 배열에 저장
	RenderTargetResource->ReadPixels ( OutBMP , FReadSurfaceDataFlags ( ) , SourceRect );

	// 새로운 텍스처 생성 (RenderTarget과 동일한 크기, PF_B8G8R8A8 포맷으로 임시 텍스처 생성)
	UTexture2D* NewTexture = UTexture2D::CreateTransient ( InRenderTarget->SizeX , InRenderTarget->SizeY , PF_B8G8R8A8 );

	// 텍스처 생성에 실패하면 함수 종료 (nullptr 반환)
	if (!NewTexture) return nullptr;

	// 텍스처의 첫 번째 MipMap을 가져옴 (여러 해상도를 지원하는 텍스처의 첫 레벨 데이터를 의미)
	FTexture2DMipMap& Mip = NewTexture->GetPlatformData ( )->Mips[0];

	// 텍스처 데이터를 수정할 수 있도록 잠금 (LOCK_READ_WRITE로 읽기/쓰기가 가능하도록 잠금)
	void* TextureData = Mip.BulkData.Lock ( LOCK_READ_WRITE );

	// 렌더 타겟에서 읽어온 픽셀 데이터를 텍스처 데이터에 복사
	FMemory::Memcpy ( TextureData , OutBMP.GetData ( ) , OutBMP.Num ( ) * sizeof ( FColor ) );

	// 잠금 해제 (텍스처 데이터 수정을 완료)
	Mip.BulkData.Unlock ( );

	// 텍스처 리소스를 업데이트 (변경 사항을 엔진에 적용)
	NewTexture->UpdateResource ( );

	// 변환된 텍스처 반환
	return NewTexture;
}


void AJJH_SelectManager::SaveImage ( UTextureRenderTarget2D* RenderTarget2 )
{
	if (!RenderTarget2) return;

	// 텍스처에서 이미지를 가져오기 위한 리드 백
	FTextureRenderTargetResource* RenderTargetResource = RenderTarget2->GameThread_GetRenderTargetResource ( );

	// 이미지 데이터를 저장할 TArray
	TArray<FColor> Bitmap;
	RenderTargetResource->ReadPixels ( Bitmap );

	// 이미지 크기 설정 및 FImageView 생성
	FImageView ImageView (
		(void*)Bitmap.GetData ( ) ,                      // 이미지 데이터
		RenderTarget2->GetSurfaceWidth ( ) ,              // 이미지 너비
		RenderTarget2->GetSurfaceHeight ( ) ,             // 이미지 높이
		ERawImageFormat::BGRA8                        // 픽셀 형식 (BGRA8)
	);

	// 저장할 경로 설정
	FString ScreenshotPath = FPaths::ProjectSavedDir ( ) / TEXT ( "Screenshots" );
	FString FileName = FDateTime::Now ( ).ToString ( TEXT ( "%Y%m%d_%H%M%S" ) ) + TEXT ( ".png" );
	FullFileName = ScreenshotPath / FileName;

	// 디렉토리 생성 (없을 경우)
	IFileManager::Get ( ).MakeDirectory ( *ScreenshotPath , true );

	// 이미지를 저장 (확장자에 따라 자동으로 포맷 선택)
	bool bSuccess = FImageUtils::SaveImageByExtension ( *FullFileName , ImageView );
	
	FString ScreenshotFilePath;

	if (bSuccess)
	{
		UE_LOG ( LogTemp , Log , TEXT ( "Screenshot saved successfully to: %s" ) , *FullFileName );
		//스크린샷 경로를 저장
		// 저장된 파일을 Base64로 인코딩
		TArray<uint8> ImageData;
		FFileHelper::LoadFileToArray ( ImageData , *FullFileName );
		FString Base64Image = FBase64::Encode ( ImageData );

		// StageInfo에 Base64 인코딩된 이미지 데이터 할당
		Stage.img = "TestImage";
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to save screenshot to: %s" ) , *FullFileName );
	}
}


#pragma endregion 