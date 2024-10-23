// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK/HttpActor_KMK.h"
#include "HttpModule.h"
<<<<<<< HEAD
=======
#include "KMK/JsonParseLib_KMK.h"
#include "GameFramework/Actor.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "KMK/StartWidget_KMK.h"
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')

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
	
}

// Called every frame
void AHttpActor_KMK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
<<<<<<< HEAD
=======
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
			gi->SwitchWidget(1);
		}
		else
		{
			gi->SwitchWidget(0);
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

	req->SetHeader(TEXT("accessToken") , FString::Printf(TEXT("%s") , *loginInfo.token));
	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/concerts") );
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
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')

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
	req->SetHeader ( TEXT ( "content-type" ) , TEXT ( "application/json" ) );
<<<<<<< HEAD
	//req->SetContentAsString ( UKMK_JsonParseLib::MakeJson ( data ) );
=======
	req->SetTimeout(60000000.0f);
	req->SetContentAsString ( UJsonParseLib_KMK::CreateTicketJson ( data ) );
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')
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
