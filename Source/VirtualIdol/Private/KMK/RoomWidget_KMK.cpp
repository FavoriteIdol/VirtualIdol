// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/RoomWidget_KMK.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KMK/HttpActor_KMK.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "VirtualIdol.h"
#include "JJH/JJH_SelectManager.h"

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
	Image_Stage->SetColorAndOpacity(FLinearColor::Blue);
	mySessionInfo = info;
	Butt_JoinSession->SetVisibility ( ESlateVisibility::Visible );
	Butt_SetStage->SetVisibility(ESlateVisibility::Hidden);
	Text_Name->SetText( FText::FromString( info.hostName + TEXT ( "의 콘서트" ) ));
	gi->roomNum = info.index;
}

void URoomWidget_KMK::SetStageText( const struct FStageInfo& stageInfo, UTexture2D* image)
{
	if(!gi) gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	if(gi && gi->sm && !sm)gi->sm = sm;
	myStageInfo = stageInfo;
	if(image)Image_Stage->SetBrushFromTexture(image);
	myTexture = image;
	Butt_JoinSession->SetVisibility ( ESlateVisibility::Hidden );
    Butt_SetStage->SetVisibility ( ESlateVisibility::Visible );
	Text_Name->SetText( FText::FromString( stageInfo.name ));
}

void URoomWidget_KMK::PressJoinSessionButt ( )
{
	if (gi->bPressSession && gi->sessionWidget != this)
	{
		gi->sessionWidget->Image_StageOut->SetVisibility(ESlateVisibility::Hidden);
		gi->sessionWidget = nullptr;
		gi->bPressSession = false;
	}
	if(!gi->bPressSession)
	{
		ChangeSessionOutSide( );
	}
	else
	{
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
    gi->myStageInfo = myStageInfo;
	gi->stageNum = myStageInfo.stageID;
	gi->ChangeTextureWidget(myTexture);
    Image_StageOut->SetVisibility ( ESlateVisibility::Visible );
}

void URoomWidget_KMK::ChangeSessionOutSide ( )
{
	gi->bPressSession = true;
    gi->sessionWidget = this;
    gi->mySessionInfo = mySessionInfo;
    Image_StageOut->SetVisibility ( ESlateVisibility::Visible );
}

