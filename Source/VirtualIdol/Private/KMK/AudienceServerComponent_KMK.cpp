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

	player = Cast<ATP_ThirdPersonCharacter> (GetOwner());
	if (gi)
	{
		if (player->HasAuthority ( ))
		{
			// 서버에서 로직 실행
			UE_LOG ( LogTemp , Warning , TEXT ( "Server: playerMeshNum on BeginPlay is %d" ) , playerMeshNum );
		}
		else
		{
			// 클라이언트에서 로직 실행
			UE_LOG ( LogTemp , Warning , TEXT ( "Client: playerMeshNum on BeginPlay is %d" ) , playerMeshNum );
		}
		// 플레이어가 로컬 플레이어 일때
		if (player->IsLocallyControlled())
        {
			// 클라이언트에서 서버로 RPC 호출
			ServerRPC_ChangeMyMesh(gi->playerMeshNum);
        }
		else
		{
			// 로컬이 아닌 경우에 플레이어의 playerMeshNum에 따라 
			player->GetMesh()->SetSkeletalMesh(audienceMesh[playerMeshNum]);
		}
	}
}


// Called every frame
void UAudienceServerComponent_KMK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	if (playerMeshNum != num)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Server: Changing playerMeshNum from %d to %d" ) , playerMeshNum , num );
		playerMeshNum = num;
		MultiRPC_ChangeMyMesh ( num ); // 클라이언트에게 RPC 호출
	}
    //if (player)
    //{
    //    USkeletalMeshComponent* playerMeshComp = player->GetMesh();
    //    if (playerMeshComp)
    //    {
    //        
    //    }
    //}
}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num )
{
	UVirtualGameInstance_KMK* gi = Cast<UVirtualGameInstance_KMK> ( GetWorld ( )->GetGameInstance ( ) );
	if (gi)
	{
		player->GetMesh ( )->SetSkeletalMesh ( audienceMesh[num] );
	}

}

void UAudienceServerComponent_KMK::OnRep_ChangePlayerMesh ( )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "OnRep_PlayerMeshNum called! playerMeshNum: %d" ) , playerMeshNum );
	// Get the PlayerController owning this component
	APlayerController* playerController = Cast<APlayerController> ( GetOwner ( ) );

	// Make sure the PlayerController is valid
	if (playerController)
	{
		// Get the character (Pawn) that the PlayerController is controlling
		ATP_ThirdPersonCharacter* playerCharacter = Cast<ATP_ThirdPersonCharacter> ( playerController->GetPawn ( ) );

		if (playerCharacter)
		{
			// Change the skeletal mesh of the character
			playerCharacter->GetMesh ( )->SetSkeletalMesh ( audienceMesh[playerMeshNum] );
		}
	}
}

void UAudienceServerComponent_KMK::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 DOREPLIFETIME(UAudienceServerComponent_KMK, playerMeshNum);
}
