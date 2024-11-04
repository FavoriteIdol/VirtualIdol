// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/JsonParseLib_KMK.h"
#include "KMK/HttpActor_KMK.h"
#include "IImageWrapperModule.h"
#include "JsonObjectConverter.h"

#pragma region Login
FString UJsonParseLib_KMK::MakeLoginJson ( const FString& id , const FString& pw )
{
	// 로그인 데이터를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable ( new FJsonObject ( ) );

    jsonObject->SetStringField("email" , *id);
	jsonObject->SetStringField("password" , *pw);

	// writer를 만들어서 JsonObject를 인코딩해서 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create ( &json );
	FJsonSerializer::Serialize ( jsonObject.ToSharedRef ( ) , writer );
	// 반환한다.
	return json;
}

FLoginInfo UJsonParseLib_KMK::ParsecMyInfo ( const FString& json )
{

    // 서버에서 가져온 json 파일 읽기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create ( json );
    // FJsonObject 형식으로 읽어온 json 데이터를 저장함 => 공유 포인터 형태로 객체 감싸기
    TSharedPtr<FJsonObject> response = MakeShareable ( new FJsonObject ( ) );
    // 역직렬화 : josn 문자열을 FJsonObject로 변경하기
    FLoginInfo result;

    if (FJsonSerializer::Deserialize ( reader , response ))
    {
         if (response->TryGetStringField ( TEXT ( "token" ) , result.token ))
         {
            response->TryGetNumberField ( TEXT ( "userId" ), result.userId );
            response->TryGetStringField ( TEXT ( "email" ), result.email );
            response->TryGetStringField ( TEXT ( "password" ), result.password );
            response->TryGetStringField ( TEXT ( "userName" ), result.userName );
         }
    }
    UE_LOG(LogTemp, Log, TEXT ("%s" ) , *result.token );
    return result;
}

TArray<struct FStageInfo>  UJsonParseLib_KMK::ParsecStageInfos ( const FString& json, const FString& giName)
{
    // 서버에서 가져온 json 파일 읽기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create ( json );
    // FJsonObject 형식으로 읽어온 json 데이터를 저장함 => 공유 포인터 형태로 객체 감싸기
    TSharedPtr<FJsonObject> response = MakeShareable ( new FJsonObject ( ) );
    TArray<struct FStageInfo> stageInfos;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        int32 stageCount = response->GetIntegerField(TEXT("totalElements" ));
        const TArray<TSharedPtr<FJsonValue>>* stageArrays;
        if (response->TryGetArrayField ( TEXT ( "content" ) , stageArrays ))
        {
            for ( const TSharedPtr<FJsonValue>& stage : *stageArrays )
            {
                FStageInfo info;
                TSharedPtr<FJsonObject> stageEle = stage->AsObject();
                if ( stageEle.IsValid() )
                { 
                   info.name = stageEle->GetStringField(TEXT("name"));
                   info.terrain = stageEle->GetIntegerField(TEXT("terrain" ));
                   info.sky = stageEle->GetIntegerField(TEXT("sky" ));
                   info.theme = stageEle->GetIntegerField(TEXT("theme" ));
                   info.specialEffect = stageEle->GetIntegerField(TEXT("specialEffect" ));
                   info.img = stageEle->GetStringField(TEXT("img"));
                   FString userName = stageEle->GetStringField(TEXT("userName"));

                   stageInfos.Add(info);
                }
            }
        }

    }
    return stageInfos;
}

#pragma endregion

#pragma region Set Concert

FString UJsonParseLib_KMK::MakeConcertJson (const struct FConcertInfo& concert )
{

    // 로그인 데이터를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable ( new FJsonObject ( ) );
    // 2024-10-24
    FString json;

    if(concert.concertDate.IsEmpty()) return "0";
    jsonObject->SetStringField(TEXT("name") , concert.name);
	jsonObject->SetStringField( TEXT ( "img") , concert.img);
	jsonObject->SetStringField( TEXT ( "concertDate") , concert.concertDate);
	jsonObject->SetStringField( TEXT ( "startTime") , concert.startTime);
	jsonObject->SetStringField( TEXT ( "endTime") , concert.endTime);
	jsonObject->SetNumberField( TEXT ( "appearedVFX") , concert.appearedVFX);
	jsonObject->SetNumberField( TEXT ( "feverVFX") , concert.feverVFX);
	jsonObject->SetNumberField( TEXT ( "stageId") , concert.stageId);
	jsonObject->SetNumberField( TEXT ( "ticketPrice") , concert.ticketPrice);
	jsonObject->SetNumberField( TEXT ( "peopleScale") , concert.peopleScale);
  
	// writer를 만들어서 JsonObject를 인코딩해서 
	
    // JsonWriter를 사용해 직렬화
    TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create ( &json );
    if (FJsonSerializer::Serialize ( jsonObject.ToSharedRef ( ) , writer ))
    {
        UE_LOG ( LogTemp , Log , TEXT ( "생성된 JSON: %s" ) , *json );
        return json;
    }
    else
    {
        UE_LOG ( LogTemp , Error , TEXT ( "JSON 직렬화 실패" ) );
        return TEXT ( "" );
    }
}

#pragma endregion
#pragma region Create Ticket
FString UJsonParseLib_KMK::CreateTicketJson ( const TMap<FString , FString> ticketSetText )
{
    // source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable ( new FJsonObject ( ) );
    
	for (TPair<FString , FString> pair : ticketSetText)
	{
		jsonObject->SetStringField ( pair.Key , pair.Value );
	}
    // 역직렬화 과정
    FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create ( &json );
	FJsonSerializer::Serialize ( jsonObject.ToSharedRef ( ) , writer );
	// 반환한다.
	return json;

}
#pragma endregion

// Json으로 만들어서 데이터 전송
FString UJsonParseLib_KMK::MakeJson ( const TMap<FString , FString> source )
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable ( new FJsonObject ( ) );

	for (TPair<FString , FString> pair : source)
	{
		jsonObject->SetStringField ( pair.Key , pair.Value );
	}

	// writer를 만들어서 JsonObject를 인코딩해서 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create ( &json );
	FJsonSerializer::Serialize ( jsonObject.ToSharedRef ( ) , writer );
	// 반환한다.
	return json;
}

TMap<FString , FString> UJsonParseLib_KMK::ResultAlchemistParsec ( const FString& json )
{
    // 서버에서 가져온 json 파일 읽기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create ( json );
    // FJsonObject 형식으로 읽어온 json 데이터를 저장함 => 공유 포인터 형태로 객체 감싸기
    TSharedPtr<FJsonObject> response = MakeShareable ( new FJsonObject ( ) );
    TMap<FString , FString> result;
    // 역직렬화 : josn 문자열을 FJsonObject로 변경하기
    if (FJsonSerializer::Deserialize ( reader , response ))
    {
        // key값으로 문자열 읽기
        FString stringValue = response->GetStringField ( TEXT ( "combinable" ) );
        // key값이 존재한고 빈 값이 아닌경우
        if (response->TryGetStringField ( TEXT ( "combinable" ) , stringValue ) && !stringValue.IsEmpty ( ))
        {
            // 내부는 비슷하게 파색함
            bool isCreate = stringValue.Equals ( TEXT ( "true" ) , ESearchCase::IgnoreCase );
            if (isCreate)
            {
                FString FinalEle = response->GetStringField ( TEXT ( "resultElement" ) );
                FString EleName = response->GetStringField ( TEXT ( "fullName" ) );
                FString UsingEle = response->GetStringField ( TEXT ( "uses" ) );

                result.Add ( TEXT ( "Result" ) , FinalEle );
                result.Add ( TEXT ( "Name" ) , EleName );
                result.Add ( TEXT ( "Using" ) , UsingEle );
            }
        }
        else result.IsEmpty ( );

    }
    return result;
}

UTexture2D* UJsonParseLib_KMK::MakeTexture(const TArray<uint8>& ImageData)
{
    if (ImageData.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Image data is empty."));
        return nullptr;
    }
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse image data."));
        return nullptr;
    }
    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
    {
        // Create a TArray to hold the raw image data
        TArray<uint8> RawImageData;

        // Correctly call GetRaw by passing the TArray reference
        if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawImageData))
        {
            int32 Width = ImageWrapper->GetWidth();
            int32 Height = ImageWrapper->GetHeight();

            // Now you can create the texture from the raw image data
            UTexture2D* Texture = CreateTextureFromImage(Width, Height, RawImageData);
            return (Texture);
        }
    }
    return (nullptr);
}

UTexture2D* UJsonParseLib_KMK::CreateTextureFromImage(int32 Width, int32 Height, const TArray<uint8>& RawData)
{
    UTexture2D* NewTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

    if (!NewTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create texture"));
        return nullptr;
    }

    // Make sure the texture has space for the mip data
    FTexture2DMipMap& Mip = NewTexture->GetPlatformData()->Mips[0];
    Mip.SizeX = Width;
    Mip.SizeY = Height;
    Mip.BulkData.Lock(LOCK_READ_WRITE);

    // Get the pointer to the texture data
    void* TextureData = Mip.BulkData.Realloc(RawData.Num());
    FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());

    // Unlock the texture and update the resource
    Mip.BulkData.Unlock();
    NewTexture->UpdateResource();

    return NewTexture;
}
