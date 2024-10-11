// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget_KMK.generated.h"

// 내부 프로젝트 시작시 나오는 화면들
// 로그인, 무대 입장 등의 버튼 판넬 등 관리하는 클래스임

UCLASS()
class VIRTUALIDOL_API UStartWidget_KMK : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StartSwitcher;

	UFUNCTION ()
	void GoBack();
	
	// 인터페이스
	UPROPERTY( )
	class UVirtualGameInstance_KMK* gi;

#pragma region Login Widget Panel
// ================================================================
// Login Widget Panel
// ================================================================

	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Login;

	// 텍스트 블럭
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_ID;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_PW;

	// 버튼 연동 함수
	UFUNCTION ()
	void OnMyLogin();
#pragma endregion
#pragma region  Four Button Widget Panel
// ================================================================
// Four Button Widget Panel
// ================================================================
	
	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_CreateStage;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_ReadyConcert;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_StartConcert;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_ComeInStage;

	// 버튼 연동 함수
	UFUNCTION()
	void CreateStagePanel();
	UFUNCTION()
	void SettingStagePanel();
	UFUNCTION()
	void StartConcertPanel();
	UFUNCTION()
	void ComeInStagePanel();
#pragma endregion
#pragma region Stage Setting Widget Panel
// ================================================================
// Stage Setting Widget Panel
// ================================================================
	// 추가 기능
	UPROPERTY(EditAnywhere, Category="Particle" )
	TArray<class UMaterial*> EffectParticles;
	UPROPERTY(EditAnywhere, Category = "Particle")
	TArray<class UMaterial*> FeversParticles;
	UPROPERTY(EditAnywhere, Category = "Particle")
	TArray<class UMaterial*> HighParticles;
	UPROPERTY(EditAnywhere, Category = "Particle" )
	TArray<FVector> ParticlePositions;

	// 버튼
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Back;
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_CreateTicket;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Complete;

	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Right;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Left;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_FRight;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_FLeft;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_HRight;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_HLeft;

	// 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Ticket;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Particle;
	UPROPERTY(meta = (BindWidget))
    class UImage* Image_Fever;
	UPROPERTY ( meta = ( BindWidget ) )
    class UImage* Image_HEffect;
	// 팝업
    UPROPERTY ( meta = ( BindWidget ) )
    class UCanvasPanel* CompletePopUp;
	// 텍스트 블럭
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_Day;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_STime;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_ETime;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_SingTime;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_Ticket;
	// 버튼 연동 함수
	UFUNCTION()
	void PressRight();
	UFUNCTION()
	void PressLeft ();
	UFUNCTION()
	void PressFRight();
	UFUNCTION()
	void PressFLeft ();
	UFUNCTION()
	void PressHRight();
	UFUNCTION()
	void PressHLeft ();
	UFUNCTION()
	void PressCreateTicket();
	UFUNCTION()
	void CompeleteSetting();
	
	// 추가함수
	UFUNCTION()
	void PlayParticleSystem(int32 index , TArray<class UMaterial*> ParticlesArray , class UImage* image );
	UFUNCTION ()
	void ClearAll();
	int32 particleNum = 0;
	int32 feverNum = 0;
	int32 highNum = 0;
	
	UFUNCTION ()
	void ChangeFindRoomPanel(const FString& title);
	UFUNCTION()
	void CreateStageWidget(const FString& createName);
#pragma endregion
#pragma region EntryPanel Widget Panel
// ================================================================
// EntryPanel Widget Panel
// ================================================================
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* VIPPopUpPanel;
	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Yes;
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_NormalEntry;	
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_No;	
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_VipEntry;	
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Back1;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Back2;

	// 버튼 연동 함수
    UFUNCTION ( )
	void PressYesButt( );
	UFUNCTION ( )
	void PressNoButt( );
    UFUNCTION ( )
    void PressVipEntry ( );
	UFUNCTION()
	void PressNormalEntry();

	// 그리드
	UPROPERTY ( meta = ( BindWidget ) )
	class UGridPanel* FindRoomGrid;
	// 추가함수
	UFUNCTION()
	void FindRoom();
	UFUNCTION()
	void CreateRoomWidget(const struct FRoomInfo& info);
	UPROPERTY(EditAnywhere, Category="Room" )
	TSubclassOf<class UUserWidget> roomWidgetFact;

	int32 roomCount = 0;
	UFUNCTION()
	void SetPosWidget(class URoomWidget_KMK* widget, int32 num);

	UFUNCTION ()
	void ClearChild( );
	
	class UTexture2D* t;
	UPROPERTY()
	int32 roomNum = -1;
	
#pragma endregion
#pragma region FindRoom & Select Stage
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Select;
	UFUNCTION( )
	void StageSelect( );
#pragma endregion

};
