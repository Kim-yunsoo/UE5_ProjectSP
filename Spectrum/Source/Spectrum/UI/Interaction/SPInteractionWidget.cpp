// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/SPInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/SPInteractionInterface.h"

void USPInteractionWidget::UpdateWidget(const FInteractableData* InteractableData)
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
				InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EInteractableType::NonPlayerCharacter:
		break;
	case EInteractableType::Device:
		break;
	case EInteractableType::Toggle:
		break;
	case EInteractableType::Container:
		break;
	default:;
	}
	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
	
}

float USPInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

void USPInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void USPInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}
