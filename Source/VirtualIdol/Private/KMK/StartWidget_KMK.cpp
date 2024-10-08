// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/StartWidget_KMK.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Engine/TimerHandle.h"
#include "KMK/RoomWidget_KMK.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Engine/Texture2D.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Components/TextBlock.h"

void UStartWidget_KMK::NativeConstruct ( )
{
    Super::NativeConstruct();
	// GI 찾기
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance() );
#pragma region LoginPanel
	if (Butt_Login)
	{
		Butt_Login->OnClicked.AddDynamic(this, &UStartWidget_KMK::OnMyLogin);
	}
#pragma endregion

#pragma region FourButtPanel
	if (Butt_CreateStage)
	{
		Butt_CreateStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::CreateStagePanel );
	}
	if (Butt_ReadyConcert)
	{
		Butt_ReadyConcert->OnClicked.AddDynamic ( this , &UStartWidget_KMK::SettingStagePanel );
	}
	if (Butt_StartConcert)
	{
		Butt_StartConcert->OnClicked.AddDynamic ( this , &UStartWidget_KMK::StartConcertPanel );
	}
	if (Butt_ComeInStage)
	{
		Butt_ComeInStage->OnClicked.AddDynamic ( this , &UStartWidget_KMK::ComeInStagePanel );
	}
#pragma endregion
#pragma region  Setting StagePanel
	if (Butt_Back)
	{
		Butt_Back->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
	}
	if (Butt_CreateTicket)
	{
		Butt_CreateTicket->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressCreateTicket );
	}
	if (Butt_Complete)
	{
		Butt_Complete->OnClicked.AddDynamic ( this , &UStartWidget_KMK::CompeleteSetting );
	}

	if (Butt_Right && Butt_FRight)
	{
		Butt_Right->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressRight );
		Butt_FRight->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressFRight );
	}
	if (Butt_Left && Butt_FLeft)
	{
		Butt_Left->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressLeft );
		Butt_FLeft->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressFLeft );
	}
	if (Butt_ActivePanel)
	{
		Butt_ActivePanel->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PanelActive );
	}
	if (Image_Fever &&Image_Particle &&Image_Ticket)
	{
		Image_Fever->SetColorAndOpacity ( FLinearColor::Blue );
		Image_Particle->SetColorAndOpacity ( FLinearColor::Blue );
		Image_Ticket->SetColorAndOpacity ( FLinearColor::White );
	}

#pragma endregion
#pragma region Entry
	if (Butt_Yes)
	{
		Butt_Yes->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressYesButt );
	}
	if (Butt_No)
	{
		Butt_No->OnClicked.AddDynamic ( this , &UStartWidget_KMK::PressNoButt );
	}	
	if (Butt_Back1 && Butt_Back2)
	{
		Butt_Back1->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
		Butt_Back2->OnClicked.AddDynamic ( this , &UStartWidget_KMK::GoBack );
	}
#pragma endregion
#pragma region Session
	if (gi)
	{
		gi->OnSearchSignatureCompleteDelegate.AddDynamic(this, &UStartWidget_KMK::CreateRoomWidget );
		gi->SetStartWidget(this);
	}

#pragma endregion


}
#pragma region BackFunction
void UStartWidget_KMK::GoBack ( )
{
    StartSwitcher->SetActiveWidgetIndex (1);
	ClearAll();
	ClearChild();
}
#pragma endregion


#pragma region LoginPanel

void UStartWidget_KMK::OnMyLogin ( )
{
    // 아이디와 패스워드가 없으면 실행 안되게 하기
	if (!EditText_ID->GetText ( ).IsEmpty ( ) && !EditText_PW->GetText ( ).IsEmpty ( ))
	{
		// 서버에 정보값 보내기

		// 선택지로 변경
		StartSwitcher->SetActiveWidgetIndex ( 1 );
	}
}

#pragma endregion

#pragma region FourButtPanel
// 무대 꾸미기 레벨로 이동
void UStartWidget_KMK::CreateStagePanel ( )
{

}
// 공연 일정 잡는 판넬로 변경
void UStartWidget_KMK::SettingStagePanel ( )
{
	StartSwitcher->SetActiveWidgetIndex ( 3 );
	ChangeFindRoomPanel(TEXT("무대 선택" ));
}
// 공연 시작 : 세션 생성
void UStartWidget_KMK::StartConcertPanel ( )
{
	if (gi)
	{
		gi->CreateMySession(TEXT("1234"), 30);
	}
}

void UStartWidget_KMK::ComeInStagePanel ( )
{	
	// 세션 찾기
	FindRoom();
}

#pragma endregion

#pragma region Setting StagePanel
// 레벨 선택
void UStartWidget_KMK::ChangeFindRoomPanel ( const FString& title )
{
	Text_Title->SetText(FText::FromString(title));
	// 파섹해서 나온 정보의 값에 따라 createWidget이 되게 만들기
	for (int i = 0; i < 5; i++)
	{
		CreateStageWidget(FString::FromInt(i));
	}
}

void UStartWidget_KMK::CreateStageWidget ( const FString& createName )
{
	// wbp 생성
	auto* wid = Cast<URoomWidget_KMK>(CreateWidget(GetWorld(), roomWidgetFact));
	if (wid)
	{
		wid->SetStageText(createName);
		wid->ActiveButton(false );
		roomCount++;
		SetPosWidget(wid, roomCount);
	}
}

// 파티클 설정하는 부분
void UStartWidget_KMK::PressRight ( )
{
	// 버튼 이름 확인
	if (EffectParticles.IsEmpty ( )) return;
	particleNum++;
	if (particleNum > EffectParticles.Num ( )) particleNum = 0;
	PlayParticleSystem ( particleNum , EffectParticles, Image_Particle );
}

void UStartWidget_KMK::PressLeft ( )
{
	if (EffectParticles.IsEmpty ( )) return;
	particleNum--;
	if (particleNum < 0) particleNum = EffectParticles.Num ( ) - 1;
	PlayParticleSystem ( particleNum , EffectParticles, Image_Particle );
}

void UStartWidget_KMK::PressFRight ( )
{
	if (FeversParticles.IsEmpty ( )) return;
	feverNum++;
	if (feverNum > FeversParticles.Num ( )) feverNum = 0;
	PlayParticleSystem ( feverNum , FeversParticles, Image_Fever );
}

void UStartWidget_KMK::PressFLeft ( )
{
	if (FeversParticles.IsEmpty ( )) return;
	feverNum--;
	if (feverNum < 0) feverNum = FeversParticles.Num ( ) - 1;
	PlayParticleSystem ( feverNum , FeversParticles, Image_Fever );
}

void UStartWidget_KMK::PressCreateTicket ( )
{
	if(EditText_Ticket->GetText().IsEmpty()) return;
	// 서버에 티켓을 생성하라고 보내줌
	Image_Ticket->SetColorAndOpacity(FLinearColor::Yellow);
}

void UStartWidget_KMK::CompeleteSetting ( )
{
	// 서버에 셋팅 내용 보내기
	// 완료 되면 팝업 띄우기
	CompletePopUp->SetVisibility(ESlateVisibility::Visible);

	FTimerHandle handle;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( handle , FTimerDelegate::CreateLambda ( [this]( )
		{
			StartSwitcher->SetActiveWidgetIndex ( 1 );
			CompletePopUp->SetVisibility ( ESlateVisibility::Hidden );
			ClearAll( );
		} ) , 3 , false );
}

void UStartWidget_KMK::PlayParticleSystem ( int32 index , TArray<class UParticleSystem*> ParticlesArray, class UImage* image )
{	
	if(ParticlesArray.Num() < 1 || index > ParticlesArray.Num()) return;
	//// 파티클 시스템 스폰
	//UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation (
	//	GetWorld ( ) ,  // 파티클을 스폰할 월드
	//	ParticlesArray[index] ,  // 재생할 파티클 시스템
	//	ParticlePositions[num] ,  // 파티클이 생성될 위치
	//	FRotator ( 0 , 0 , 0 )  // 파티클의 회전
	//);
	//if (ParticleComponent)
	//{
	//	ParticleComponent->ActivateSystem ( );  // 파티클 재생
	//}
	switch (index)
	{
	case 0:
		image->SetColorAndOpacity ( FLinearColor::Blue );
		break;
	case 1:
		image->SetColorAndOpacity ( FLinearColor::Black );
		break;
	case 2:
		image->SetColorAndOpacity ( FLinearColor::Red );
		break;
	}

}

void UStartWidget_KMK::ClearAll ( )
{
	particleNum = 0;
	feverNum = 0;
	
	// 이미지 초기화
	Image_Fever->SetColorAndOpacity ( FLinearColor::Blue );
	Image_Particle->SetColorAndOpacity ( FLinearColor::Blue );
	Image_Ticket->SetColorAndOpacity ( FLinearColor::White );

	// 텍스트 초기화
	EditText_Day->SetText(FText::GetEmpty());
	EditText_STime->SetText(FText::GetEmpty());
	EditText_ETime->SetText(FText::GetEmpty());
	EditText_Ticket->SetText(FText::GetEmpty());
	EditText_SingTime->SetText(FText::GetEmpty());
}
#pragma endregion

#pragma region Entry Panel
// 세션 확인할 수 있는 UI 띄우기
void UStartWidget_KMK::PressYesButt ( )
{
	if (gi && roomNum >= 0)
	{
		gi->JoinRoom(roomNum);
		ChangeAudienceMesh(0);
	}
}

void UStartWidget_KMK::PressNoButt ( )
{
	if (gi && roomNum >= 0)
	{
		gi->JoinRoom(roomNum);
		ChangeAudienceMesh(1);
	}
}

// 세션 찾기
void UStartWidget_KMK::FindRoom( )
{
	// VIP 입장 판넬 띄우기
	StartSwitcher->SetActiveWidgetIndex ( 3 );
	ChangeFindRoomPanel(TEXT("공연장 확인" ));
	ClearChild();
	if (gi)
	{
		gi->FindOtherSession();
	}
}
// 세션을 만들면 wbp를 생성해서 그리드에 배치
void UStartWidget_KMK::CreateRoomWidget (const struct FRoomInfo& info )
{
	// wbp 생성
	auto* wid = Cast<URoomWidget_KMK>(CreateWidget(GetWorld(), roomWidgetFact));
	if (wid)
	{
		wid->SetImageAndText(info);
		wid->ActiveButton(true );
		roomCount++;
		SetPosWidget(wid, roomCount);
	}
}
// 생성된 wbp의 개수에 따라 위치값 조정
void UStartWidget_KMK::SetPosWidget ( class URoomWidget_KMK* widget , int32 num )
{
	// row 계산 
	int32 row = ( num - 1 ) / 3;
	// col 계산
	int32 col = (num - 1) % 3;
	// 위치 설정
	UGridSlot* childSlot = FindRoomGrid->AddChildToGrid( widget );
	if (childSlot)
	{
		childSlot->SetRow(row);
		childSlot->SetColumn(col);
	}
	
}
// 그리드 삭제
void UStartWidget_KMK::ClearChild ( )
{
	if (FindRoomGrid->GetChildrenCount ( ) > 0)
	{
		roomCount = 0;
		FindRoomGrid->ClearChildren();
	}
}

void UStartWidget_KMK::ChangeAudienceMesh (int32 num )
{
	auto* player = GetWorld ( )->GetFirstPlayerController( )->GetPawn()->FindComponentByClass<USkeletalMeshComponent> ();
	if (player)
	{
		player->SetSkeletalMesh(audienceMesh[num]);
	}
}

void UStartWidget_KMK::PanelActive ( )
{
	StartSwitcher->SetActiveWidgetIndex ( 3 );
}

#pragma endregion



