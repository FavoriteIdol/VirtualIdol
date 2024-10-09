// Fill out your copyright notice in the Description page of Project Settings.


#include "HSW/HSW_FeverGaugeWidget.h"
#include "Components/ProgressBar.h"


void UHSW_FeverGaugeWidget::NativeConstruct ( )
{
	CurrentGauge = 0;
	SetFeverGauge(0);
}

void UHSW_FeverGaugeWidget::SetFeverGauge ( float value)
{
	CurrentGauge += value;
	FeverGauge->SetPercent( CurrentGauge / 1);
}
