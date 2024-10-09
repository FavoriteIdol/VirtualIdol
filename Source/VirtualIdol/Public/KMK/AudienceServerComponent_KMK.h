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
	// 게임 인스턴스
	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;
	
	// 서버에게 채팅 요청
	UFUNCTION(Server, Reliable )
	void ServerRPCChat(const FString& chat);
	// 채팅 업데이트
	UFUNCTION(NetMulticast, Reliable )
	void MultiRPCChat ( const FString& chat );
		
};
