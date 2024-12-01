// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/RoomWidget_KMK.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KMK/HttpActor_KMK.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "VirtualIdol.h"
#include "JJH/JJH_SelectManager.h"
/*
* 이 위잿은 2가지로 사용되고 있음
* 1. 만들어져 있는 공연장 확인용 : SetStageText
* 2. 현재 열린 세션 확인용 : SetImageAndText
*/
void URoomWidget_KMK::NativeConstruct ( )
{
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	if (Butt_JoinSession && Butt_SetStage && Image_StageOut)
	{
		Butt_JoinSession->OnClicked.AddDynamic ( this , &URoomWidget_KMK::PressJoinSessionButt );
		Butt_SetStage->OnClicked.AddDynamic ( this , &URoomWidget_KMK::PressSetStageButt );
		Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
		gi->bPressSession = false;
		gi->bPressStage = false;
	}
}
// 세션에 들어가는 버튼에 이미지와 텍스트를 변경함
void URoomWidget_KMK::SetImageAndText (const struct FRoomInfo& info)
{
	// 이미지 설정
	//Image_Stage->SetBrushFromTexture( newTexture );
	if(!gi) gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	if(!gi->sm)gi->sm = sm;
	
	if (info.texture)
	{
		Image_Stage->SetBrushFromTexture( info.texture );
	}
	
	// 세션정보를 할당하는 ㅂ분
	mySessionInfo = info;
	// 세션과 관련된 버튼이 on되고 관련없는 버튼이 Off
	Butt_JoinSession->SetVisibility ( ESlateVisibility::Visible );
	Butt_SetStage->SetVisibility(ESlateVisibility::Hidden);

	// 세션의 roomName이 작성되는 부분
	//Text_Name->SetText( FText::FromString( TEXT("STARLIGHT ARIN" ) ));
	Text_Name->SetText( FText::FromString ( *mySessionInfo.roomName ) );
}

// 스테이지 정보값을 불러와 셋팅하는 부분
void URoomWidget_KMK::SetStageText( const struct FStageInfo& stageInfo, UTexture2D* image)
{
	// gi 내에 있는 정보값을 가져오기 위해 찾기
	if(!gi) gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	if(gi && gi->sm && !sm)gi->sm = sm;
	// 공연장 정보를 담아놓음
	myStageInfo = stageInfo;
	// 이미지에 미리보기 텍스쳐 입히기
	if(image)Image_Stage->SetBrushFromTexture(image);
	myTexture = image;
	// 세션과 관련된 버튼이 Off되고 관련없는 버튼이 on
	Butt_JoinSession->SetVisibility ( ESlateVisibility::Hidden );
    Butt_SetStage->SetVisibility ( ESlateVisibility::Visible );
	Text_Name->SetText( FText::FromString( stageInfo.name ));
}

// 세션 버튼 함수
void URoomWidget_KMK::PressJoinSessionButt ( )
{	
	
	if (gi->bPressSession && gi->sessionWidget != this)
	{
		// 클릭이 된 경우, 클릭 확인용 outline이 켜짐
		gi->sessionWidget->Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
		gi->sessionWidget = nullptr;
		gi->bPressSession = false;
	}
	if(!gi->bPressSession)
	{
		// 아웃라인이 없을때 클릭된 경우
		ChangeSessionOutSide( );
	}
	else
	{
		// 아웃라인이 있을때 클릭된 경우,
		// bPressSession을 이용해 확인함
		gi->bPressSession = false;
		gi->sessionWidget = nullptr;
		Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URoomWidget_KMK::PressSetStageButt ( )
{
	if (gi->roomWidget && gi->roomWidget != this)
	{
		gi->roomWidget->Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
		gi->roomWidget = nullptr;
		gi->bPressStage = false;
	}
	if(!gi->bPressStage)
	{
		ChangeMyOutSide( );
	}
	else
	{
		gi->bPressStage = false;
		gi->roomWidget = nullptr;
		Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URoomWidget_KMK::ChangeMyOutSide ( )
{
	gi->bPressStage = true;
    gi->roomWidget = this;
	// 내 공연장 정보를 gi에 저장
    gi->myStageInfo = myStageInfo;
	gi->stageNum = myStageInfo.stageID;
	// 텍스처 변경
	gi->ChangeTextureWidget(myTexture);
    Image_StageOut->SetVisibility ( ESlateVisibility::Visible );
}

void URoomWidget_KMK::ChangeSessionOutSide ( )
{	
	gi->bPressSession = true;
    gi->sessionWidget = this;
	// gi에 있는 mySessionInfo값에 할당함
    gi->mySessionInfo = mySessionInfo;
	// 내가 가진 콘서트 정보를 gi로 이동 시킴
	gi->concerInfo.feverVFX = mySessionInfo.feverNum;
	gi->roomNum = mySessionInfo.index;
	gi->HostName = mySessionInfo.hostName;
	gi->ChangeTextureWidget(mySessionInfo.texture);
	// 더미 이름 변경
	//gi->ChangeTextureWidget(dummyText);
	// 아웃라인 생성
    Image_StageOut->SetVisibility ( ESlateVisibility::Visible );
}

