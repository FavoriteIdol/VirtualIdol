// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK/HttpActor_KMK.h"
#include "HttpModule.h"
#include "KMK/JsonParseLib_KMK.h"
#include "GameFramework/Actor.h"
#include "KMK/VirtualGameInstance_KMK.h"

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

	// req->SetURL();
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

	// req->SetURL();
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type") , TEXT("application/json"));
	//req->SetContentAsString(UJsonParseLib_KMK::MakeLoginJson(id , pw));

	req->OnProcessRequestComplete().BindUObject(this , &AHttpActor_KMK::OnResSetConcert);

	req->ProcessRequest();
}

void AHttpActor_KMK::OnResSetConcert ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{

}
#pragma endregion

#pragma region with Ai for Ticket

void AHttpActor_KMK::ReqTicket ( FString json )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	//TMap<FString , FString> data;
	//data.Add ( TEXT ( "key" ) , json );

	req->SetURL ( "https://singular-swine-deeply.ngrok-free.app/posttest" );
	req->SetVerb ( TEXT ( "POST" ) );
	// TEXT ( "application/json" )  ->TEXT("image/jpeg")
	req->SetHeader ( TEXT ( "content-type" ) , TEXT ( "application/json" ) );
	//req->SetContentAsString ( UKMK_JsonParseLib::MakeJson ( data ) );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResTicket );
	// 서버에 요청

	req->ProcessRequest ( );
}

void AHttpActor_KMK::OnResTicket ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString ( );
	}
	else {
		// 실패
		UE_LOG ( LogTemp , Warning , TEXT ( "OnResPostTest Failed..." ) );
	}
}
#pragma endregion