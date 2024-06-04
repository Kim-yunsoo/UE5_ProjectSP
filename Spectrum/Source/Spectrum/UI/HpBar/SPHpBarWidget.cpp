// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBar/SPHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "Interface/SPNonCharacterWidgetInterface.h"

USPHpBarWidget::USPHpBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	MaxHp=-1.0f;
}

void USPHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	ISPNonCharacterWidgetInterface* CharacterWidget = Cast<ISPNonCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
	
}

void USPHpBarWidget::UpdateHpBar(float NewCurrentHp) 
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
