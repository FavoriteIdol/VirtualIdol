// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH_SetupPlayerController.h"
#include "Components/Image.h"
#include "JJH/JJH_MapSelectWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "ImageCore.h"
#include "IImageWrapper.h"

void AJJH_SetupPlayerController::BeginPlay ( )
{
	MapSelectWidget = CreateWidget<UJJH_MapSelectWidget> ( this , MapSelectWidgetFactory );
	if (MapSelectWidget) MapSelectWidget->AddToViewport ( );

	// SceneCaptureComponent2D를 생성
	CaptureComponent2D = NewObject<USceneCaptureComponent2D> ( this );
	CaptureComponent2D->RegisterComponent ( );
	CaptureComponent2D->SetWorldLocation ( GetPawn ( )->GetActorLocation ( ) ); // 플레이어 위치 기준으로 캡처

	// 렌더 타겟 생성
	RenderTarget = NewObject<UTextureRenderTarget2D> ( );
	RenderTarget->InitAutoFormat ( 1920 , 1080 ); // 원하는 해상도 설정
	RenderTarget->ClearColor = FLinearColor::Black; // 초기화 색상 설정

	// CaptureComponent에 렌더 타겟 설정
	CaptureComponent2D->TextureTarget = RenderTarget;
	CaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR; // 최종 색상 캡처


	//마우스 보여주고 인풋모드 바꾸기
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode ( InputMode );
}
void AJJH_SetupPlayerController::TakeScreenshot2 ( )
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
		SaveImage( RenderTarget );

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

UTexture2D* AJJH_SetupPlayerController::RenderTargetToTexture2D ( UTextureRenderTarget2D* InRenderTarget )
{
	if (!InRenderTarget) return nullptr;

	FTextureRenderTargetResource* RenderTargetResource = InRenderTarget->GameThread_GetRenderTargetResource ( );

	TArray<FColor> OutBMP;
	FIntRect SourceRect ( 0 , 0 , InRenderTarget->SizeX , InRenderTarget->SizeY );
	RenderTargetResource->ReadPixels ( OutBMP , FReadSurfaceDataFlags ( ) , SourceRect );

	UTexture2D* NewTexture = UTexture2D::CreateTransient ( InRenderTarget->SizeX , InRenderTarget->SizeY , PF_B8G8R8A8 );
	if (!NewTexture) return nullptr;

	FTexture2DMipMap& Mip = NewTexture->GetPlatformData ( )->Mips[0];
	void* TextureData = Mip.BulkData.Lock ( LOCK_READ_WRITE );
	FMemory::Memcpy ( TextureData , OutBMP.GetData ( ) , OutBMP.Num ( ) * sizeof ( FColor ) );
	Mip.BulkData.Unlock ( );

	NewTexture->UpdateResource ( );
	return NewTexture;
}


void AJJH_SetupPlayerController::SaveImage( UTextureRenderTarget2D* RenderTarget2 )
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
	FString FullFileName = ScreenshotPath / FileName;

	// 디렉토리 생성 (없을 경우)
	IFileManager::Get ( ).MakeDirectory ( *ScreenshotPath , true );

	// 이미지를 저장 (확장자에 따라 자동으로 포맷 선택)
	bool bSuccess = FImageUtils::SaveImageByExtension ( *FullFileName , ImageView );

	if (bSuccess)
	{
		UE_LOG ( LogTemp , Log , TEXT ( "Screenshot saved successfully to: %s" ) , *FullFileName );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed to save screenshot to: %s" ) , *FullFileName );
	}
}







void AJJH_SetupPlayerController::TakeScreenshot ( )
{
    FString ScreenshotPath = FPaths::ProjectSavedDir ( ) / TEXT ( "Screenshots" );
    FString FileName = FDateTime::Now ( ).ToString ( TEXT ( "%Y%m%d_%H%M%S" ) );
    FString FullFileName = ScreenshotPath / FileName + TEXT ( ".png" );

    FScreenshotRequest::RequestScreenshot ( FullFileName , true , false );
	GEngine->GameViewport->Viewport->TakeHighResScreenShot ( );
    // 스크린샷 캡처 완료 후 호출될 델리게이트 설정
    UGameViewportClient::OnScreenshotCaptured ( ).Clear ( );
    UGameViewportClient::OnScreenshotCaptured ( ).AddUObject ( this , &AJJH_SetupPlayerController::OnScreenshotCaptured );
}


void AJJH_SetupPlayerController::OnScreenshotCaptured ( int32 Width , int32 Height , const TArray<FColor>& Image)
{
	SavedWidth = Width;
	SavedHeight = Height;
	SavedImage = Image;
	UTexture2D* CaptureTexture = ConvertFColorArrayToTexture ( Width , Height , Image );
	if (CaptureTexture && MapSelectWidget)
	{
		if (MapSelectWidget->CapturedImage)
		{
			MapSelectWidget->CapturedImage->SetBrushFromTexture ( CaptureTexture , true );
		}
		else
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "OnScreenshotCaptured: MapSelectWidget->IMG_Screenshot is not valid" ) );
		}
	}
	//일단 여기서 이미지셋
}


UTexture2D* AJJH_SetupPlayerController::ConvertFColorArrayToTexture ( int32 Width , int32 Height , const TArray<FColor>& Image )
{
	UTexture2D* CaptureTexture = UTexture2D::CreateTransient ( Width , Height , PF_B8G8R8A8 );
	if (nullptr == CaptureTexture)
	{
		return nullptr;
	}

	if (nullptr == CaptureTexture->GetPlatformData())
	{
		return nullptr;
	}

	if (false == CaptureTexture->GetPlatformData ( )->Mips.IsValidIndex ( 0 ))
	{
		return nullptr;
	}

	// Copy!
	FTexture2DMipMap& Mip = CaptureTexture->GetPlatformData ( )->Mips[0];
	void* TextureData = Mip.BulkData.Lock ( LOCK_READ_WRITE );
	FMemory::Memcpy ( TextureData , Image.GetData ( ) , Image.Num ( ) * 4 );
	Mip.BulkData.Unlock ( );

	// Update!
	CaptureTexture->UpdateResource ( );
	return CaptureTexture;
}

