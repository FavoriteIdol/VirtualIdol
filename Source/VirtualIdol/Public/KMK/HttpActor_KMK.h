// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "IImageWrapper.h"
#include "HttpActor_KMK.generated.h"
// 세션을 위한 roomInfo 구조체
USTRUCT ( BlueprintType )
struct FConcertInfo
{
	GENERATED_BODY ( )

	UPROPERTY(BlueprintReadOnly)
	FString name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString img = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString concertDate = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString startTime = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString endTime = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 appearedVFX = -1;
	UPROPERTY(BlueprintReadOnly)
	int32 feverVFX = -1;
	UPROPERTY ( BlueprintReadOnly )
    int32 stageId = 1;
	UPROPERTY(BlueprintReadOnly)
    int32 ticketPrice = -1;
	UPROPERTY(BlueprintReadOnly)
	int32 peopleScale = -1;

	void Clear ( )
	{
		name.Empty ( );
		img.Empty ( );
		concertDate.Empty ( );
		startTime.Empty ( );
		endTime.Empty ( );
		appearedVFX = 0;
		feverVFX = 0;
		stageId = 0;
		ticketPrice = 0;
		peopleScale = 0;
	}

};
// 개인정보
USTRUCT ( BlueprintType )
struct FLoginInfo
{
	GENERATED_BODY ( )

	UPROPERTY(BlueprintReadOnly)
	int32 userId = -1;
	UPROPERTY(BlueprintReadOnly)
	FString email= TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString password= TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString token= TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString userName= TEXT("");
};

//무대 정보
USTRUCT(BlueprintType )
struct FStageInfo
{

	GENERATED_BODY( )

	UPROPERTY(BlueprintReadOnly)
	FString name= TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 terrain = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 sky = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 theme = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 specialEffect = 0;
	UPROPERTY( )
	int32 stageID = -1;
	UPROPERTY(BlueprintReadOnly)
	FString img= TEXT("");

};

UCLASS()
class VIRTUALIDOL_API AHttpActor_KMK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor_KMK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> effectArray;

#pragma region with BE for Login
	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;

	UPROPERTY( )
	struct FLoginInfo loginInfo;
	TMap<FString, FString> myInfo;
	// 요청
	void ReqLogin(const FString& id, const FString& pw );
	// 응답
	void OnResLogin( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion
#pragma region with BE for SettingStage
	int32 count = 0;
	// 요청
	void ReqSetMyConcert(const FConcertInfo& concert );
	// 응답
	void OnResSetConcert( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion
#pragma region with Ai for Ticket
	// 티켓 정보 저장
	TMap<FString , FString> ticketData;
	// 요청
	void ReqTicket( const TMap<FString , FString> data);
	// 응답
	void OnResTicket( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
	
	void OnTextureCreated(UTexture2D* texture );
#pragma endregion
#pragma region BE CheckAll Stage
	// 요청
	void ReqCheckStage(class UStartWidget_KMK* startWidget );
	// 응답 
	void OnResCheckStage( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
	// 전체 무대
	TArray<struct FStageInfo> allStageInfoArray;
	TArray<struct FStageInfo> myStageInfoArray;
	
	UPROPERTY( )
	class UStartWidget_KMK* sw;

	// 내무대 확인
	// 요청
	void ReqCheckMyStage(class UStartWidget_KMK* startWidget );
	// 응답 
	void OnResCheckMyStage( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );

	// 이미지 생성 및 다운
	void DownloadImageFromUrl ( const FString& imageUrl , const FStageInfo& stageInfo );
	void OnImageDownComplete ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful, FStageInfo stageInfo );

	// 특정 무대 요청
	void ReqCheckIdStage( int32 num);
	void OnResCheckIdStage(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion
#pragma region Check My Concert
	// 내무대 확인
	UPROPERTY( )
	class UStartWidget_KMK* myStartWid;
	// 요청
	void ReqCheckMyConcert( );
	// 응답 
	void OnResqCheckMyConcert( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion

#pragma region with Ai for Text

#pragma endregion


#pragma region with BE for StageSettings
	//요청
	void ReqMultipartCapturedURL( FStageInfo& Stage , const FString& ImagePath );
	//응답
	void OnReqMultipartCapturedURL( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully , FStageInfo* Stage );
	//요청
	void ReqStageInfo( const FStageInfo& Stage);
	//응답
	void OnReqStageInfo( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion


};
