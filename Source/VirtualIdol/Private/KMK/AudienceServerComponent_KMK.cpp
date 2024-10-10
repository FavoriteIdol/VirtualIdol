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
		if (player->IsLocallyControlled())
        {
			// 클라이언트에서 서버로 RPC 호출
			ServerRPC_ChangeMyMesh(gi->playerMeshNum);
        }
		else
		{
			player->GetMesh()->SetSkeletalMesh(audienceMesh[playerMeshNum]);
            UE_LOG(LogTemp, Warning, TEXT("Mesh set on server for player: %d"), gi->playerMeshNum);
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
	if (player->widget)
	{
		player->widget->CreateChatWidget(chat );
	}
}


void UAudienceServerComponent_KMK::ServerRPC_ChangeMyMesh_Implementation ( int32 num)
{
	playerMeshNum = num;
    
    if (player)
    {
        USkeletalMeshComponent* playerMeshComp = player->GetMesh();
        if (playerMeshComp)
        {
            MultiRPC_ChangeMyMesh(playerMeshNum);  // 모든 클라이언트에 메쉬 변경 전달
        }
    }
}

void UAudienceServerComponent_KMK::MultiRPC_ChangeMyMesh_Implementation ( int32 num )
{
    if (player)
    {
        USkeletalMeshComponent* playerMeshComp = player->GetMesh();
        if (playerMeshComp)
        {
            playerMeshComp->SetSkeletalMesh(audienceMesh[num]);
            UE_LOG(LogTemp, Warning, TEXT("Mesh changed for player: %s to mesh: %d"), *player->GetName(), num);
        }
    }
}
void UAudienceServerComponent_KMK::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	 DOREPLIFETIME(UAudienceServerComponent_KMK, playerMeshNum);
}