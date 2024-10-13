// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudienceServerComponent_KMK.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALIDOL_API UAudienceServerComponent_KMK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudienceServerComponent_KMK();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 플레이어 컨트롤러 들고오기
	UPROPERTY( )
    class ATP_ThirdPersonCharacter* player;
	UPROPERTY ( )
    class ATP_ThirdPersonCharacter* playerMesh;

#pragma region Chatting
	// 서버에게 채팅 요청
	UFUNCTION(Server, Reliable )
	void ServerRPCChat(const FString& chat);
	// 채팅 업데이트
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPCChat ( const FString& chat );
#pragma endregion

#pragma region ChangeMyMesh
	// 플레이어 메쉬 변경
	UPROPERTY(EditAnywhere, Category = "Audience" )
	TArray<class USkeletalMesh*> audienceMesh;

	UPROPERTY( ReplicatedUsing = OnRep_ChangePlayerMesh )
	int32 playerMeshNum = 0;

	UFUNCTION(Server, Reliable )
	void ServerRPC_ChangeMyMesh(int32 num );

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_ChangeMyMesh(int32 num );
	UFUNCTION( )
	void OnRep_ChangePlayerMesh ( );

	UFUNCTION( )
	void SetVirtualVisible(class ATP_ThirdPersonCharacter* mesh, bool bVisible );

#pragma endregion
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable )
	void ServerRPC_StartConcert( );
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_StartConcert( float CountdownTime );
	
	FTimerHandle startCountDownHandle;
	UPROPERTY(EditAnywhere )
	float durationTime = 6;
	float remainTime = 0.f;
	UPROPERTY()
	bool bTime = false;

	UFUNCTION( )
	void StartCountDown( );
};
