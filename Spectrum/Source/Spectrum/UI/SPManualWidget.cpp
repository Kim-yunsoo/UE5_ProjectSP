// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPManualWidget.h"

#include "SPHUDWidget.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/Image.h"


void USPManualWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SpectrumLocation = Cast<UButton>(GetWidgetFromName(TEXT("SpectrumLocation")));
	Manual = Cast<UButton>(GetWidgetFromName(TEXT("Manual")));
	Back = Cast<UButton>(GetWidgetFromName(TEXT("Back")));
		
	SpectrumLocation->OnClicked.AddDynamic(this, &USPManualWidget::SpectrumLocationCallBack);
	Manual->OnClicked.AddDynamic(this, &USPManualWidget::ManualCallBack);
	Back->OnClicked.AddDynamic(this, &USPManualWidget::BackCallBack);
	MainImage->SetVisibility(ESlateVisibility::Visible);

}

void USPManualWidget::SpectrumLocationCallBack()
{
	SpectrumLocationImage->SetVisibility(ESlateVisibility::Visible);
	MainImage->SetVisibility(ESlateVisibility::Hidden);
	ManualImage->SetVisibility(ESlateVisibility::Hidden);


}

void USPManualWidget::ManualCallBack()
{
	SpectrumLocationImage->SetVisibility(ESlateVisibility::Hidden);
	MainImage->SetVisibility(ESlateVisibility::Hidden);
	ManualImage->SetVisibility(ESlateVisibility::Visible);
}

void USPManualWidget::BackCallBack()
{
	if(MainImage->IsVisible())
	{
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
		Player->HUDWidget->UpdateManualWidget(false);
		
	}
	else
	{
		MainImage->SetVisibility(ESlateVisibility::Visible);
	}
	
}
