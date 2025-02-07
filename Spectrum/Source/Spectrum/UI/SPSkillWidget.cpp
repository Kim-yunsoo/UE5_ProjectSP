// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPSkillWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

USPSkillWidget::USPSkillWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void USPSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SkillUI = Cast<UImage>(GetWidgetFromName(TEXT("ISkill")));
	ensure(SkillUI);

	SlowBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBSlow")));
	ensure(SlowBar);
	IceBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBIce")));
	ensure(IceBar);
	TeleBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBTeleport")));
	ensure(TeleBar);

	if (SlowBar)
	{
		SlowBar->SetPercent(0.0f);
	}

	if (IceBar)
	{
		IceBar->SetPercent(0.0f);
	}

	if (TeleBar)
	{
		TeleBar->SetPercent(0.0f);
	}
}

void USPSkillWidget::UpdateSlowBar(float NewCurrentPercent) //��������Ʈ�� �ޱ�
{

	SlowBar->SetPercent(NewCurrentPercent);
}

void USPSkillWidget::UpdateIceBar(float NewCurrentPercent) //��������Ʈ�� �ޱ�
{
	IceBar->SetPercent(NewCurrentPercent);
}

void USPSkillWidget::UpdateTeleBar(float NewCurrentPercent) //��������Ʈ�� �ޱ�
{
	TeleBar->SetPercent(NewCurrentPercent);
}
