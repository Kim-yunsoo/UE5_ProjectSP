// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPHUDWidget.h"

#include "SPSkillWidget.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "UI/SPTargetUI.h"
#include "UI/SPMainMenu.h"
#include "UI/Interaction/SPInteractionWidget.h"

class ISPCharacterHUDInterface;

USPHUDWidget::USPHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
void USPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SlowSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WBSkill"));
	IceSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WBSkill"));
	TeleSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WBSkill"));

	TargetUI = Cast<USPTargetUI>(GetWidgetFromName(TEXT("WBTargetUI")));
	ensure(TargetUI);

	//MainMenuWidget = Cast<USPMainMenu>(GetWidgetFromName(TEXT("WBTargetUI")));
	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<USPMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed); 
	}
	
	if(InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<USPInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed); 
	}

	ISPCharacterHUDInterface* CharacterWidget = Cast<ISPCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}
}

void USPHUDWidget::DisplayMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void USPHUDWidget::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USPHUDWidget::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
	}
}

void USPHUDWidget::UpdateSlowCDTime(float NewCurrentTime)
{
	SlowSkillWidget->UpdateSlowBar(NewCurrentTime);
}

void USPHUDWidget::UpdateIceCDTime(float NewCurrentTime)
{
	IceSkillWidget->UpdateIceBar(NewCurrentTime);
}

void USPHUDWidget::UpdateTeleCDTime(float NewCurrentTime)
{
	TeleSkillWidget->UpdateTeleBar(NewCurrentTime);
}

void USPHUDWidget::ShowInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USPHUDWidget::HideInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USPHUDWidget::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
	


