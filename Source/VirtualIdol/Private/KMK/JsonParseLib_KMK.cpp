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
                   info.stageID = stageEle->GetIntegerField(TEXT("id" ));
                   info.img = stageEle->GetStringField(TEXT("img"));
                   FString userName = stageEle->GetStringField(TEXT("userName"));

                   stageInfos.Add(info);
                }
            }
        }

    }
    return stageInfos;
}
FStageInfo UJsonParseLib_KMK::ParsecMyStageInfo ( const FString& json )
{
    // 서버에서 가져온 json 파일 읽기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create ( json );
    // FJsonObject 형식으로 읽어온 json 데이터를 저장함 => 공유 포인터 형태로 객체 감싸기
    TSharedPtr<FJsonObject> response = MakeShareable ( new FJsonObject ( ) );
    FStageInfo stageInfos;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        if ( response.IsValid() )
        { 
            stageInfos.name = response->GetStringField ( TEXT ( "name" ) );
            stageInfos.terrain = response->GetIntegerField ( TEXT ( "terrain" ) );
            stageInfos.sky = response->GetIntegerField ( TEXT ( "sky" ) );
            stageInfos.theme = response->GetIntegerField ( TEXT ( "theme" ) );
            stageInfos.specialEffect = response->GetIntegerField ( TEXT ( "specialEffect" ) );
            stageInfos.stageID = response->GetIntegerField ( TEXT ( "id" ) );
            stageInfos.img = response->GetStringField ( TEXT ( "img" ) );
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

FConcertInfo  UJsonParseLib_KMK::ParsecMyConcertInfo ( const FString& json )
{
    // 서버에서 가져온 json 파일 읽기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create ( json );
    // FJsonObject 형식으로 읽어온 json 데이터를 저장함 => 공유 포인터 형태로 객체 감싸기
    TSharedPtr<FJsonObject> response = MakeShareable ( new FJsonObject ( ) );
    TArray<struct FConcertInfo> concertInfoArray;
     FConcertInfo con;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        int32 stageCount = response->GetIntegerField(TEXT("totalElements" ));
        const TArray<TSharedPtr<FJsonValue>>* conecertArray;
        if (response->TryGetArrayField ( TEXT ( "content" ) , conecertArray ))
        {
            
            for ( const TSharedPtr<FJsonValue>& concert : *conecertArray )
            {
                FConcertInfo concertInfo;
                TSharedPtr<FJsonObject> concertEle = concert->AsObject();
                if ( concertEle.IsValid() )
                { 
                    concertInfo.name = concertEle->GetStringField ( TEXT ( "name" ) );
                    concertInfo.img = concertEle->GetStringField ( TEXT ( "img" ) );
                    FString price = concertEle->GetStringField ( TEXT ( "ticketPrice" ) );
                    concertInfo.ticketPrice = FCString::Atoi ( *price );
                    FString scale = concertEle->GetStringField ( TEXT ( "peopleScale" ) );
                    concertInfo.peopleScale = FCString::Atoi ( *scale );
                    concertInfo.stageId = concertEle->GetIntegerField ( TEXT ( "stageId" ) );

                    concertInfo.appearedVFX = concertEle->GetIntegerField ( TEXT ( "appearedVFX" ) );
                    concertInfo.feverVFX = concertEle->GetIntegerField ( TEXT ( "feverVFX" ) );

                    int32 id = concertEle->GetIntegerField ( TEXT ( "id" ) );
                    concertInfo.concertDate = concertEle->GetStringField ( TEXT ( "concertDate" ) );

                    concertInfo.startTime = concertEle->GetStringField ( TEXT ( "startTime" ) );
                    concertInfoArray.Add(concertInfo);
                }
            }
        }

    }
    if(concertInfoArray.Num() <= 0) return con;
    return concertInfoArray[0];
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

FString UJsonParseLib_KMK::ChangeString ( const FString& editText )
{
    FString s = editText;
    if (s.Len ( ) == 1)
    {
        s = TEXT ( "0" ) + s;
    }
	return s;
}