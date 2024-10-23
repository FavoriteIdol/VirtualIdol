// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpActor_KMK.generated.h"
// 세션을 위한 roomInfo 구조체
USTRUCT ( BlueprintType )
struct FConcertInfo
{
	GENERATED_BODY ( )

	UPROPERTY(BlueprintReadOnly)
	FString name;
	UPROPERTY(BlueprintReadOnly)
	FString img;
	UPROPERTY(BlueprintReadOnly)
	FString concertDate;
	UPROPERTY(BlueprintReadOnly)
	FString startTime;
	UPROPERTY(BlueprintReadOnly)
	FString endTime;
	UPROPERTY(BlueprintReadOnly)
	int32 appearedVFX;
	UPROPERTY(BlueprintReadOnly)
	int32 feverVFX;
	UPROPERTY ( BlueprintReadOnly )
    int32 stageId;
	UPROPERTY(BlueprintReadOnly)
    int32 ticketPrice;
	UPROPERTY(BlueprintReadOnly)
	int32 peopleScale;

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
	FString email;
	UPROPERTY(BlueprintReadOnly)
	FString password;
	UPROPERTY(BlueprintReadOnly)
	FString token;
	UPROPERTY(BlueprintReadOnly)
	FString userName;
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
	void ReqSetConcert(const FConcertInfo& concert );
	// 응답
	void OnResSetConcert( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion
#pragma region with Ai for Ticket
	// 요청
	void ReqTicket( const TMap<FString , FString> data);
	// 응답
	void OnResTicket( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
	
	void OnTextureCreated(UTexture2D* texture );
#pragma endregion
#pragma region with Ai for Text

#pragma endregion



};
