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
	FString concertName;
	UPROPERTY(BlueprintReadOnly)
	FString concertDay;
	UPROPERTY(BlueprintReadOnly)
	int32 concertTime;
	UPROPERTY(BlueprintReadOnly)
	int32 brrowTime;
	UPROPERTY(BlueprintReadOnly)
	int32 tickePrice;
	UPROPERTY(BlueprintReadOnly)
	FString ticketInfo;
	UPROPERTY(BlueprintReadOnly)
	int32 appearEffectNum;
	UPROPERTY(BlueprintReadOnly)
	int32 feverEffectNum;
	UPROPERTY(BlueprintReadOnly)
	int32 personNum;
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

#pragma region with Ai for Ticket
	// 요청
	void ReqTicket( FString json );
	// 응답
	void OnResTicket( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully );
#pragma endregion
#pragma region with Ai for Text

#pragma endregion
#pragma region with BE for Login

#pragma endregion
#pragma region with BE for SettingStage

#pragma endregion

};
