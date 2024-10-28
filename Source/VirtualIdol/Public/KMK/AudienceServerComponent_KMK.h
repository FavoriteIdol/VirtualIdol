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
    class AHSW_ThirdPersonCharacter* player;
	UPROPERTY ( )
    class AHSW_ThirdPersonCharacter* playerMesh;

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

	UPROPERTY(ReplicatedUsing = OnRep_ChangePlayerMesh)
	int32 playerMeshNum = -2;

	UFUNCTION(Server, Reliable )
	void ServerRPC_ChangeMyMesh(int32 num );

	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_ChangeMyMesh(int32 num);
	UFUNCTION( )
	void OnRep_ChangePlayerMesh ( );

	UFUNCTION( )
	void SetVirtualVisible(class AHSW_ThirdPersonCharacter* mesh, bool bVisible );

	bool bCount = false;
	bool bVis = false;
#pragma endregion
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable )
	void ServerRPC_StartConcert( );
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPC_StartConcert( float CountdownTime );
	UFUNCTION(NetMulticast, Unreliable )
	void MultiRPC_UpdateCount( const FString& TimeText );
	FTimerHandle startCountDownHandle;
	UPROPERTY(EditAnywhere )
	float durationTime = 5;
	float remainTime = 0.f;
	UPROPERTY()
	bool bTime = false;
	FString s;
	UFUNCTION( )
	void StartCountDown( );
	UFUNCTION ( )
	FString GetTimeDifference ( const FString& SetTime );

	UPROPERTY( )
	class UVirtual_KMK* virtualCharacter;

	UPROPERTY( EditAnywhere)
	FString setConcertTime = "22:10:00";

	UFUNCTION( )
	void FindVirtualCharacter( );
	UPROPERTY ( EditDefaultsOnly , Category = Fever )
	TArray< TSubclassOf<class AActor>> appearFact;

	bool onReq = false;
};
