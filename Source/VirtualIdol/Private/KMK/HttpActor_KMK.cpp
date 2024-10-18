// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK/HttpActor_KMK.h"
#include "HttpModule.h"
#include "KMK/JsonParseLib_KMK.h"
#include "GameFramework/Actor.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "KMK/StartWidget_KMK.h"
#include "Components/CanvasPanel.h"
#include "GenericPlatform/GenericPlatformHttp.h"

// Sets default values
AHttpActor_KMK::AHttpActor_KMK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHttpActor_KMK::BeginPlay()
{
	Super::BeginPlay();
	 gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance() );
	 if (gi && !gi->GetMyInfo().token.IsEmpty())
	 {
		 loginInfo = gi->GetMyInfo();
	 }
}

// Called every frame
void AHttpActor_KMK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma region Login
void AHttpActor_KMK::ReqLogin ( const FString& id , const FString& pw )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );

	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/auth/login") );
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type") , TEXT("application/json"));
	req->SetContentAsString(UJsonParseLib_KMK::MakeLoginJson(id , pw));

	req->OnProcessRequestComplete().BindUObject(this , &AHttpActor_KMK::OnResLogin);

	req->ProcessRequest();

}

void AHttpActor_KMK::OnResLogin ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		loginInfo = UJsonParseLib_KMK::ParsecMyInfo(Response->GetContentAsString());
		// 토큰이 존재한다면 => 모든 정보값이 존재함
		if ( gi && !loginInfo.email.IsEmpty()) 
		{
			gi->SetMyInfo(loginInfo);
			UE_LOG ( LogTemp , Log , TEXT ( "%s" ) , *loginInfo.token );
            gi->SwitchWidget ( 1 );
		}
		else
		{
			gi->LoginPanel();
		}
	}
	else 
	{
		// 실패
		UE_LOG ( LogTemp , Warning , TEXT ( "OnResLogin Failed..." ) );
	}
}
#pragma endregion
#pragma region Concert

void AHttpActor_KMK::ReqSetConcert ( const FConcertInfo& concert )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
    req->SetHeader(TEXT("Authorization"), *( authHeader ));
	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/concerts") );
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type") , TEXT("application/json"));
	FString s = UJsonParseLib_KMK::MakeConcertJson(concert) ;
    req->SetContentAsString ( s);
	UE_LOG ( LogTemp , Log , TEXT ( "%s" ) ,  *(TEXT("Bearer " ) + loginInfo.token) );
	req->OnProcessRequestComplete().BindUObject(this , &AHttpActor_KMK::OnResSetConcert);

	req->ProcessRequest();
}
void AHttpActor_KMK::OnResSetConcert ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (!bConnectedSuccessfully)
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP 요청 실패"));
        return;
       
    }
	else
	{
        int32 StatusCode = Response->GetResponseCode ( );
        FString ResponseBody = Response->GetContentAsString ( );

        if (StatusCode == 403)
        {
			if(count > 1)  return;
            UE_LOG ( LogTemp , Error , TEXT ( "403 Forbidden: 인증 문제 또는 권한 부족 - 응답 내용: %s" ) , *ResponseBody );
			FPlatformProcess::Sleep ( 1.0f ); // 1초 대기
			ReqSetConcert (gi->concerInfo);
			count++;
        }
        else
        {
            UE_LOG ( LogTemp , Warning , TEXT ( "응답 코드: %d - %s" ) , StatusCode , *ResponseBody );
            gi->PopUpVisible ( );
			//gi->concerInfo.Clear( );
        }
	}
}
#pragma endregion

#pragma region with Ai for Ticket

void AHttpActor_KMK::ReqTicket ( const TMap<FString , FString> data )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	//TMap<FString , FString> data;
	//data.Add ( TEXT ( "key" ) , json );

	req->SetURL(TEXT("https://singular-swine-deeply.ngrok-free.app/generate-image") );
	req->SetVerb ( TEXT ( "POST" ) );
	// TEXT ( "application/json" )  ->TEXT("image/jpeg")
	req->SetHeader ( TEXT ( "content-type" ) , TEXT ( "application/json" ) );
	req->SetTimeout(1000.0f);
	req->SetContentAsString ( UJsonParseLib_KMK::CreateTicketJson ( data ) );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResTicket );
	// 서버에 요청

	req->ProcessRequest ( );
}

void AHttpActor_KMK::OnResTicket ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
 	if (bConnectedSuccessfully && Response.IsValid() && Response->GetResponseCode() == 200)
    {
        // 다운로드 받은 PNG 데이터를 파일로 저장
        TArray<uint8> ImageData = Response->GetContent();

        FString RequestUrl = Request->GetURL();

        FString Nickname;
        FParse::Value(*RequestUrl, TEXT("filename="), Nickname);
        FString FullPath = FPaths::ProjectSavedDir() / TEXT("Pictures/") / Nickname + TEXT(".png");

        if (FFileHelper::SaveArrayToFile(ImageData, *FullPath))
        {
            UTexture2D* Texture = UJsonParseLib_KMK::MakeTexture(ImageData);
            if (Texture)
            {
                UE_LOG(LogTemp, Log, TEXT("Texture successfully created from downloaded image"));

                // Use the texture as needed, e.g., apply it to a material or widget
                OnTextureCreated(Texture);  // Custom method to handle the texture
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to decode the image data."));
            }
        }
        
    }
   else
   {
       if (Response.IsValid())
       {
           UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
       }
       else
       {
           UE_LOG(LogTemp, Error, TEXT("Response is invalid"));
       }
   }

}
void AHttpActor_KMK::OnTextureCreated(UTexture2D* texture)
{
	 gi->widget->CreateTicketMaterial(texture );
}


#pragma endregion