// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/VirtualGameInstance_KMK.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"
#include "string"
#include "VirtualIdol.h"
#include "KMK/StartWidget_KMK.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "KMK/AudienceServerComponent_KMK.h"
#include "KMK/HttpActor_KMK.h"
#include "Components/CanvasPanel.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "KMK/Virtual_KMK.h"
#include "KMK/Audience_KMK.h"
#include "Kismet/GameplayStatics.h"


#include "SocketSubsystem.h"
#include "Sockets.h"
#include "IPAddress.h"
#include "JJH/JJH_SelectManager.h"

void UVirtualGameInstance_KMK::Init ( )
{
    Super::Init();
    if (auto* subSystem = IOnlineSubsystem::Get ( ))
    {
        sessionInterface = subSystem->GetSessionInterface();
        // 방생성 요청, 응답
        sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UVirtualGameInstance_KMK::OnMyCreateSessionComplete );
        // 방찾기 성공
        sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UVirtualGameInstance_KMK::OnMyFindSessionComplete);
         // 방입장
        sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this , &UVirtualGameInstance_KMK::JoinRoomComplete);
        // 방 퇴장
        sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this , &UVirtualGameInstance_KMK::OnMyDestroyRoomComplete);
    }

}

#pragma region Create Session

void UVirtualGameInstance_KMK::CreateMySession ( FString RoomName, int32 PlayerCount )
{
    FOnlineSessionSettings settings;
    // 전용서버를 사용하는가? => 데디케이트 서버
    settings.bIsDedicated = false;
    HostName = loginInfo.userName;
    // 랜선인가?
    FName subSystemName = IOnlineSubsystem::Get( )->GetSubsystemName();
    // 온라인 서브 시스템이 없는 경우에 LAN으로 연결, 아니라면 온라인 서브 시스템으로 연결
    settings.bIsLANMatch = (subSystemName == NAME_None);
    // 공개된 서버인가 아닌가
    settings.bShouldAdvertise = true;
    // 유저 상태정보 사용 여부
    settings.bUsesPresence = true;
    // 중간에 난입 가능 여부
    settings.bAllowJoinInProgress = true;
    settings.bAllowJoinViaPresence = true;
    // 최대 인원
    settings.NumPublicConnections = PlayerCount;
    // 방 커스텀 정보
    settings.Set(FName("Room_Name"), StringBase64Encode(RoomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
    settings.Set(FName("Host_Name"), StringBase64Encode(HostName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
    // FUniqueNetId
    // GetUniqueNetIdForPlatformUser():스팀에서 사용하는 고유 번호
    // 현재 컨트롤러의 첫번째 로컬 플레이어의 고유한 네트워크 ID 가져옴
    FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

    sessionInterface->CreateSession( *NetID, FName(*HostName) , settings);
    
    PRINTLOG(TEXT("Create Session Start %s, Host Name : %s"), *RoomName, *HostName);
}

void UVirtualGameInstance_KMK::OnMyCreateSessionComplete ( FName SessionName , bool bSuccessful )
{
    if ( bSuccessful )
    {
        PRINTLOG(TEXT("OnMyCreateSessionComplete"));

        // 서버가 여행을 떠나고 싶다.
        GetWorld ( )->ServerTravel ( TEXT ( "/Game/Project/Personal/KMK/Maps/KMK_TravelLevel?listen" ) );
    }
    else
    {
        PRINTLOG(TEXT("OnMyCreateSessionFailed"));
    }
}
#pragma endregion
#pragma region Find Session
void UVirtualGameInstance_KMK::FindOtherSession ( )
{
    // 세션 찾기
    sessionSearch = MakeShareable(new FOnlineSessionSearch);

    // 찾은 세션의 셋팅 : presence이용 방목록 검색
    // QuerySettings : 세션 검색에 사용할 조건을 저장
    // SEARCH_PRESENCE : 세션이 유저의 상태 정보를 포함하는지 여부 파악 => 친구가 온라인인 경우만 참여가능, true : 유저 상태 정보를 사용하는 곳에만 사용
    //  EOnlineComparisonOp::Equals : 검색 조건이 부합한지 => 여기서 조건은 유저 상태정보를 표현하는 세션만 사용한다는 의미
    sessionSearch->QuerySettings.Set ( SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals );
    // 온라인 서브 시스템이 존재하지 않으면 LAN 기반으로 세션을 검색함, 아니라면 인터넷 기반 세션으로 사용
    sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
    // 최대 찾는 방 개수
    sessionSearch->MaxSearchResults = 20;
    // 플레이어의 닉네임 0 : 로컬플레이어 기준으로 네트워크 세션 검색
    sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
    // 델리게이트에 바인딩된 함수가 있다면
    if ( OnFindRoomCompDelegate.IsBound() )
    {
        // 방을 찾았다는 정보를 전달함 => 위잿 생성하게 만들게 하면되는듯
        OnFindRoomCompDelegate.Broadcast(true);
    }
}

void UVirtualGameInstance_KMK::OnMyFindSessionComplete ( bool bSuccessful )
{
    if (bSuccessful)
    {
        // 찾은 방정보 생성
        TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;
        for (int32 i = 0; i < results.Num ( ); i++)
        {
            FOnlineSessionSearchResult result = results[i];
            // 방 검색 결과가 없다면 패스
            if(!result.IsValid()) continue;
            // 방정보 : session 내부에 존재
            FRoomInfo roomInfo;
            // 방의 인덱스
            roomInfo.index = i;
            // 방이름
            FString room;
            result.Session.SessionSettings.Get(FName("ROOM_NAME" ), room );
            roomInfo.roomName = StringBase64Decode(room);
            // 호스트 이름
            FString host;
            result.Session.SessionSettings.Get(FName("HOST_NAME" ), host );
            roomInfo.hostName = StringBase64Decode(host);
            // 최대 플레이어 수
            roomInfo.MaxPlayer = result.Session.SessionSettings.NumPublicConnections;
            // 핑정보
            roomInfo.pingMS = result.PingInMs;
            if (OnSearchSignatureCompleteDelegate.IsBound ( ))
            {
                OnSearchSignatureCompleteDelegate.Broadcast(roomInfo);
                
                PRINTLOG(TEXT("%s"), *roomInfo.ToString());
            }
        }
    }
    else
    {
        PRINTLOG(TEXT("OnMyFindSessionFailed"));
    }

    if ( OnFindRoomCompDelegate.IsBound() )
    {
        OnFindRoomCompDelegate.Broadcast(false);
    }

}
#pragma endregion
#pragma region Join Session

void UVirtualGameInstance_KMK::JoinRoom ( int32 ChooseRoomNum, int32 vipNum)
{
    playerMeshNum = vipNum;
    auto res = sessionSearch->SearchResults[ChooseRoomNum];
    sessionInterface->JoinSession(0, FName(HostName), res);
}


void UVirtualGameInstance_KMK::JoinRoomComplete ( FName SessionName , EOnJoinSessionCompleteResult::Type result )
{
    if (EOnJoinSessionCompleteResult::Success == result)
    {
        auto* pc = GetWorld()->GetFirstPlayerController();

        FString url;
        sessionInterface->GetResolvedConnectString(SessionName, url);
        if (!url.IsEmpty ( ))
        {
            pc->ClientTravel(url , ETravelType::TRAVEL_Absolute);
        }
    }
}

void UVirtualGameInstance_KMK::ExitRoom ( )
{
    ServerRPCExitRoom();
}

void UVirtualGameInstance_KMK::ServerRPCExitRoom_Implementation ( )
{
    MultiRPCExitRoom();
}

void UVirtualGameInstance_KMK::MultiRPCExitRoom_Implementation ( )
{
    // 방퇴장 요청
    sessionInterface->DestroySession(FName(HostName));
}

void UVirtualGameInstance_KMK::OnMyDestroyRoomComplete ( FName RoomName , bool bWasSucessful )
{
    if ( bWasSucessful )
    {
        // 로비로 돌아가고 싶다 = 클라이언트가 여행을 갈것이다.
        auto* pc = GetWorld()->GetFirstPlayerController();
        pc->ClientTravel(TEXT("/Game/Project/Personal/KMK/Maps/KMK_Maps.KMK_Maps'?listen"), ETravelType::TRAVEL_Absolute);
        // 방을 만들었다면 방을 부수고 아니라면 그냥 나감
    }
}

#pragma endregion
#pragma region Encoding & Decoding for steam
FString UVirtualGameInstance_KMK::StringBase64Encode(const FString& str)
{
    // Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
    std::string utf8String = TCHAR_TO_UTF8(*str);
    TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()) ,
    utf8String.length());
    return FBase64::Encode(arrayData);

}

FString UVirtualGameInstance_KMK::StringBase64Decode(const FString& str)
{
    // Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
    TArray<uint8> arrayData;
    FBase64::Decode(str , arrayData);
    std::string ut8String((char*)(arrayData.GetData()) , arrayData.Num());
    return UTF8_TO_TCHAR(ut8String.c_str());

}

void UVirtualGameInstance_KMK::SetStartWidget ( class UStartWidget_KMK* startUi )
{
    widget = startUi;
}

void UVirtualGameInstance_KMK::SwitchStartUIWidget (int32 num )
{
    SwitchWidget ( 4 );
    widget->roomNum = num;
}

void UVirtualGameInstance_KMK::VisibleStartWidget (bool bVisible)
{
    if (!bVisible)
    {
        widget->ClearSB();
        // 위잿이미지 변경
        SwitchWidget ( 5 );
    }
}

void UVirtualGameInstance_KMK::SwitchWidget ( int32 num )
{
    widget->StartSwitcher->SetActiveWidgetIndex ( num );
}

void UVirtualGameInstance_KMK::PopUpVisible ( )
{
    widget->PayPopUpPanel->SetVisibility(ESlateVisibility::Visible);
}

void UVirtualGameInstance_KMK::LoginPanel ( )
{
    widget->FailLoginPanel->SetVisibility(ESlateVisibility::Visible);
}

#pragma endregion
#pragma region Token
void UVirtualGameInstance_KMK::SetMyInfo (const struct FLoginInfo& info )
{
    loginInfo.userId = info.userId;
    loginInfo.email = info.email;
    loginInfo.password = info.password;
    loginInfo.token = info.token;
    loginInfo.userName = info.userName;
}

FLoginInfo UVirtualGameInstance_KMK::GetMyInfo ( )
{
    return loginInfo;
}

void UVirtualGameInstance_KMK::SetConcertInfo ( const struct FConcertInfo& info )
{
    concerInfo = info;
}

#pragma endregion
#pragma region Chat
void UVirtualGameInstance_KMK::SetWidget ( class UAudience_KMK* wid )
{
	myWidget = wid;
}

#pragma endregion

FString UVirtualGameInstance_KMK::GetLocalIPAddress ( )
{
    bool bCanBindAll;
    TSharedPtr<class FInternetAddr> Addr = ISocketSubsystem::Get ( PLATFORM_SOCKETSUBSYSTEM )->GetLocalHostAddr ( *GLog , bCanBindAll );
    return Addr->ToString ( false );
}

void UVirtualGameInstance_KMK::SendMulticastMessage ( )
{
    FSocket* Socket = ISocketSubsystem::Get ( PLATFORM_SOCKETSUBSYSTEM )->CreateSocket ( NAME_DGram , TEXT ( "MulticastSocket" ) , false );
    TSharedRef<FInternetAddr> MulticastAddr = ISocketSubsystem::Get ( PLATFORM_SOCKETSUBSYSTEM )->CreateInternetAddr ( );

    bool bIsValid;
    MulticastAddr->SetIp ( TEXT ( "239.1.1.1" ) , bIsValid ); // 멀티캐스트 주소
    MulticastAddr->SetPort ( 5500 );

    if (bIsValid)
    {
        int32 Sent;
        FString Message = TEXT ( "Hello, Multicast!" );
        TArray<uint8> Data;
        Data.Append ( (uint8*)TCHAR_TO_UTF8 ( *Message ) , Message.Len ( ) );

        Socket->SendTo ( Data.GetData ( ) , Data.Num ( ) , Sent , *MulticastAddr );
    }

    Socket->Close ( );
    ISocketSubsystem::Get ( PLATFORM_SOCKETSUBSYSTEM )->DestroySocket ( Socket );
}


void UVirtualGameInstance_KMK::OnJoinSessionButt( )
{
	SwitchStartUIWidget(roomNum);
}
// 무대 선택에서 무대선택한 경우
void UVirtualGameInstance_KMK::OnSetStageButt ( )
{
	// 원래 보이던 위잿을 비활성화
	UE_LOG(LogTemp, Warning, TEXT("Load Stage" ));
	UE_LOG(LogTemp, Warning, TEXT("%s" ), *myStageInfo.name);
	// 추가되어야하는것 : 저장된 무대를 로드
	if (sm && !myStageInfo.name.IsEmpty())
	{
		VisibleStartWidget(false);
		sm->CreateStage(myStageInfo);
	}
}

void UVirtualGameInstance_KMK::SetMyProfile ( )
{
    widget->ChangeMyProfile();
}
