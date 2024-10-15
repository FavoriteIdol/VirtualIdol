// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_MainWidget.h"
#include "Components/Button.h"
#include "HSW/HSW_ThirdPersonCharacter.h"
#include "HSW/HSW_ImojiConponent.h"

void UHSW_MainWidget::NativeConstruct ( )
{
	Btn_Imoji_1->OnClicked.AddDynamic ( this, &UHSW_MainWidget::OnMyImoji01 );
	Btn_Imoji_2->OnClicked.AddDynamic ( this , &UHSW_MainWidget::OnMyImoji02 );
	Btn_Imoji_3->OnClicked.AddDynamic ( this , &UHSW_MainWidget::OnMyImoji03 );
	Btn_Imoji_4->OnClicked.AddDynamic ( this , &UHSW_MainWidget::OnMyImoji04 );

	Player = Cast<AHSW_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn() );
	ImojiComponent = Player->GetComponentByClass<UHSW_ImojiConponent>();
}

void UHSW_MainWidget::OnMyImoji01 ( )
{
	Player->Imoji (0);
}

void UHSW_MainWidget::OnMyImoji02 ( )
{
	Player->Imoji ( 1 );
}

void UHSW_MainWidget::OnMyImoji03 ( )
{
	Player->Imoji ( 2 );
}

void UHSW_MainWidget::OnMyImoji04 ( )
{
	Player->Imoji ( 3 );
}
