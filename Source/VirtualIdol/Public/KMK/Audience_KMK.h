// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Audience_KMK.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALIDOL_API UAudience_KMK : public UUserWidget
{
	GENERATED_BODY()
public :
	virtual void NativeConstruct ( ) override;
	
	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;
#pragma region PopUp
	// 캔버스
	UPROPERTY(meta = (BindWidget ) )
	class UCanvasPanel* PopUpPanel;
	// 버튼
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Vip;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Yes;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_No;

	UFUNCTION( )
	void PressVipButt( );
	UFUNCTION( )
	void PressYesButt ( );
	UFUNCTION( )
	void PressNoButt( );

	UFUNCTION( )
	void VisiblePanel( ESlateVisibility visible );

#pragma endregion

#pragma region Chatting
	// 캔버스
	UPROPERTY(meta = (BindWidget ) )
	class UCanvasPanel* ChatPanel;
	// 버튼
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Send;
	// 텍스트
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_Chat;
	// 스크롤 박스
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SB_ChatLog;

	// 버튼 연결 함수
	UFUNCTION( )
	void PressSendButt( );

	// 기타 함수
	UFUNCTION( )
	void CreateChatWidget(const FString& chatText );

	UPROPERTY(EditDefaultsOnly )
	TSubclassOf<class USendChat_KMK> chatWidgetFact;

#pragma endregion

	
};
