// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/Virtual_KMK.h"
#include "chrono"
#include "KMK/Audience_KMK.h"
#include "HSW/HSW_GameState_Auditorium.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerState.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "JJH/JJH_SelectManager.h"
#include "KMK/VirtualGameInstance_KMK.h"
#include "GameFramework/SpringArmComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Sound/SoundWave.h"
#include "MediaSoundComponent.h"
#include "MediaPlayer.h"
#include "Internationalization/Text.h"
#include "Components/TextBlock.h"
#include "HSW_AudioLoadingActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UVirtual_KMK::UVirtual_KMK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UVirtual_KMK::BeginPlay()
{
	Super::BeginPlay();
	meshComp = GetOwner()->FindComponentByTag<USkeletalMeshComponent>(FName(TEXT("Mesh")));
	gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());
	FName tag = TEXT ( "InterviewLocation" );
	for (TActorIterator<AActor> It ( GetWorld ( ) , AStaticMeshActor::StaticClass ( ) ); It; ++It)
	{
		AActor* Actor = *It;
		if (IsValid ( Actor ) && Actor->ActorHasTag ( tag ))
		{
			StageLocation = Actor->GetTransform ( );
		}

	}

	pc = GetWorld ( )->GetFirstPlayerController ( );

	if (pc)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "name: %s" ) , *( GetName ( ) ) );
		StartVoiceChat( );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "PC none" ) );
	}
    if (sm)
    {
        sm->CreateDummyStage( gi->GetConcertStageInfo() );
// 		if (gi->GetConcertStageInfo ( ).theme == 3)
// 		{
// 			gi->spawnTrans = FTransform(FVector(0, 0, 2000 ) );
// 		}
// 		else if(gi->GetConcertStageInfo ( ).theme != 3)
// 		{
// 			gi->spawnTrans = FTransform(FVector(0) );
// 		}
		gi->GINowEffect = gi->effectArrayForEachFloor[gi->concertStageInfo.terrain];
    }
	//if (GetWorld ( )->GetCurrentLevel()->GetName ( ).Contains ( TEXT ( "Alpha" ) ))
	//{
	//	gi->spawnTrans = FTransform(FVector(0,0,2000 ) );
	//}
	SetWavFiles( );
	FTimerHandle timerHandle;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( timerHandle , this , &UVirtual_KMK::SetSongList , 0.3f , false );
}


// Called every frame
void UVirtual_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (pc->HasAuthority ( ) && pc->IsLocalController ( ))
    {
		diffTime = GetTimeDifference ( setConcertTime );
        if (virtualWidget)
        {
			virtualWidget->BeforeStartConcertCount(diffTime);
            // virtualWidget->SetConcertName ( gi->concerInfo.name );
            virtualWidget->SetConcertName ( TEXT("STARLIGHT ARIN" ) );
        }
    }
	//if (bTime)
	//{
	//	remainTime = GetWorld ( )->GetTimerManager ( ).GetTimerRemaining ( startCountDownHandle );
	//	if (virtualWidget)
	//	{
	//		if (remainTime >= 0.0f)
	//		{
	//			// 남은 시간을 위젯에 표시
	//			virtualWidget->CountDownText ( FString::FromInt ( remainTime ) );
	//		}
	//	}
	//}
	//else
	//{
	//	if (pc->HasAuthority ( ) && pc->IsLocalController ( ))
	//	{
	//		diffTime = GetTimeDifference ( setConcertTime );
 //           if (virtualWidget)
 //           {
 //               virtualWidget->BeforeStartConcertCount(diffTime);
	//			virtualWidget->SetConcertName(gi->concerInfo.name );
 //           }
	//	}
	//}
}
#pragma region Time
FString UVirtual_KMK::GetTimeDifference ( const FString& SetTime )
{
	// 현재 시스템 시간을 가져오기
	auto now = std::chrono::system_clock::now ( );
	std::time_t currentTime = std::chrono::system_clock::to_time_t ( now );

	// tm 구조체로 변환 (로컬 시간)
	std::tm localTime;
	localtime_s ( &localTime , &currentTime );

	// 설정한 시간 (예: "20:19:00")을 tm 구조체로 변환
	int32 SetHour , SetMinute , SetSecond;
	if (sscanf_s ( TCHAR_TO_ANSI ( *SetTime ) , "%d:%d:%d" , &SetHour , &SetMinute , &SetSecond ) != 3)
	{
		return FString ( "Invalid time format" );
	}
	FString CurrentTimeStr = FString::Printf ( TEXT ( "%02d:%02d:%02d" ) ,
	localTime.tm_hour , localTime.tm_min , localTime.tm_sec );
	//UE_LOG ( LogTemp , Warning , TEXT ( "Current Time: %s" ) , *CurrentTimeStr );

	// 현재 시간의 시분초를 설정된 시간으로 덮어씀
	std::tm setTime = localTime;  // 현재 날짜 정보를 사용
	setTime.tm_hour = SetHour;
	setTime.tm_min = SetMinute;
	setTime.tm_sec = SetSecond;

	// 설정된 시간을 std::chrono::time_point로 변환
	std::time_t setTimeT = std::mktime ( &setTime );
	auto setTimeChrono = std::chrono::system_clock::from_time_t ( setTimeT );

	// 현재 시간과 설정된 시간의 차이를 계산
	auto duration = now - setTimeChrono;
	auto secondsDifference = std::chrono::duration_cast<std::chrono::seconds>( duration ).count ( );

	// 차이를 시:분:초로 변환
	int32 hours = FMath::Abs ( static_cast<int32>( secondsDifference / 3600 ) );
	int32 minutes = FMath::Abs ( static_cast<int32>( ( secondsDifference % 3600 ) / 60 ) );
	int32 seconds = FMath::Abs ( static_cast<int32>( secondsDifference % 60 ) );
	//AHSW_ThirdPersonCharacter* playerCharacter = Cast<AHSW_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	//if (secondsDifference == 0)  // 시간이 같을 때
	//{
 //       if (!bVis)
 //       {
 //           bVis = true;
	//		ServerRPC_ChangeMyMesh ( 2 );
 //       }
	//}
	//else if (minutes == 0 && seconds < 6 && !bCount)
	//{
	//	ServerRPC_StartConcert ( );
	//	bCount = true;
	//}
	//else if (minutes == 0 && seconds > 58)
	//{
	//	
	//	if (playerCharacter && playerCharacter->HasAuthority ( ))
	//	{
	//		playerCharacter->audienceWidget->ChangeVirtualWidget ( );
	//	}
	//}
	// FString으로 변환
    FString TimeDifference = FString::Printf ( TEXT ( "%02d:%02d" ) , minutes , seconds );
	if(virtualWidget) virtualWidget->ChangeTextClock(TimeDifference );
	return TimeDifference;
}

void UVirtual_KMK::SetVirtualChat (const FString& nickName, const FString& text )
{
	if(virtualWidget)virtualWidget->CreateChatWidget(nickName, text);
}

void UVirtual_KMK::SetVirtualVisible ( bool bVisit /*= false */ )
{
	meshComp = GetOwner()->FindComponentByTag<USkeletalMeshComponent>(FName(TEXT("Mesh")));
	if (meshComp)
	{
		meshComp->SetRenderInMainPass ( bVisit );
		meshComp->SetRenderInDepthPass ( bVisit );
		meshComp->CastShadow = bVisit;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::White, FString::Printf(TEXT("No Mesh")));
	}
}

void UVirtual_KMK::ShowCoundDownPanel ( )
{
	if (!virtualWidget) return;
	virtualWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
	StartCountDown ( );
}

void UVirtual_KMK::CallGMShowServer ( )
{
	AHSW_GameState_Auditorium* gs = GetWorld ( )->GetGameState<AHSW_GameState_Auditorium> ( );
	if (gs)
	{
		gs->ServerRPC_ShowCountDown();
	}
}

void UVirtual_KMK::StartCountDown ( )
{
    SetVirtualVisible ( true );
    virtualWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
    /*if (gi->effectArray.Num ( ) > 0) GetWorld ( )->SpawnActor<AActor> ( gi->effectArray[gi->GetConcertInfo ( ).appearedVFX] , gi->spawnTrans );*/
	//if (gi->effectArray.Num ( ) > 0) GetWorld ( )->SpawnActor<AActor> ( gi->effectArray[3] ,FTransform(FVector(0, 0, 2000 )) );
    UE_LOG ( LogTemp , Warning , TEXT ( "%d" ) , gi->GetConcertInfo ( ).appearedVFX );

}

void UVirtual_KMK::PlayMusic ( USoundBase* wavFile )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Virtual : Play Music" ) );
	UAudioComponent* AudioComponent = NewObject<UAudioComponent> ( this );
	AudioComponent->SetSound ( wavFile );
	AudioComponent->RegisterComponent ( );
	AudioComponent->Play ( );  // 음원 재생
}

void UVirtual_KMK::SetInterviewee ( bool bInterview , APlayerState* interviewee, FTransform preLoc )
{
	if (bInterview)
	{
		//intervieweePlayer->CameraBoom->TargetArmLength = 0;
		interviewee->GetPawn ( )->SetActorTransform ( StageLocation );
		interviewee->GetPawn ( )->SetActorScale3D ( FVector ( 2.5 ) );

		if (APawn* pawn = interviewee->GetPawn ( ))
		{
			if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( pawn ))
			{
				Character->GetCameraBoom( )->TargetArmLength = 0;
				//Character->GetCameraBoom ( )->SetRelativeLocation(FVector(140,0,70));
				//UE_LOG ( LogTemp , Warning , TEXT ( "character name: %s" ) , *( Character->GetName() ) );
				//Character->StartVoiceChat ( );
			}
		}
	}
	else
	{
		interviewee->GetPawn ( )->SetActorTransform ( preLoc );
		interviewee->GetPawn ( )->SetActorScale3D ( FVector ( 1.0 ) );

		if (APawn* pawn = interviewee->GetPawn ( ))
		{
			if (AHSW_ThirdPersonCharacter* Character = Cast<AHSW_ThirdPersonCharacter> ( pawn ))
			{
				Character->GetCameraBoom ( )->TargetArmLength = 250;
				//UE_LOG ( LogTemp , Warning , TEXT ( "character name: %s" ) , *( Character->GetName ( ) ) );
				//Character->CancleVoiceChat ( );
			}
		}
	}
}

void UVirtual_KMK::StartVoiceChat ( )
{
	pc->StartTalking ( );
}

void UVirtual_KMK::CancleVoiceChat ( )
{
	pc->StopTalking( );
}


#pragma endregion

#pragma region Music

void UVirtual_KMK::SetWavFiles ( )
{
	FString DirectoryPath = FPaths::ProjectSavedDir ( ) + TEXT ( "Music" );
	TArray<FString> FileNames;
	IFileManager& FileManager = IFileManager::Get ( );

	// 디렉토리에서 모든 WAV 파일 검색
	FileManager.FindFiles ( FileNames , *DirectoryPath , TEXT ( ".wav" ) );

	WavFiles.Empty ( );

	for (const FString& FileName : FileNames)
	{
		TArray<FString> SplitName;
		FileName.ParseIntoArray ( SplitName , TEXT ( "_" ) );
		//FileName.Split ( TEXT ( "_" ) , &SplitName , nullptr , ESearchCase::IgnoreCase );

		for (FString Splited : SplitName)
		{
			UE_LOG ( LogTemp , Warning , TEXT ( "splited: %s" ), *Splited);
		}
		// 파일 이름에서 정보 추출 (형식: ConcertID_SongID_Title.wav)
		if (SplitName.Num ( ) >= 3)
		{

			int32 FileConcertID = FCString::Atoi ( *SplitName[0] );
			int32 SongID = FCString::Atoi ( *SplitName[1] );
			FString Title = SplitName[2].Left ( SplitName[2].Len ( ) - 4 ); // ".wav" 제거

			if (FileConcertID == gi->concerInfo.concertId)
			{

				FWavFileInfo CurrentFile;

				FString FilePath = DirectoryPath + "/" + FileName;

				CurrentFile.ConcertID = FileConcertID;
				CurrentFile.SongID = SongID;
				CurrentFile.Title = Title;
				CurrentFile.FilePath = FilePath;

				UE_LOG ( LogTemp , Warning , TEXT ( "Loaded: ConcertID=%d, SongID=%d, Title=%s, Path=%s" ) ,
	CurrentFile.ConcertID , CurrentFile.SongID , *CurrentFile.Title , *CurrentFile.FilePath );

				WavFiles.Add ( CurrentFile );
			}
		}
	}

	// 디버깅: 불러온 파일 로그 출력
	if (!WavFiles.IsEmpty ( ))
	{
		// 		for (const auto& WavFile : WavFiles)
		// 		{
		// 			UE_LOG ( LogTemp , Warning , TEXT ( "Loaded: ConcertID=%d, SongID=%d, Title=%s, Path=%s" ) ,
		// 				WavFile.ConcertID , WavFile.SongID , *WavFile.Title , *WavFile.FilePath );
		// 		}
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "'WavFiles' is Empty!" ) );
	}
}

void UVirtual_KMK::SetCurrentSongIndex ( )
{
	CurrentSongIndex++;
	if (CurrentSongIndex == WavFiles.Num ( )) CurrentSongIndex = 0;
}

FText UVirtual_KMK::GetSongTitle ( int SongIndex )
{
	return FText::FromString ( WavFiles[SongIndex].Title );
}

void UVirtual_KMK::CreateAudioActor ( FWavFileInfo currentSongInfo )
{
	FindAudioActor( );
	if (!AudioLoadingActor && bCanPlaySong)
	{
		SongInfo= currentSongInfo;
		AudioLoadingActor = GetWorld ( )->SpawnActor<AHSW_AudioLoadingActor> ( AudioLoadingActorFactory , FTransform::Identity );
		bCanPlaySong = false;
	}
}

void UVirtual_KMK::FindAudioActor ( )
{
	AudioLoadingActor = Cast<AHSW_AudioLoadingActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHSW_AudioLoadingActor::StaticClass( ) ));
}

void UVirtual_KMK::DestroyAudioActor ( )
{
	if(AudioLoadingActor) 
	{
		AudioLoadingActor->Destroy();
		bCanPlaySong = true;
	}
}


void UVirtual_KMK::SetSongList ( )
{
	UE_LOG(LogTemp,Error,TEXT("SetSongList 함수 실행" ));
	for (FWavFileInfo songInfo : WavFiles)
	{
		virtualWidget->AddSongList( songInfo );
	}
}

#pragma endregion

// USoundWave* UVirtual_KMK::WavToSoundWave ( const FString& FilePath )
// {
// 	TArray<uint8> RawFileData;
// 	if (!FFileHelper::LoadFileToArray ( RawFileData , *FilePath ))
// 	{
// 		UE_LOG ( LogTemp , Error , TEXT ( "Failed to load file: %s" ) , *FilePath );
// 		return nullptr;
// 	}
// 
// 	// WAV 파일 헤더 검증
// 	if (RawFileData.Num ( ) < 44 || FMemory::Memcmp ( RawFileData.GetData ( ) , "RIFF" , 4 ) != 0 ||
// 		FMemory::Memcmp ( RawFileData.GetData ( ) + 8 , "WAVE" , 4 ) != 0)
// 	{
// 		UE_LOG ( LogTemp , Error , TEXT ( "Invalid WAV file format: %s" ) , *FilePath );
// 		return nullptr;
// 	}
// 
// 	// WAV 헤더 파싱
// 	const uint8* Header = RawFileData.GetData ( );
// 	int32 SampleRate = *reinterpret_cast<const int32*>( Header + 24 ); // 24번 오프셋에서 샘플 레이트
// 	int32 Channels = *reinterpret_cast<const int16*>( Header + 22 );  // 22번 오프셋에서 채널 수
// 	int32 BitsPerSample = *reinterpret_cast<const int16*>( Header + 34 ); // 34번 오프셋에서 비트 깊이
// 	int32 DataSize = *reinterpret_cast<const int32*>( Header + 40 );  // 40번 오프셋에서 데이터 크기
// 
// 	if (DataSize <= 0 || DataSize > ( RawFileData.Num ( ) - 44 ) ||
// 		SampleRate <= 0 || Channels <= 0 || BitsPerSample <= 0)
// 	{
// 		UE_LOG ( LogTemp , Error , TEXT ( "WAV file contains invalid metadata: %s" ) , *FilePath );
// 		return nullptr;
// 	}
// 
// 	// SoundWave 생성 및 데이터 설정
// 	USoundWave* SoundWave = NewObject<USoundWave> ( );
// 
// 	SoundWave->RawData.Lock ( LOCK_READ_WRITE );
// 	void* LockedData = SoundWave->RawData.Realloc ( DataSize );
// 	FMemory::Memcpy ( LockedData , Header + 44 , DataSize ); // WAV 헤더 이후가 PCM 데이터
// 	SoundWave->RawData.Unlock ( );
// 
// 	SoundWave->Duration = static_cast<float>( DataSize ) / ( SampleRate * Channels * BitsPerSample / 8 );
// 	SoundWave->SampleRate = SampleRate;
// 	SoundWave->NumChannels = Channels;
// 
// 	UE_LOG ( LogTemp , Warning , TEXT ( "Loaded WAV file: %s" ) , *FilePath );
// 	UE_LOG ( LogTemp , Warning , TEXT ( "SampleRate: %d, Channels: %d, Duration: %.2f" ) ,
// 		   SampleRate , Channels , SoundWave->Duration );
// 
// 	return SoundWave;
// }