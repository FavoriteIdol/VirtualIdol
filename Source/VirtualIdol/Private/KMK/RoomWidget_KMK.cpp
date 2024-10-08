// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/RoomWidget_KMK.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "VirtualIdol.h"

void URoomWidget_KMK::NativeConstruct ( )
{
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	if (Butt_JoinSession && Butt_SetStage)
	{
		Butt_JoinSession->OnClicked.AddDynamic ( this , &URoomWidget_KMK::PressJoinSessionButt );
		Butt_SetStage->OnClicked.AddDynamic ( this , &URoomWidget_KMK::PressSetStageButt );
	}
}
// 세션에 들어가는 버튼에 이미지와 텍스트를 변경함
void URoomWidget_KMK::SetImageAndText (const struct FRoomInfo& info)
{
	// 이미지 설정
	//Image_Stage->SetBrushFromTexture( newTexture );
	Image_Stage->SetColorAndOpacity(FLinearColor::Blue);
	 PRINTLOG(TEXT("%s"), *info.hostName);
	Text_Name->SetText( FText::FromString( info.hostName + TEXT ( "의 콘서트" ) ));
	roomNum = info.index;
}

void URoomWidget_KMK::SetStageText ( const FString& createName )
{
	Image_Stage->SetColorAndOpacity(FLinearColor::Yellow);
	 PRINTLOG(TEXT("%s"), *createName);
	Text_Name->SetText( FText::FromString( createName + TEXT ( "가 만든 무대" ) ));
}

void URoomWidget_KMK::PressJoinSessionButt( )
{
	if (gi)
	{
		gi->SwitchStartUIWidget(roomNum);
	}

}
// 무대 선택에서 무대선택한 경우
void URoomWidget_KMK::PressSetStageButt ( )
{
	// 원래 보이던 위잿을 비활성화
	// 추가되어야하는것 : 저장된 무대를 로드
	if (gi)
	{
		gi->VisibleStartWidget(false);
	}
}

// 버튼 변경
void URoomWidget_KMK::ActiveButton ( bool bJoinSessionButt )
{
	if (bJoinSessionButt)
	{
		Butt_JoinSession->bIsEnabled = true;
		Butt_SetStage->bIsEnabled = false;
	}
	else
	{
		Butt_JoinSession->bIsEnabled = false;
		Butt_SetStage->bIsEnabled = true;
	}
}

