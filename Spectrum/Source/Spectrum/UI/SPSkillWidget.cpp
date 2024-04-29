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

	if(SlowBar)
	{
		SlowBar->SetPercent(0.0f);
	}
	
}

void USPSkillWidget::UpdateSlowBar(float NewCurrentPercent) //델리게이트로 받기
{
	SlowBar->SetPercent(NewCurrentPercent);
}
