// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW_AudioLoadingActor.h"
#include "MediaSoundComponent.h"
#include "MediaPlayer.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/Virtual_KMK.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AHSW_AudioLoadingActor::AHSW_AudioLoadingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent> ( TEXT ( "RootSceneComponent" ) );
	RootComponent = SceneComponent;

	MediaSoundComp = CreateDefaultSubobject<UMediaSoundComponent> ( TEXT ( "MediaSoundComp" ) );
	MediaSoundComp->SetupAttachment ( RootComponent );

	bReplicates = true;
}

// Called when the game starts or when spawned
void AHSW_AudioLoadingActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG ( LogTemp , Error , TEXT ( "AudioActorCreate" ) );

	gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );

	FindVirtualCharacter ( );
// 
// 	FTimerHandle timerHandle;
// 	GetWorld ( )->GetTimerManager ( ).SetTimer ( timerHandle , this , &AHSW_AudioLoadingActor::PlayWavFile , 0.3f , false );

	if (HasAuthority ( ))
	{
		SongFilePath = FPaths::Combine(FPaths::ProjectSavedDir ( ) ,TEXT ( "Music" ), FString::Printf ( TEXT ( "%d_%d_%s.wav" ) ,
			VirtualCharacter->SongInfo.ConcertID , // Concert ID
			VirtualCharacter->SongInfo.SongID ,   // Song ID
			*VirtualCharacter->SongInfo.Title // Title
		) );
		ServerRPC_PlayWaveFile( );
	}

}

// Called every frame
void AHSW_AudioLoadingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHSW_AudioLoadingActor::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );	

}

void AHSW_AudioLoadingActor::ServerRPC_PlayWaveFile_Implementation ( )
{
	MultiRPC_PlayWavFile( );
}

void AHSW_AudioLoadingActor::MultiRPC_PlayWavFile_Implementation ( )
{
	if (!MediaPlayer)
	{
		MediaPlayer = NewObject<UMediaPlayer> ( );
		if (MediaPlayer)
		{
			MediaPlayer->OnEndReached.AddDynamic ( this , &AHSW_AudioLoadingActor::OnPlayEnded );
		}

	}
	//UE_LOG ( LogTemp , Warning , TEXT ( "PlayWavFile" ) );
 	UE_LOG ( LogTemp , Warning , TEXT ( "Wav File Path: %s" ) , *SongFilePath );
	if (MediaPlayer && MediaPlayer->OpenFile ( SongFilePath ))
	{
		if (MediaSoundComp)
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "SongPlay" ) );
			MediaSoundComp->SetMediaPlayer ( MediaPlayer );
			MediaPlayer->Play();
		}
	}
}

void AHSW_AudioLoadingActor::OnPlayEnded ( )
{
	MediaPlayer->Close ( );
	VirtualCharacter->bCanPlaySong=true;
	this->Destroy();
}

void AHSW_AudioLoadingActor::FindVirtualCharacter ( )
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
	GetWorld ( )->GetTimerManager ( ).SetTimerForNextTick ( this , &AHSW_AudioLoadingActor::FindVirtualCharacter );
}

