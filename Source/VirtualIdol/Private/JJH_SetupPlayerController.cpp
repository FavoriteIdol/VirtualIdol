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
	//마우스 보여주고 인풋모드 바꾸기
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode ( InputMode );
	
}

#pragma region full screenshot
//
//void AJJH_SetupPlayerController::TakeScreenshot ( )
//{
//    FString ScreenshotPath = FPaths::ProjectSavedDir ( ) / TEXT ( "Screenshots" );
//    FString FileName = FDateTime::Now ( ).ToString ( TEXT ( "%Y%m%d_%H%M%S" ) );
//    FString FullFileName = ScreenshotPath / FileName + TEXT ( ".png" );
//
//    FScreenshotRequest::RequestScreenshot ( FullFileName , true , false );
//	GEngine->GameViewport->Viewport->TakeHighResScreenShot ( );
//    // 스크린샷 캡처 완료 후 호출될 델리게이트 설정
//    UGameViewportClient::OnScreenshotCaptured ( ).Clear ( );
//    UGameViewportClient::OnScreenshotCaptured ( ).AddUObject ( this , &AJJH_SetupPlayerController::OnScreenshotCaptured );
//}
//
//
//void AJJH_SetupPlayerController::OnScreenshotCaptured ( int32 Width , int32 Height , const TArray<FColor>& Image)
//{
//	SavedWidth = Width;
//	SavedHeight = Height;
//	SavedImage = Image;
//	UTexture2D* CaptureTexture = ConvertFColorArrayToTexture ( Width , Height , Image );
//	if (CaptureTexture && MapSelectWidget)
//	{
//		if (MapSelectWidget->CapturedImage)
//		{
//			MapSelectWidget->CapturedImage->SetBrushFromTexture ( CaptureTexture , true );
//		}
//		else
//		{
//			UE_LOG ( LogTemp , Warning , TEXT ( "OnScreenshotCaptured: MapSelectWidget->IMG_Screenshot is not valid" ) );
//		}
//	}
//	//일단 여기서 이미지셋
//}
//
//
//UTexture2D* AJJH_SetupPlayerController::ConvertFColorArrayToTexture ( int32 Width , int32 Height , const TArray<FColor>& Image )
//{
//	UTexture2D* CaptureTexture = UTexture2D::CreateTransient ( Width , Height , PF_B8G8R8A8 );
//	if (nullptr == CaptureTexture)
//	{
//		return nullptr;
//	}
//
//	if (nullptr == CaptureTexture->GetPlatformData())
//	{
//		return nullptr;
//	}
//
//	if (false == CaptureTexture->GetPlatformData ( )->Mips.IsValidIndex ( 0 ))
//	{
//		return nullptr;
//	}
//
//	// Copy!
//	FTexture2DMipMap& Mip = CaptureTexture->GetPlatformData ( )->Mips[0];
//	void* TextureData = Mip.BulkData.Lock ( LOCK_READ_WRITE );
//	FMemory::Memcpy ( TextureData , Image.GetData ( ) , Image.Num ( ) * 4 );
//	Mip.BulkData.Unlock ( );
//
//	// Update!
//	CaptureTexture->UpdateResource ( );
//	return CaptureTexture;
//}

#pragma endregion
