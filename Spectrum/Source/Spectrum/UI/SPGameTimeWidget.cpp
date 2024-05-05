// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPGameTimeWidget.h"

#include "Components/TextBlock.h"

USPGameTimeWidget::USPGameTimeWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void USPGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MatchCountDownText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("MatchCountdownText"))));
}

void USPGameTimeWidget::UpdateTime(const float Time)
{
	UE_LOG(LogTemp,Log,TEXT("update!"));
	int32 Minutes = FMath::FloorToInt(Time / 60.f); //분을 계산
	int32 Seconds = Time - Minutes *60;

	FString CountdownText = FString::Printf(TEXT("%02d:%02d"),Minutes,Seconds);
	if(MatchCountDownText)
	{
		MatchCountDownText->SetText(FText::FromString(CountdownText));
	}
	
}
