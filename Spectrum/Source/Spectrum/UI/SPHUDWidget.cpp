// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPHUDWidget.h"

#include "SpectrumLog.h"
#include "SPGameTimeWidget.h"
#include "SPManualWidget.h"
#include "SPScoreWidget.h"
#include "SPSkillWidget.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "Potion/Make/SPMakingPotionWidget.h"
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

	SlowSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WB_Skill"));
	IceSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WB_Skill"));
	TeleSkillWidget = Cast<USPSkillWidget>(GetWidgetFromName("WB_Skill"));
	ScoreWidget = Cast<USPScoreWidget>(GetWidgetFromName("WBSPScoreWidget"));
	TargetUI = Cast<USPTargetUI>(GetWidgetFromName(TEXT("WBTargetUI")));
	MakingPotionWidget = Cast<USPMakingPotionWidget>(GetWidgetFromName(TEXT("WBPSPMakingPotionWidget")));
	ManualWidget = Cast<USPManualWidget>(GetWidgetFromName(TEXT("WBPManual")));
	
	if(!MakingPotionWidget)
	{
	}
	//ensure(TargetUI);
	GameTimeWidget= Cast<USPGameTimeWidget>(GetWidgetFromName(TEXT("WB_GameTimeWidget")));
	
	//ensure(TargetUI);

	MainMenuWidget = Cast<USPMainMenu>(GetWidgetFromName(TEXT("WBTargetUI")));
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
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void USPHUDWidget::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void USPHUDWidget::UpdateTime(float CountdownTime)
{
	GameTimeWidget->UpdateTime(CountdownTime);
}

void USPHUDWidget::UpdateSlowCDTime(float NewCurrentTime)
{
	SlowSkillWidget->UpdateSlowBar(NewCurrentTime);
}

void USPHUDWidget::UpdateMakingPotionWidget(bool bIsVisible)
{
	if(bIsVisible)
	{
		if(MakingPotionWidget)
		{
			MakingPotionWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if(MakingPotionWidget)
		{
			MakingPotionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USPHUDWidget::UpdateManualWidget(bool bIsVisible)
{
	if(bIsVisible)
	{
		if(ManualWidget)
		{
			ManualWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if(ManualWidget)
		{
			ManualWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USPHUDWidget::UpdateIceCDTime(float NewCurrentTime)
{
	IceSkillWidget->UpdateIceBar(NewCurrentTime);
}

void USPHUDWidget::UpdateTeleCDTime(float NewCurrentTime)
{
	TeleSkillWidget->UpdateTeleBar(NewCurrentTime);
}

void USPHUDWidget::UpdateScore(const ColorType& Mycolor, const int32 Score)
{
	ScoreWidget->UpdateScore(Mycolor,Score);
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

void USPHUDWidget::ClearMakingWieget()
{
	MakingPotionWidget->ClearWidget();
}
	

void USPHUDWidget::MakingPotionWieget(USPItemBase* Item)
{
	MakingPotionWidget->MakingPotion(Item);
}
