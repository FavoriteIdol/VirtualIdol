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
#pragma region Set Concert
	static FString MakeConcertJson(const struct FConcertInfo& concert);

#pragma endregion


#pragma region Example
	static FString MakeJson ( const TMap<FString , FString> source );
	static TMap<FString , FString>  ResultAlchemistParsec ( const FString& json );
#pragma endregion

};
