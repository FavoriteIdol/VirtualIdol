// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_SongUnit.h"
#include "Components/Button.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/Virtual_KMK.h"
#include "Components/TextBlock.h"

void UHSW_SongUnit::NativeConstruct ( )
{
    Super::NativeConstruct ( );
    gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
    FindVirtualCharacter ( );

    if (Button_Play && !Button_Play->OnClicked.IsAlreadyBound ( this , &UHSW_SongUnit::MusicStart ))
    {
        Button_Play->OnClicked.AddDynamic ( this , &UHSW_SongUnit::MusicStart );
    }

    if (Button_Stop && !Button_Stop->OnClicked.IsAlreadyBound ( this , &UHSW_SongUnit::MusicStop ))
    {
        Button_Stop->OnClicked.AddDynamic ( this , &UHSW_SongUnit::MusicStop );
    }

}

void UHSW_SongUnit::MusicStart ( )
{
    VirtualCharacter->CreateAudioActor (SongInfo);
}

void UHSW_SongUnit::MusicStop ( )
{
    VirtualCharacter->DestroyAudioActor ( );
}

void UHSW_SongUnit::FindVirtualCharacter ( )
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
            return; // 성공적으로 찾았으므로 종료
        }
    }
    UE_LOG ( LogTemp , Warning , TEXT ( "Virtual Character not found, retrying..." ) );
    // 버츄얼을 못찾았다면, 일정 시간 후 다시 시도
    GetWorld ( )->GetTimerManager ( ).SetTimerForNextTick ( this , &UHSW_SongUnit::FindVirtualCharacter );
}

void UHSW_SongUnit::SetSongTitle ( )
{
    if (SongInfo.Title.IsEmpty()) return;
    Text_SongTitle->SetText ( FText::FromString(SongInfo.Title ));
}
