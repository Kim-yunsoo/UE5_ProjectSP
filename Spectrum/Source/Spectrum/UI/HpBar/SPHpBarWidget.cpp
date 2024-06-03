// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBar/SPHpBarWidget.h"

#include "Components/ProgressBar.h"

USPHpBarWidget::USPHpBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	MaxHp=-1.0f;
}

void USPHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USPHpBarWidget::UpdateHpBar(float NewCurrentHp) 
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
