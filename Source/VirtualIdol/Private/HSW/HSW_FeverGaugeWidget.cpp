// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_FeverGaugeWidget.h"
#include "Components/ProgressBar.h"


void UHSW_FeverGaugeWidget::NativeConstruct ( )
{
	//SetFeverGauge();
}

void UHSW_FeverGaugeWidget::SetFeverGauge ( float value)
{
	FeverGauge->SetPercent( value / 1);
}
