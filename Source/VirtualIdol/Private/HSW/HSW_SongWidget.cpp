// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_SongWidget.h"
#include "Components/Button.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/Virtual_KMK.h"
#include "Components/TextBlock.h"

void UHSW_SongWidget::NativeConstruct ( )
{
	Super::NativeConstruct ( );
	gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );

	if (Button_Play)
	{
		Button_Play->OnClicked.AddDynamic ( this , &UHSW_SongWidget::MusicStart );
	}

	if (Button_Next)
	{
		Button_Next->OnClicked.AddDynamic ( this , &UHSW_SongWidget::SetMusic );
	}

    FindVirtualCharacter( );

    Text_CurrentSongTitle->SetText ( VirtualCharacter->GetCurrentSongTitle ( ) );
}

void UHSW_SongWidget::MusicStart ( )
{
    UE_LOG(LogTemp, Warning, TEXT("UI: MusicStart.") );
    VirtualCharacter->CreateAudioActor();
}

void UHSW_SongWidget::SetMusic ( )
{
    VirtualCharacter->SetCurrentSongIndex();
    Text_CurrentSongTitle->SetText( VirtualCharacter->GetCurrentSongTitle() );
}

void UHSW_SongWidget::FindVirtualCharacter ( )
{
    TArray<AActor*> actorArray;
    // 태그로 검색
    UGameplayStatics::GetAllActorsWithTag ( GetWorld ( ) , TEXT ( "Virtual" ) , actorArray );
    for (AActor* actor : actorArray)
    {
        // 버츄얼을 찾았다면 버츄얼에 달린 component를 검색함
        UVirtual_KMK* virtualComp = actor->FindComponentByClass<UVirtual_KMK> ( );
        if (virtualComp)
        {
            // 버츄얼 캐릭터가 있다면 안보이게 만듦
            VirtualCharacter = virtualComp;
            VirtualCharacter->SetVirtualVisible ( false );
            //UE_LOG ( LogTemp , Warning , TEXT ( "Found Virtual Character: %s" ) , *actor->GetName ( ) );
            return; // 성공적으로 찾았으므로 종료
        }
    }
    UE_LOG ( LogTemp , Warning , TEXT ( "Virtual Character not found, retrying..." ) );
    // 버츄얼을 못찾았다면, 일정 시간 후 다시 시도
    GetWorld ( )->GetTimerManager ( ).SetTimerForNextTick ( this , &UHSW_SongWidget::FindVirtualCharacter );
}
