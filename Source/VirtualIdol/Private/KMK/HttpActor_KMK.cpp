// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK/HttpActor_KMK.h"
#include "HttpModule.h"
#include "KMK/JsonParseLib_KMK.h"
#include "GameFramework/Actor.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "KMK/StartWidget_KMK.h"
#include "Components/CanvasPanel.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "JsonObjectConverter.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "IImageWrapper.h"

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
	 if (effectArray.Num ( ) > 0 && gi)
	 {
		 gi->effectArray = effectArray;
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
			gi->SetMyProfile();
			ReqCheckMyConcert();
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
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *loginInfo.token );
}
#pragma endregion
#pragma region Concert
void AHttpActor_KMK::ReqSetMyConcert (FConcertInfo& concert )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );

	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/concerts") );
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type") , TEXT("application/json"));
	concert.img = ticketURL;
	FString s = UJsonParseLib_KMK::MakeConcertJson(concert) ;
    req->SetContentAsString (s);
	FString AuthHeader = FString::Printf(TEXT("Bearer %s"), *loginInfo.token);
	req->SetHeader(TEXT("Authorization"), AuthHeader);
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
        }
        else
        {
            UE_LOG ( LogTemp , Warning , TEXT ( "응답 코드: %d - %s" ) , StatusCode , *ResponseBody );
            gi->PopUpVisible ( );
        }
	}
}
// 무대 불러오기
void AHttpActor_KMK::ReqCheckStage (class UStartWidget_KMK* startWidget )
{
	if(!startWidget) return;
	sw = startWidget;
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
    req->SetHeader(TEXT("Authorization"), *( authHeader ));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/stages") );
	req->SetVerb ( TEXT ( "GET" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResCheckStage );
}

void AHttpActor_KMK::OnResCheckStage ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (Response->GetResponseCode ( ) == 200)
	{
		UE_LOG ( LogTemp , Error , TEXT ( "CheckStage Successed" ));
		// 성공
		FString respon = Response->GetContentAsString();
		// gi에 있는 닉네임 파악
		allStageInfoArray = UJsonParseLib_KMK::ParsecStageInfos(respon, gi->GetMyInfo().userName);
		for (int i = 0; i < allStageInfoArray.Num ( ); i++)
		{
			DownloadImageFromUrl(allStageInfoArray[i].img, allStageInfoArray[i] );
			//sw->CreateStageWidget(allStageInfoArray[i] , texture );
		}

		UE_LOG ( LogTemp , Error , TEXT ( "%d" ), allStageInfoArray.Num());
	}
	else
	{
		UE_LOG ( LogTemp , Error , TEXT ( "CheckStage Failed" ));
	}
}

// 내가 만든 공연장이 있다면
void AHttpActor_KMK::ReqCheckMyStage ( class UStartWidget_KMK* startWidget )
{
	if(!startWidget) return;
	sw = startWidget;
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
    req->SetHeader(TEXT("Authorization"), *( authHeader ));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString url = TEXT("http://master-of-prediction.shop:8123/api/v1/stages/user/") + FString::FormatAsNumber(gi->GetMyInfo().userId );
	UE_LOG ( LogTemp , Warning , TEXT ( "ImagePath: %s" ) , *url );
	req->SetURL(url );
	req->SetVerb ( TEXT ( "GET" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResCheckMyStage );
}

void AHttpActor_KMK::OnResCheckMyStage ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공 => 내가 만든 세트장이 있다면
		FString respon = Response->GetContentAsString();
		// 스테이지를 array에 넣어주고
		myStageInfoArray = UJsonParseLib_KMK::ParsecStageInfos(respon, gi->GetMyInfo().userName);
		// 각각의 스테이지에 있는 썸네일을 넣어준다
		for (int i = 0; i < myStageInfoArray.Num ( ); i++)
		{
			FConcertInfo concert;
			// 이 코드가 백엔드에 들어간 썸네일을 들고오는 부분
			DownloadImageFromUrl(myStageInfoArray[i].img, myStageInfoArray[i] );
			//sw->CreateStageWidget(myStageInfoArray[i] , texture );
			UE_LOG ( LogTemp , Error , TEXT ( "%s" ) , *myStageInfoArray[i].name);
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed myStage" ) );
	}
}
// 백엔드에서 링크를 가져온다
void AHttpActor_KMK::DownloadImageFromUrl(const FString& imageUrl, const FStageInfo& stageInfo)
{
	FHttpModule* http = &FHttpModule::Get();
    TSharedRef<IHttpRequest> req = http->CreateRequest();
    req->SetURL(imageUrl);
    req->SetVerb("GET");
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor_KMK::OnImageDownComplete, stageInfo);
    req->ProcessRequest();
}


void AHttpActor_KMK::OnImageDownComplete ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful, FStageInfo stageInfo  )
{
	if (bWasSuccessful&& Response.IsValid())
	{
		if (Response->GetResponseCode() != 200)
        {
            UE_LOG ( LogTemp , Error , TEXT ( "Failed to get image. Response Code: %d" ) , Response->GetResponseCode ( ) );
            return;
        }
		FString ContentType = Response->GetContentType();
        UE_LOG ( LogTemp , Log , TEXT ( "Content-Type: %s" ) , *ContentType );

        if (!ContentType.Contains ( "image" ))
        {
            UE_LOG ( LogTemp , Error , TEXT ( "Unexpected Content-Type: %s" ) , *ContentType );
            return;
        }
        // 다운로드 받은 PNG 데이터를 파일로 저장
        const TArray<uint8>& ImageData = Response->GetContent();

		UTexture2D* texture = UJsonParseLib_KMK::MakeTexture(ImageData);
        if (texture)
        {
            UE_LOG(LogTemp, Log, TEXT("Image downloaded and texture created successfully!"));
            // 예: 다운로드한 텍스처를 위젯이나 다른 액터에 할당
			sw->CreateStageWidget(stageInfo, texture);

        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create texture from downloaded image data."));
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
		   if(ticketData.Num() > 0) ReqTicket(ticketData );
       }
   }
    
}


#pragma endregion

#pragma region with Ai for Ticket
// 티켓 만들기 => AI 측에 만들어달라고 신호보내기
void AHttpActor_KMK::ReqTicket ( const TMap<FString , FString> data )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	//TMap<FString , FString> data;
	//data.Add ( TEXT ( "key" ) , json );
	if(ticketData.Num() <= 0) ticketData = data;

	req->SetURL(TEXT("https://singular-swine-deeply.ngrok-free.app/generate-image") );
	req->SetVerb ( TEXT ( "POST" ) );
	// TEXT ( "application/json" )  ->TEXT("image/jpeg")
	req->SetHeader ( TEXT ( "content-type" ) , TEXT ( "application/json" ) );
	req->SetTimeout(240.f);
	req->SetContentAsString ( UJsonParseLib_KMK::CreateTicketJson ( ticketData ) );
	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResTicket );
	// 서버에 요청

}
// 응답 받기
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
			ReqMultipartCapturedWithAI(FullPath);
            UTexture2D* Texture = UJsonParseLib_KMK::MakeTexture(ImageData);
            if (Texture)
            {
                UE_LOG(LogTemp, Log, TEXT("Texture successfully created from downloaded image"));
				sw->bCreateTicket = true;
				sw->SetLoadImage();
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
		   if(ticketData.Num() > 0) ReqTicket(ticketData );
       }
   }

}
void AHttpActor_KMK::ReqMultipartCapturedWithAI (const FString& ImagePath , const FString& url )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Image upload start." ) );

	// Create an HTTP request for the multipart upload
	TSharedRef<IHttpRequest> ImageUploadRequest = FHttpModule::Get ( ).CreateRequest ( );
	ImageUploadRequest->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnReqMultipartCapturedWithAI );

	// Set the URL and verb for the image upload request
	ImageUploadRequest->SetURL ( url );
	ImageUploadRequest->SetVerb ( TEXT ( "POST" ) );

	UE_LOG ( LogTemp , Warning , TEXT ( "ImagePath: %s" ) , *ImagePath );

	// Prepare multipart form data for image upload
	TArray<uint8> FileData;
	bool bSuccess = FFileHelper::LoadFileToArray ( FileData , *ImagePath );
	if (!bSuccess)
	{
		UE_LOG ( LogTemp , Error , TEXT ( "Failed to load file: %s" ) , *ImagePath );
		return; // Exit if file loading fails
	}

	FString FileName = FPaths::GetCleanFilename ( ImagePath );
	FString Boundary = "---------------------------boundary_string";
	FString BeginBoundary = FString ( "--" ) + Boundary + TEXT ( "\r\n" );
	FString EndBoundary = FString ( "\r\n--" ) + Boundary + TEXT ( "--\r\n" );

	FString FileHeader = FString ( "Content-Disposition: form-data; name=\"file\"; filename=\"" ) + FileName + TEXT ( "\"\r\nContent-Type: image/png\r\n\r\n" );

	TArray<uint8> PostData;
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *BeginBoundary ) , BeginBoundary.Len ( ) );
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *FileHeader ) , FileHeader.Len ( ) );
	PostData.Append ( FileData );
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *EndBoundary ) , EndBoundary.Len ( ) );

	// Add Authorization header
	FString AuthHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
	ImageUploadRequest->SetHeader ( TEXT ( "Authorization" ) , AuthHeader );

	// Set multipart header
	FString ContentType = FString::Printf ( TEXT ( "multipart/form-data; boundary=%s" ) , *Boundary );
	ImageUploadRequest->SetHeader ( TEXT ( "Content-Type" ) , *ContentType );

	ImageUploadRequest->SetTimeout ( 180.f );
	// Set the content to send
	ImageUploadRequest->SetContent ( PostData );

	// Process the request
	ImageUploadRequest->ProcessRequest ( );
}

void AHttpActor_KMK::OnReqMultipartCapturedWithAI ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid ( ))
	{
		int32 ResponseCode = Response->GetResponseCode ( );
		if (ResponseCode == 200) // 성공적으로 응답받은 경우
		{
			// 응답 바디를 가져와 Stage의 img에 설정합니다.
			ticketURL = Response->GetContentAsString ( );

			UE_LOG ( LogTemp , Warning , TEXT ( "Image URL set to: %s" ) , *ticketURL );
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Image upload failed." ) );
	}
}

void AHttpActor_KMK::OnTextureCreated(UTexture2D* texture)
{
	 gi->widget->CreateTicketMaterial(texture );
}
#pragma endregion
#pragma region Translate

void AHttpActor_KMK::ReqTranslateChat ( const FString& json )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );

	req->SetURL(TEXT("https://singular-swine-deeply.ngrok-free.app/translate") );
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type") , TEXT("application/json"));

	FString s = UJsonParseLib_KMK::MakeChatTranslate(json);
    req->SetContentAsString (s);

	req->OnProcessRequestComplete().BindUObject(this , &AHttpActor_KMK::OnReqTranslateChat);

	req->ProcessRequest();
}

void AHttpActor_KMK::OnReqTranslateChat ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString();
		if (Response.IsValid() && Response->GetResponseCode() == 200)
		{

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
		}
		
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed myStage" ) );
	}
}

#pragma endregion

#pragma region BE for CheckConcert
// 이미지 다운로드 코드 작성

void AHttpActor_KMK::DownloadImageConcert ( const FString& imageUrl, const FConcertInfo& concertInfo )
{
	FHttpModule* http = &FHttpModule::Get();
    TSharedRef<IHttpRequest> req = http->CreateRequest();
    req->SetURL(imageUrl);
    req->SetVerb("GET");
    req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnImageDownConcertComplete , concertInfo );
    req->ProcessRequest ( );
}


void AHttpActor_KMK::OnImageDownConcertComplete ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bWasSuccessful , FConcertInfo concertInfo )
{
if (bWasSuccessful&& Response.IsValid())
	{
		if (Response->GetResponseCode() != 200)
        {
            UE_LOG ( LogTemp , Error , TEXT ( "Failed to get image. Response Code: %d" ) , Response->GetResponseCode ( ) );
            return;
        }
		FString ContentType = Response->GetContentType();
        UE_LOG ( LogTemp , Log , TEXT ( "Content-Type: %s" ) , *ContentType );

        if (!ContentType.Contains ( "image" ))
        {
            UE_LOG ( LogTemp , Error , TEXT ( "Unexpected Content-Type: %s" ) , *ContentType );
            return;
        }
        // 다운로드 받은 PNG 데이터를 파일로 저장
        const TArray<uint8>& ImageData = Response->GetContent();

		UTexture2D* texture = UJsonParseLib_KMK::MakeTexture(ImageData);
        if (texture)
        {
            UE_LOG(LogTemp, Log, TEXT("Image downloaded and texture created successfully!"));
            // 예: 다운로드한 텍스처를 위젯이나 다른 액터에 할당
			concertInfo.texture = texture;
			gi->allConcertInfoArray.Add(concertInfo);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create texture from downloaded image data."));
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
		   if(ticketData.Num() > 0) ReqTicket(ticketData );
       }
   }
}

void AHttpActor_KMK::ReqCheckMyConcert ( )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	FString AuthHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
	req->SetHeader ( TEXT ( "Authorization" ) , AuthHeader );
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString url = TEXT("http://master-of-prediction.shop:8123/api/v1/concerts/user/") + FString::FormatAsNumber(gi->GetMyInfo().userId );
	UE_LOG ( LogTemp , Warning , TEXT ( "ImagePath: %s" ) , *url );
	req->SetURL(url );
	req->SetVerb ( TEXT ( "GET" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResqCheckMyConcert );
}

void AHttpActor_KMK::OnResqCheckMyConcert ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString();
		if (Response.IsValid() && Response->GetResponseCode() == 200)
		{
			FConcertInfo info = UJsonParseLib_KMK::ParsecMyConcertInfo(respon);
			if (!info.name.IsEmpty())
			{
				if (info.texture == nullptr)
				{
					gi->SetConcertInfo(info);
					ReqCheckIdStage(info.stageId);
                }
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
		}
		
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed myStage" ) );
	}
}
void AHttpActor_KMK::ReqCheckAllOpenConcert ( )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	FString AuthHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
	req->SetHeader ( TEXT ( "Authorization" ) , AuthHeader );
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	req->SetURL(TEXT("http://master-of-prediction.shop:8123/api/v1/concerts" ) );
	req->SetVerb ( TEXT ( "GET" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResCheckAllOpenConcert );
}

void AHttpActor_KMK::OnResCheckAllOpenConcert ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString();
		if (Response.IsValid() && Response->GetResponseCode() == 200)
		{
			allConcertInfoArray = UJsonParseLib_KMK::ParsecAllConcert(respon);

			// 사진 관련된 항목
			if (allConcertInfoArray.Num ( ) > 0)
			{
				for (auto& concert : allConcertInfoArray)
				{
					DownloadImageConcert(concert.img, concert);

				}
				
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
		}
		
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed myStage" ) );
	}
}

void AHttpActor_KMK::ReqCheckIdStage ( int32 num )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
    req->SetHeader(TEXT("Authorization"), *( authHeader ));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString url = TEXT("http://master-of-prediction.shop:8123/api/v1/stages/") + FString::FormatAsNumber(num );
	UE_LOG ( LogTemp , Warning , TEXT ( "URL: %s" ) , *url );
	req->SetURL(url );
	req->SetVerb ( TEXT ( "GET" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnResCheckIdStage );
}

void AHttpActor_KMK::OnResCheckIdStage ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString();
		if (Response.IsValid() && Response->GetResponseCode() == 200)
		{
			FStageInfo info = UJsonParseLib_KMK::ParsecMyStageInfo(respon);
			gi->SetConcertStageInfo(info);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed myStage" ) );
	}
}


#pragma endregion

#pragma region with BE for StageSettings

void AHttpActor_KMK::ReqMultipartCapturedURL ( FStageInfo& Stage , const FString& ImagePath )
{   
	UE_LOG ( LogTemp , Warning , TEXT ( "Image upload start." ) );

	// Create an HTTP request for the multipart upload
	TSharedRef<IHttpRequest> ImageUploadRequest = FHttpModule::Get ( ).CreateRequest ( );
	ImageUploadRequest->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnReqMultipartCapturedURL , &Stage );

	// Set the URL and verb for the image upload request
	ImageUploadRequest->SetURL ( TEXT ( "http://master-of-prediction.shop:8123/api/v1/files/upload" ) );
	ImageUploadRequest->SetVerb ( TEXT ( "POST" ) );

	UE_LOG ( LogTemp , Warning , TEXT ( "ImagePath: %s" ) , *ImagePath );

	// Prepare multipart form data for image upload
	TArray<uint8> FileData;
	bool bSuccess = FFileHelper::LoadFileToArray ( FileData , *ImagePath );
	if (!bSuccess)
	{
		UE_LOG ( LogTemp , Error , TEXT ( "Failed to load file: %s" ) , *ImagePath );
		return; // Exit if file loading fails
	}

	FString FileName = FPaths::GetCleanFilename ( ImagePath );
	FString Boundary = "---------------------------boundary_string";
	FString BeginBoundary = FString ( "--" ) + Boundary + TEXT ( "\r\n" );
	FString EndBoundary = FString ( "\r\n--" ) + Boundary + TEXT ( "--\r\n" );

	FString FileHeader = FString ( "Content-Disposition: form-data; name=\"file\"; filename=\"" ) + FileName + TEXT ( "\"\r\nContent-Type: image/png\r\n\r\n" );

	TArray<uint8> PostData;
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *BeginBoundary ) , BeginBoundary.Len ( ) );
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *FileHeader ) , FileHeader.Len ( ) );
	PostData.Append ( FileData );
	PostData.Append ( (uint8*)TCHAR_TO_ANSI ( *EndBoundary ) , EndBoundary.Len ( ) );

	// Add Authorization header
	FString AuthHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
	ImageUploadRequest->SetHeader ( TEXT ( "Authorization" ) , AuthHeader );

	// Set multipart header
	FString ContentType = FString::Printf ( TEXT ( "multipart/form-data; boundary=%s" ) , *Boundary );
	ImageUploadRequest->SetHeader ( TEXT ( "Content-Type" ) , *ContentType );

	ImageUploadRequest->SetTimeout ( 180.f );
	// Set the content to send
	ImageUploadRequest->SetContent ( PostData );

	// Process the request
	ImageUploadRequest->ProcessRequest ( );
}

void AHttpActor_KMK::OnReqMultipartCapturedURL ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully , FStageInfo* Stage )
{
	if (bConnectedSuccessfully && Response.IsValid ( ))
	{
		int32 ResponseCode = Response->GetResponseCode ( );
		if (ResponseCode == 200) // 성공적으로 응답받은 경우
		{
			// 응답 바디를 가져와 Stage의 img에 설정합니다.
			FString ImageURL = Response->GetContentAsString ( );
			Stage->img = ImageURL;

			ReqStageInfo(*Stage);
			UE_LOG ( LogTemp , Warning , TEXT ( "Image URL set to: %s" ) , *Stage->img );
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Image upload failed." ) );
	}
}

void AHttpActor_KMK::ReqStageInfo ( const FStageInfo& Stage)
{

	// JSON으로 변환
	FString JsonString;
	if (!FJsonObjectConverter::UStructToJsonObjectString ( Stage , JsonString ))
	{
		UE_LOG ( LogTemp , Error , TEXT ( "Failed to convert Stage to JSON." ) );
		return;
	}
	UE_LOG ( LogTemp , Warning , TEXT ( "Sending Request: %s" ) , *JsonString );
	
	FJsonObjectConverter::UStructToJsonObjectString (Stage, JsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get ( ).CreateRequest ( );
	Request->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnReqStageInfo );
	Request->SetURL ( TEXT ( "http://master-of-prediction.shop:8123/api/v1/stages" ) );
	Request->SetVerb ( TEXT ( "POST" ) );

	// Add Authorization header
	FString AuthHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
	Request->SetHeader ( TEXT ( "Authorization" ) , AuthHeader );

	// JSON 데이터 전송
	Request->SetHeader ( TEXT ( "Content-Type" ) , TEXT ( "application/json" ) );
	Request->SetContentAsString ( JsonString ); // JSON 문자열 설정

	// 요청 전송
	Request->ProcessRequest ( );
}

void AHttpActor_KMK::OnReqStageInfo ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{

	if (Response->GetResponseCode ( ) == 201)
	{
		UE_LOG ( LogTemp , Error , TEXT ( "Uploaded" ));

	}
	else
	{
		UE_LOG ( LogTemp , Error , TEXT ( "failed" ) );
	}

}

#pragma endregion

void AHttpActor_KMK::ReqCollcetionConcert ( int32 concertIndex )
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get ( );
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest ( );
	// 요청할 정보를 설정
	req->SetContentAsString(FString::FromInt(concertIndex));
	FString authHeader = FString::Printf ( TEXT ( "Bearer %s" ) , *gi->loginInfo.token );
    req->SetHeader(TEXT("Authorization"), *( authHeader ));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString url = TEXT("http://master-of-prediction.shop:8123/api/user-activity/collect-concert");
	req->SetURL(url );
	req->SetVerb ( TEXT ( "POST" ) );

	req->ProcessRequest ( );
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete ( ).BindUObject ( this , &AHttpActor_KMK::OnReqMultiCollectionConcert );
}

void AHttpActor_KMK::OnReqMultiCollectionConcert ( FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully )
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString respon = Response->GetContentAsString();
		if (Response->GetResponseCode() == 200)
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "Collcet concert" ) );
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to download image. Response code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Failed Collcet concert" ) );
	}
}
