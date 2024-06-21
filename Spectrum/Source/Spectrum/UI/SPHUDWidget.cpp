// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPHUDWidget.h"
#include "SPGameTimeWidget.h"
#include "SPKeyWidget.h"
#include "SPLoadingWidget.h"
#include "SPManualWidget.h"
#include "SPScoreWidget.h"
#include "SPSkillWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/SPCharacterPlayer.h"
#include "Chat/SPChatWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Interface/SPCharacterHUDInterface.h"
#include "Potion/Make/SPMakingPotionWidget.h"
#include "UI/SPTargetUI.h"
#include "UI/Interaction/SPInteractionWidget.h"

class UCharacterMovementComponent;
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
	ScoreWidget = Cast<USPScoreWidget>(GetWidgetFromName("WB_SPScoreWidget"));
	TargetUI = Cast<USPTargetUI>(GetWidgetFromName(TEXT("WB_TargetUI")));
	MakingPotionWidget = Cast<USPMakingPotionWidget>(GetWidgetFromName(TEXT("WBPSPMakingPotionWidget")));
	ManualWidget = Cast<USPManualWidget>(GetWidgetFromName(TEXT("WBPManual")));
	//ensure(TargetUI);
	GameTimeWidget = Cast<USPGameTimeWidget>(GetWidgetFromName(TEXT("WB_GameTimeWidget")));
	ChatWidget = Cast<USPChatWidget>(GetWidgetFromName(TEXT("WBP_Chat")));
	
	LoadingWidget = Cast<USPLoadingWidget>(GetWidgetFromName(TEXT("WBP_LoadingUI")));

	if (ChatWidget)
	{
		ChatWidget->ActiveChat(false);
	}
	KeyWidget = Cast<USPKeyWidget>(GetWidgetFromName("WBPKey"));

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<USPInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	ISPCharacterHUDInterface* CharacterWidget = Cast<ISPCharacterHUDInterface>(GetOwningPlayerPawn());
	if (CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}
	if (SpectrumText)
	{
		SpectrumText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(LoadingWidget)
	{
		//LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(AIText)
	{
		AIText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(AlarmImage)
	{
		AlarmImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USPHUDWidget::ToggleMouse(bool bIsShowMouse)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	if (bIsShowMouse)
	{
		const FInputModeGameAndUI InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(true);
		PlayerCharacter->ServerRPCWidgetMove(false);
	}
	else
	{
		const FInputModeGameOnly InputMode;
		GetOwningPlayer()->SetInputMode(InputMode);
		GetOwningPlayer()->SetShowMouseCursor(false);
		PlayerCharacter->ServerRPCWidgetMove(true);
	}
}

void USPHUDWidget::UpdateTarget(bool bIsShowUI)
{
	TargetUI->UpdateTargetUI(bIsShowUI);
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
	if (bIsVisible)
	{
		if (MakingPotionWidget)
		{
			MakingPotionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		ToggleMouse(true);
	}
	else
	{
		if (MakingPotionWidget)
		{
			MakingPotionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		ToggleMouse(false);
	}
}

void USPHUDWidget::UpdateManualWidget(bool bIsVisible)
{
	if (bIsVisible)
	{
		if (ManualWidget)
		{
			ManualWidget->SetVisibility(ESlateVisibility::Visible);
		}
		ToggleMouse(true);
	}
	else
	{
		if (ManualWidget)
		{
			ManualWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		ToggleMouse(false);
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
	ScoreWidget->UpdateScore(Mycolor, Score);
}

void USPHUDWidget::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USPHUDWidget::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USPHUDWidget::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void USPHUDWidget::ToggleKeyWidget(bool bIsVisible)
{
	if (bIsVisible)
	{
		if (KeyWidget)
		{
			KeyWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (KeyWidget)
		{
			KeyWidget->SetVisibility(ESlateVisibility::Hidden);
		}
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

void USPHUDWidget::UpdateChatting(const FString& Sender, const FString& Message)
{
	ChatWidget->AddMessageOntoMessagesList(Sender, Message);
}

void USPHUDWidget::UpdatePotionUI(const int32 Index)
{
	ManualWidget->UpdatePotionUI(Index);
	
	UpdateShowUIText();
}

void USPHUDWidget::ShowChat()
{
	ChatWidget->SetFousOnChat();
	ChatWidget->ActiveChat(true);
}

void USPHUDWidget::UpdateShowUIText()
{
	AlarmImage->SetVisibility(ESlateVisibility::Visible);
	SpectrumText->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		AlarmImage->SetVisibility(ESlateVisibility::Collapsed);
		SpectrumText->SetVisibility(ESlateVisibility::Collapsed);
	}), 10.0f, false);
}

void USPHUDWidget::UpdateAIInfoText()
{
	AlarmImage->SetVisibility(ESlateVisibility::Visible);
	AIText->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		AlarmImage->SetVisibility(ESlateVisibility::Collapsed);
		AIText->SetVisibility(ESlateVisibility::Collapsed);
	}), 10.0f, false);
}

void USPHUDWidget::HideLoadingWidget()
{
	if (LoadingWidget)
	{
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
