// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomWidget_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API URoomWidget_KMK : public UUserWidget
{
	GENERATED_BODY()

public : 
	virtual void NativeConstruct ( ) override;
	// 인터페이스
	class UVirtualGameInstance_KMK* gi;
	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_JoinSession;	
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_SetStage;

	// 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Stage;
	
	// 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;

	// 버튼 바인딩 함수
	UFUNCTION ()
	void PressJoinSessionButt(  );
	UFUNCTION()
	void PressSetStageButt(  );

	// 기타함수
	UFUNCTION( )
	void SetImageAndText(const struct FRoomInfo& info);
	UFUNCTION( )
	void SetStageText(const FString& createName);

	UFUNCTION( )
	void ActiveButton(bool bButton );

	int32 roomNum = 0;
};
