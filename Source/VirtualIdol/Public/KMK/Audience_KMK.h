// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Audience_KMK.generated.h"

// 나눠진 판넬을 묶기 위한 구조체
USTRUCT ( BlueprintType )
struct FButtonInfo
{
	GENERATED_BODY ( )

	UPROPERTY(meta = (BindWidget))
    class UButton* button;
	UPROPERTY(meta = (BindWidget ) )
	class UImage* backImage;
	UPROPERTY(meta = (BindWidget ) )
	class UImage* image;
	UPROPERTY(meta = (BindWidget ) )
	class UTextBlock* text;
};
UCLASS()
class VIRTUALIDOL_API UAudience_KMK : public UUserWidget
{
	GENERATED_BODY()
public :
	virtual void NativeConstruct ( ) override;
	
	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;
	UPROPERTY( )
	class ATP_ThirdPersonCharacter* pc;

	void OnOffFunction(class UTextBlock* textBlocks, int32 num, bool bAllVisib = false );
	void ChangeTextAndImage ( FLinearColor color , int32 num , TArray<FString> textArray , bool bMyAuth = false );
		
	UFUNCTION( )
	void SetVirtualWBP( );
#pragma region StructButt
	TArray<FButtonInfo> ButtonsInfoArray;
	void SetUpButtonInfo( );

	TArray<FString> buttonName = {TEXT("Hidden"), TEXT("Mode"), TEXT("Mike"), TEXT("Chat"), TEXT("Emotion"), TEXT("Vip")};
	UPROPERTY(EditAnywhere )
	TArray<FString> currentText = {TEXT("숨기기"), TEXT("1인 모드"), TEXT("마이크"), TEXT("채팅"), TEXT("이모티콘"), TEXT("VIP 결제")};
	UPROPERTY(EditAnywhere )
	TArray<FString> changeText = {TEXT("보이기"), TEXT("다인 vip모드"), TEXT("음소거"), TEXT("채팅"), TEXT("이모티콘"), TEXT("VIP")};
	
	void OnOffInfo(FLinearColor color,  ESlateVisibility bVisib, int32 num, TArray<FString> textArray );

	bool bMyVip = false;
	// 1. Hidden
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Hidden;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Hidden;

	UFUNCTION( )
	void PressHiddenButt( );
	// 2. Mode
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Mode;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Mode;

	UFUNCTION( )
	void PressModeButt( );
	
	// 3. Mike
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Mike;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Mike;
	bool bMikeOn;
	UFUNCTION( )
	void StartVoiceChat( );
	UFUNCTION( )
	void StopVoiceChat( );

	UFUNCTION( )
	void PressMikeButt( );
	// 4. Chat
	bool bChatOn = false;
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Chat;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Chat;

	UFUNCTION( )
	void PressChatButt( );
	// 5. Emotion
	bool bEmotion = false;
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Emotion;
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Emotion;

	UFUNCTION( )
	void PressEmotionButt( );
	// 6. VIP
	bool bVip = false;
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Vip;		
	UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Vip;

	UFUNCTION( )
	void PressVipButt( );
	UFUNCTION( )
	void VipAuthority( );

	// 콘서트 시작 5초전
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_StartConcert;

	UFUNCTION( )
	void PressStartConcertButt( );

#pragma endregion
#pragma region PopUp
	// 캔버스
	UPROPERTY(meta = (BindWidget ) )
	class UCanvasPanel* PopUpPanel;
	UPROPERTY(meta = (BindWidget ) )
	class UGridPanel* ChatGridPanel;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Yes;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_No;

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
#pragma region Exit Room
	// 나가기 
	UPROPERTY ( meta = ( BindWidget ) )
	class UCanvasPanel* ExitPanel;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Exit;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Out;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Cancel;
	
	UFUNCTION( )
	void PressExitButt( );
	UFUNCTION( )
	void PressOutButt( );
	UFUNCTION( )
	void PressCancelButt( );

#pragma endregion
#pragma region Cash
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Object0;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Object1;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Object2;

	UPROPERTY(EditAnywhere, Category = Object )
	TArray<AActor*> objects;

	UFUNCTION( )
	void PressObjectButt( );
	UFUNCTION( )
	void PressObject1Butt( );
	UFUNCTION( )
	void PressObject2Butt( );

#pragma endregion

};
