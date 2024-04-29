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
	ensure(SlowBar);
	ensure(IceBar);

	if(SlowBar)
	{
		SlowBar->SetPercent(0.0f);
	}

	if(IceBar)
	{
		IceBar->SetPercent(0.0f);
	}
}

void USPSkillWidget::UpdateSlowBar(float NewCurrentPercent) //델리게이트로 받기
{
	SlowBar->SetPercent(NewCurrentPercent);
}
void USPSkillWidget::UpdateIceBar(float NewCurrentPercent) //델리게이트로 받기
{
	IceBar->SetPercent(NewCurrentPercent);
}