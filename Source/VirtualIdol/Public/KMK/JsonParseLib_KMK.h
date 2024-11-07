// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpActor_KMK.h"
#include "JsonParseLib_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UJsonParseLib_KMK : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
#pragma region Login
	// 로그인
	static FString MakeLoginJson( const FString& id,  const FString& pw);
	static FLoginInfo ParsecMyInfo(const FString& json);
#pragma endregion
#pragma region Load Stage
	static TArray<struct FStageInfo> ParsecStageInfos(const FString& json, const FString& giName);
#pragma endregion

#pragma region Concert
	static FString MakeConcertJson(const struct FConcertInfo& concert);
	static FConcertInfo ParsecMyConcertInfo(const FString& json);

	static TArray<struct FConcertInfo> ParsecAllConcert ( const FString& json );
	static FStageInfo ParsecMyStageInfo ( const FString& json );
#pragma endregion
#pragma region CreateTicket
	static FString CreateTicketJson( const TMap<FString , FString> ticketSetText);
#pragma endregion

public:
	static UTexture2D* MakeTexture ( const TArray<uint8>& ImageData );
	static UTexture2D* CreateTextureFromImage(int32 Width, int32 Height, const TArray<uint8>& RawData);
	FString ChangeString ( const FString& editText );
};
