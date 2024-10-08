// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UJsonParseLib_KMK : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
#pragma region MakeJson
	static FString MakeJson ( const TMap<FString , FString> source );
#pragma endregion

#pragma region Parsec_KMK
	static TMap<FString , FString>  ResultAlchemistParsec ( const FString& json );
#pragma endregion

};
