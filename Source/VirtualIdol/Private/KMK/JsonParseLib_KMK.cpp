// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/JsonParseLib_KMK.h"
#include "KMK/HttpActor_KMK.h"

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
         FString token = response->GetStringField ( TEXT ( "token" ) );
         if (response->TryGetStringField ( TEXT ( "token" ) , token ) && !token.IsEmpty ( ))
         {
            FString email = response->GetStringField ( TEXT ( "email" ) );
            FString pw = response->GetStringField ( TEXT ( "password" ) );
            FString nick = response->GetStringField ( TEXT ( "userName" ) );

            result.email = email;
            result.token = token;
            result.pw = pw;
            result.nickName = nick;
         }
    }
    return result;
}

#pragma endregion

#pragma region Set Concert

FString UJsonParseLib_KMK::MakeConcertJson (const struct FConcertInfo& concert )
{
    // 로그인 데이터를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable ( new FJsonObject ( ) );

    jsonObject->SetStringField("name" , concert.concertName);
    // 2024-10-24


	// writer를 만들어서 JsonObject를 인코딩해서 
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