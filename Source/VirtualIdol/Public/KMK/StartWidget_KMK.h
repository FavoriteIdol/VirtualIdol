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
	TArray<class UParticleSystem*> EffectParticles;
	UPROPERTY(EditAnywhere, Category = "Particle")
<<<<<<< HEAD
	TArray<class UParticleSystem*> FeversParticles;
	UPROPERTY(EditAnywhere, Category = "Particle" )
	TArray<FVector> ParticlePositions;

	// 버튼
=======
	TArray<class UMaterial*> FeversParticles;
	UPROPERTY(EditAnywhere, Category = "Particle")
	TArray<class UMaterial*> HighParticles;
	UPROPERTY(EditAnywhere, Category = "Particle" )
	TArray<FVector> ParticlePositions;
	// 텍스트
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_Back;
	UPROPERTY ( meta = ( BindWidget ) )
<<<<<<< HEAD
    class UButton* Butt_CreateTicket;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Complete;
=======
	class UButton* Butt_CreateTicket;
	UPROPERTY ( meta = ( BindWidget ) )
	class UMultiLineEditableText* EditMultiText_Ticket;
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')

	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Right;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Left;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_FRight;
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_FLeft;

<<<<<<< HEAD
	// 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Ticket;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Particle;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Fever;
	// 팝업
    UPROPERTY ( meta = ( BindWidget ) )
    class UCanvasPanel* CompletePopUp;
	// 텍스트 블럭
=======
	void ClearAllText( );
#pragma endregion
#pragma region Set Effect
	// 이펙트 결정 패널
	UPROPERTY ( meta = ( BindWidget ) )
    class UCanvasPanel* SetEffectPanel;
	UPROPERTY ( meta = ( BindWidget ) )
    class UImage* Image_Particle;
	UPROPERTY ( meta = ( BindWidget ) )
    class UImage* Image_Fever;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Right;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Left;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_FRight;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_FLeft;

	int32 particleNum = 0;
	int32 feverNum = 0;

	UFUNCTION( )
	void PressRightButt( );
	UFUNCTION( )
	void PressLeftButt( );
	UFUNCTION( )
	void PressFRightButt( );
	UFUNCTION( )
	void PressFLeftButt( );
	UFUNCTION( )
	void SetPanelVisible( class UCanvasPanel* visiblePanel, class UCanvasPanel* hiddenPanel0 , class UCanvasPanel* hiddenPanel1 , class UCanvasPanel* hiddenPanel2 );

	void AddIndex( int32 num , TArray<class UMaterial*> meshArray , class UImage* image );
	void MinusIndex( int32 num , TArray<class UMaterial*>  meshArray , class UImage* image );
#pragma endregion

#pragma region Final Set Stage
	// 최종 결제 페널
>>>>>>> parent of 03c4198 (Merge branch 'KMK_Proto_241017')
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_Day;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_STime;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditText_ETime;

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
	void PressCreateTicket();
	UFUNCTION()
	void CompeleteSetting();
	
	// 추가함수
	UFUNCTION()
	void PlayParticleSystem(int32 index , TArray<class UParticleSystem*> ParticlesArray , class UImage* image );
	UFUNCTION ()
	void ClearAll();
	int32 particleNum = 0;
	int32 feverNum = 0;
#pragma endregion
#pragma region EntryPanel Widget Panel
// ================================================================
// EntryPanel Widget Panel
// ================================================================

	// 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Butt_Yes;
	UPROPERTY(meta = (BindWidget))
    class UButton* Butt_No;	
	UPROPERTY ( meta = ( BindWidget ) )
    class UButton* Butt_Back1;
	UPROPERTY ( meta = ( BindWidget ) )
	class UButton* Butt_Back2;

	// 버튼 연동 함수
    UFUNCTION ( )
    void PressYesButt ( );
	UFUNCTION()
	void PressNoButt();

	// 그리드
	UPROPERTY ( meta = ( BindWidget ) )
	class UGridPanel* FindRoomGrid;
	// 추가함수
	UFUNCTION()
	void FindRoom();
	UFUNCTION()
	void CreateRoomWidget();
	UPROPERTY(EditAnywhere, Category="Room" )
	TSubclassOf<class UUserWidget> roomWidgetFact;

	int32 roomNum = 0;
	UFUNCTION()
	void SetPosWidget(class URoomWidget_KMK* widget, int32 num);

	UFUNCTION ()
	void ClearChild( );

#pragma endregion
};
