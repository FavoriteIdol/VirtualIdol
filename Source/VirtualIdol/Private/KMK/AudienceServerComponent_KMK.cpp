// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/AudienceServerComponent_KMK.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "KMK/Audience_KMK.h"
#include "Net/UnrealNetwork.h"
#include "KMK/VirtualGameInstance_KMK.h"

// Sets default values for this component's properties
UAudienceServerComponent_KMK::UAudienceServerComponent_KMK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UAudienceServerComponent_KMK::BeginPlay()
{
	Super::BeginPlay();
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK>(GetWorld()->GetGameInstance());

	player = Cast<ATP_ThirdPersonCharacter> (GetWorld()->GetFirstPlayerController()->GetPawn());
	playerMesh = Cast<ATP_ThirdPersonCharacter> (GetOwner());
	if (gi)
	{
		// 플레이어가 로컬 플레이어 일때
		if (playerMesh->IsLocallyControlled())
        {
			// 클라이언트에서 서버로 RPC 호출
			ServerRPC_ChangeMyMesh(gi->playerMeshNum);
			
        }
		else
		{
			// 로컬이 아닌 경우에 플레이어의 playerMeshNum에 따라 
			if (playerMeshNum < 0)
			{
				SetVirtualVisible ( playerMesh , false );
			}
			else if (playerMeshNum > 1)
			{
				SetVirtualVisible ( playerMesh , true );
			}
			else
			{
				playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
			}
			
		}
	}
}


// Called every frame
void UAudienceServerComponent_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bTime)
	{
		remainTime = GetWorld ( )->GetTimerManager ( ).GetTimerRemaining ( startCountDownHandle );
		if (player && player->audienceWidget)
		{
			if (remainTime >= 0.0f)
			{
				// 남은 시간을 위젯에 표시
				player->audienceWidget->CountDownText ( remainTime );
			}
		}
	}
}

void UAudienceServerComponent_KMK::ServerRPCChat_Implementation ( const FString& chat )
{
	MultiRPCChat(chat);
}

void UAudienceServerComponent_KMK::MultiRPCChat_Implementation ( const FString& chat )
{
	auto* p = Cast<ATP_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	if (p->audienceWidget)
	{
		p->audienceWidget->CreateChatWidget(chat );
	}

}


void UAudienceServerComponent_KMK::ServerRPC_ChangeMyMesh_Implementation ( int32 num)
{
	playerMeshNum = num;
	MultiRPC_ChangeMyMesh ( num ); // 클라이언트에게 RPC 호출
}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num )
{
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
	if (gi)
	{
		if (num < 0)
		{
			SetVirtualVisible ( playerMesh , false );
		}
		else if(num > 1)
		{
			SetVirtualVisible ( playerMesh , true );
		}
		else
		{
			playerMesh->GetMesh ( )->SetSkeletalMesh ( audienceMesh[num] );
		}
	}

}

void UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh ( )
{
	// Get the PlayerController owning this component
	APlayerController* playerController = Cast<APlayerController> ( GetOwner ( ) );
	UE_LOG ( LogTemp , Warning , TEXT ( "Player %s MeshNum: %d" ) , *GetOwner ( )->GetName ( ) , playerMeshNum );

	// Make sure the PlayerController is valid
	if (playerController)
	{
		// Get the character (Pawn) that the PlayerController is controlling
		ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( playerController->GetPawn ( ) );

		if (playerCharacter)
		{
			// Change the skeletal mesh of the character
			if (playerMeshNum < 0)
			{
				SetVirtualVisible(playerCharacter, false );
			}
			else if (playerMeshNum > 1)
			{
				SetVirtualVisible ( playerCharacter , true );
			}
			else
			{
				playerCharacter->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
			}
		}
	}
}

void UAudienceServerComponent_KMK::SetVirtualVisible ( class ATP_ThirdPersonCharacter* mesh , bool bVisible )
{
	mesh->GetMesh ( )->SetRenderInMainPass ( bVisible );
	mesh->GetMesh ( )->SetRenderInDepthPass ( bVisible );
}

void UAudienceServerComponent_KMK::StartCountDown ( )
{
	bTime = true;
	GetWorld ( )->GetTimerManager ( ).SetTimer ( startCountDownHandle , FTimerDelegate::CreateLambda ( [this]( )
		{
			ServerRPC_ChangeMyMesh ( 2 );
		} ) , 6 , false );

}
void UAudienceServerComponent_KMK::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 DOREPLIFETIME(UAudienceServerComponent_KMK, playerMeshNum);
}

void UAudienceServerComponent_KMK::ServerRPC_StartConcert_Implementation ( )
{
	MultiRPC_StartConcert( durationTime );
}

void UAudienceServerComponent_KMK::MultiRPC_StartConcert_Implementation ( float CountdownTime )
{
	ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( GetWorld ( )->GetFirstPlayerController ( )->GetPawn ( ) );
	bTime = true;
	remainTime = CountdownTime;
	// 카운트다운 UI 패널 표시
	if (playerCharacter && !playerCharacter->HasAuthority() && playerCharacter->audienceWidget)
	{
		playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Visible );
	}

	// 타이머 시작 (각 클라이언트에서 동작)
	GetWorld ( )->GetTimerManager ( ).SetTimer (
		startCountDownHandle ,
		FTimerDelegate::CreateLambda ( [this, playerCharacter]( )
			{
				bTime = false;  // 카운트다운 종료
				if (playerCharacter && !playerCharacter->HasAuthority() && playerCharacter->audienceWidget)
				{
					playerCharacter->audienceWidget->CountDownPanelVisible ( ESlateVisibility::Hidden );
				}
				ServerRPC_ChangeMyMesh ( 2 );
			} ) ,
		CountdownTime , false  // CountdownTime 후 한 번 실행
	);

}

