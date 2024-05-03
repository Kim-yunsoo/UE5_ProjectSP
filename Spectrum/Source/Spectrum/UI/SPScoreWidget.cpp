// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPScoreWidget.h"
#include "SPGlobalEnum.h"
#include "Components/TextBlock.h"

USPScoreWidget::USPScoreWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}



void USPScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTextBlock* GreenScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("T_GreenScore"))));
	if(GreenScoreTextBlock)
	{
		Score.Add(ColorType::Green,GreenScoreTextBlock);
	}

	UTextBlock* OrangeScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("T_OrangeScore"))));
	if(OrangeScoreTextBlock)
	{
		Score.Add(ColorType::Orange,OrangeScoreTextBlock);
	}
	
	UTextBlock* PurpleScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("T_PurpleScore"))));
	if(PurpleScoreTextBlock)
	{
		Score.Add(ColorType::Purple,PurpleScoreTextBlock);
	}
}

void USPScoreWidget::UpdateScore(const ColorType& MyColor, const int32 MyScore)
{
	UTextBlock** BaseTextBlockPtr = Score.Find(MyColor);
	if(BaseTextBlockPtr)
	{
		FString ScoreText = FString::FromInt(MyScore);
		(*BaseTextBlockPtr)->SetText(FText::FromString(ScoreText));
	}
}