// Fill out your copyright notice in the Description page of Project Settings.


#include "JJH/JJH_SelectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "JJH/JJH_MapSelectWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "Components/Image.h"
#include "JJH_SetupPlayerController.h"

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

	if(!MapSelectWidgetFactory) return;
	MapSelectWidget = CreateWidget<UJJH_MapSelectWidget> ( GetWorld() , MapSelectWidgetFactory );
	if (MapSelectWidget) MapSelectWidget->AddToViewport ( );

	// SceneCaptureComponent2D를 생성
	CaptureComponent2D = NewObject<USceneCaptureComponent2D> ( this );
	CaptureComponent2D->RegisterComponent ( );
	JJHPC = Cast<AJJH_SetupPlayerController>( GetWorld ( )->GetFirstPlayerController ( ) );

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
	if (JJHPC)
	{
		// 카메라 매니저에서 현재 카메라 위치와 회전 가져오기
		FVector CameraLocation = JJHPC->PlayerCameraManager->GetCameraLocation ( );
		FRotator CameraRotation = JJHPC->PlayerCameraManager->GetCameraRotation ( );

		// CaptureComponent2D의 위치와 회전 설정
		CaptureComponent2D->SetWorldLocation ( CameraLocation );
		CaptureComponent2D->SetWorldRotation ( CameraRotation );
	}
}

void AJJH_SelectManager::UpdateSunNightPosition (int32 index)
{
    FindActorAndDestroy ( TEXT ( "Skybox" ) );

	GetWorld()->SpawnActor<AActor>(SkyFactory[index], GetActorTransform ( ) );
	Stage.sky = index;

}

void AJJH_SelectManager::ChangeMap ( int32 index )
{
	// 이전 테마 액터 파괴
	FindActorAndDestroy ( TEXT ( "Theme" ) );
	Stage.theme = index;

	// 인덱스에 따라 로드할 레벨 결정
	if (index >= 1 && index <= Levels.Num ( ))
	{
		LevelToLoad = Levels[index - 1]; // 인덱스가 1부터 시작하므로 조정

		// 로드 중인 레벨을 제외한 모든 레벨을 언로드 목록에 추가
		for (int32 i = 0; i < Levels.Num ( ); i++)
		{
			if (i != index - 1)
			{
				LevelsToUnload.Add ( Levels[i] );
			}
		}
	}
	else
	{
		// 인덱스가 범위를 벗어나면 새로운 테마 액터를 스폰하고 바로 반환
		GetWorld ( )->SpawnActor<AActor> ( ThemeFactory[index] , GetActorTransform ( ) );
		return;
	}

	// 선택된 레벨 로드
	UGameplayStatics::LoadStreamLevel (
		GetWorld ( ) ,
		LevelToLoad ,
		true ,  // 로드 후 보이게 설정
		true ,  // 로드 시 블록 여부 설정
		FLatentActionInfo ( )
	);

	// 일정 시간 후 이전 레벨을 개별적으로 언로드하는 타이머 설정
	for (int32 i = 0; i < LevelsToUnload.Num ( ); ++i)
	{
		const FName Level = LevelsToUnload[i];
		FTimerHandle UnloadTimerHandle;

		GetWorld ( )->GetTimerManager ( ).SetTimer ( UnloadTimerHandle , [this , Level]( )
		{
				UGameplayStatics::UnloadStreamLevel (
					GetWorld ( ) ,
					Level ,
					FLatentActionInfo ( ) ,
					true  // 언로드 시 블록 여부 설정
				);
				UE_LOG ( LogTemp , Warning , TEXT ( "Unloading level: %s" ) , *Level.ToString ( ) );

		} , 0.2f * ( i + 1 ) , false );  // 첫 번째 레벨도 0.4초 지연 후 언로드 <- 비동기적이라 이거 해야함
	}
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


void AJJH_SelectManager::CreateStage ( const struct FStageInfo& info )
{
	GetWorld()->SpawnActor<AActor>(SkyFactory[info.sky], GetActorTransform ( ) );
	
	//if (info.theme == 1)
	//{
	//	UGameplayStatics::LoadStreamLevel (
	//	GetWorld ( ) ,
	//	FName ( "LV_Island_JSW" ) ,
	//	true ,  // Whether to make the level visible
	//	true ,  // Should block on load
	//	FLatentActionInfo ( )
	//		);
	//}
	//else
	//{
	//	UGameplayStatics::UnloadStreamLevel(
	//	GetWorld ( ) ,
	//	FName ( "LV_Island_JSW" ) ,
	//	FLatentActionInfo ( ),	
	//	true   // Should block on load
	//	);
	//	GetWorld ( )->SpawnActor<AActor> ( ThemeFactory[info.theme] , GetActorTransform ( ) );
	//}
		// 케이스 나누어서 하기

	//FName LevelToLoad;
	//FName LevelToUnload;

	//if (info.theme == 1)
	//{
	//	LevelToLoad = FName ( "LV_Island_JSW" );
	//	LevelToUnload = FName ( "BP_ThirdLevel" );
	//}
	//else if (info.theme == 2)
	//{
	//	LevelToLoad = FName ( "BP_ThirdLevel" );
	//	LevelToUnload = FName ( "LV_Island_JSW" );
	//}
	//else
	//{
	//	 처리 없이 바로 반환
	//	GetWorld ( )->SpawnActor<AActor> ( ThemeFactory[info.theme] , GetActorTransform ( ) );
	//	LevelToUnload = FName ( "LV_Island_JSW" );
	//	LevelToUnload = FName ( "BP_ThirdLevel" );
	//}

	//UGameplayStatics::LoadStreamLevel (
	//	GetWorld ( ) ,
	//	LevelToLoad ,
	//	true ,  // 레벨을 보이게 할지 여부
	//	true ,  // 로드 시 블록할지 여부
	//	FLatentActionInfo ( )
	//);

	//// 일정 시간 후 이전 레벨을 언로드하는 타이머 설정
	//FTimerHandle UnloadTimerHandle;
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( UnloadTimerHandle , [this , LevelToUnload]( )
	//{
	//	UGameplayStatics::UnloadStreamLevel (
	//		GetWorld ( ) ,
	//		LevelToUnload ,
	//		FLatentActionInfo ( ) ,
	//		true   // 언로드 시 블록할지 여부
	//	);
	//} , 0.2f , false ); // 0.2초의 지연 시간
		// 각 인덱스에 대한 레벨 이름 정의


	// 인덱스에 따라 로드할 레벨 결정
	if (info.theme >= 1 && info.theme <= Levels.Num ( ))
	{
		LevelToLoad = Levels[info.theme - 1]; // 인덱스가 1부터 시작하므로 조정

		// 로드 중인 레벨을 제외한 모든 레벨을 언로드 목록에 추가
		for (int32 i = 0; i < Levels.Num ( ); i++)
		{
			if (i != info.theme - 1)
			{
				LevelsToUnload.Add ( Levels[i] );
			}
		}
	}
	else
	{
		// 인덱스가 범위를 벗어나면 새로운 테마 액터를 스폰하고 바로 반환
		GetWorld ( )->SpawnActor<AActor> ( ThemeFactory[info.theme] , GetActorTransform ( ) );
		return;
	}

	// 선택된 레벨 로드
	UGameplayStatics::LoadStreamLevel (
		GetWorld ( ) ,
		LevelToLoad ,
		true ,  // 로드 후 보이게 설정
		true ,  // 로드 시 블록 여부 설정
		FLatentActionInfo ( )
	);

	GetWorld ( )->SpawnActor<AActor> (FloorFactory[info.terrain] , GetActorTransform ( ) );
	GetWorld ( )->SpawnActor<AActor> (VFXFactory[info.specialEffect] , GetActorTransform ( ) );

	UE_LOG(LogTemp, Warning, TEXT("%d, %d, %d, %d" ), info.sky, info.theme, info.terrain, info.specialEffect);
}

void AJJH_SelectManager::DeleteStage ( )
{
	TArray<FName> tagName = {TEXT("VFX" ), TEXT ( "Floor" ), TEXT ( "Skybox" ) ,TEXT("Theme")  };
	for (int i = 0; i < tagName.Num ( ); i++)
	{
		FindActorAndDestroy(tagName[i] );

	}
	//UGameplayStatics::UnloadStreamLevel(
	//	GetWorld ( ) ,
	//	FName ( "LV_Island_JSW" ) ,
	//	FLatentActionInfo ( ),	
	//	true   // Should block on load
	//	);

	// 일정 시간 후 이전 레벨을 개별적으로 언로드하는 타이머 설정
	for (int32 i = 0; i < LevelsToUnload.Num ( ); ++i)
	{
		const FName Level = LevelsToUnload[i];
		FTimerHandle UnloadTimerHandle;

		GetWorld ( )->GetTimerManager ( ).SetTimer ( UnloadTimerHandle , [this , Level]( )
		{
			UGameplayStatics::UnloadStreamLevel (
				GetWorld ( ) ,
				Level ,
				FLatentActionInfo ( ) ,
				true  // 언로드 시 블록 여부 설정
			);
			UE_LOG ( LogTemp , Warning , TEXT ( "Unloading level: %s" ) , *Level.ToString ( ) );

		} , 0.2f * ( i + 1 ) , false );  // 첫 번째 레벨도 0.4초 지연 후 언로드 <- 비동기적이라 이거 해야함
	}
}

#pragma endregion 