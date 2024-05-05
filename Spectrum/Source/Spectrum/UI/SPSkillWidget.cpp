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
	IceBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBIce")));
	TeleBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBTeleport")));
	
	ensure(SlowBar);
	ensure(IceBar);
	ensure(TeleBar);

	if(SlowBar)
	{
		SlowBar->SetPercent(0.0f);
	}

	if(IceBar)
	{
		IceBar->SetPercent(0.0f);
	}
	
	if(TeleBar)
	{
		TeleBar->SetPercent(0.0f);
	}
}

void USPSkillWidget::UpdateSlowBar(float NewCurrentPercent) //델리게이트로 받기
{
	SlowBar->SetPercent(0.f);
	
}
void USPSkillWidget::UpdateIceBar(float NewCurrentPercent) //델리게이트로 받기
{
	IceBar->SetPercent(NewCurrentPercent);
}

void USPSkillWidget::UpdateTeleBar(float NewCurrentPercent) //델리게이트로 받기
{
	TeleBar->SetPercent(NewCurrentPercent);
}