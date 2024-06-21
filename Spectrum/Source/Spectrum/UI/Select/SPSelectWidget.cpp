// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Select/SPSelectWidget.h"

#include "SpectrumLog.h"
#include "SPGlobalEnum.h"
#include "SPPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Player/SPLobbyPlayerController.h"

USPSelectWidget::USPSelectWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USPSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GreenBut = Cast<UButton>(GetWidgetFromName(TEXT("GreenButton")));
	OrangeBut = Cast<UButton>(GetWidgetFromName(TEXT("OrangeButton")));
	PurpleBut = Cast<UButton>(GetWidgetFromName(TEXT("PurpleButton")));
	ReadyBut = Cast<UButton>(GetWidgetFromName(TEXT("ReadyButton")));
	ManBut = Cast<UButton>(GetWidgetFromName(TEXT("ManButton")));
	WomanBut = Cast<UButton>(GetWidgetFromName(TEXT("WomanButton")));
	NickNameText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NickNameText")));
	NickNameText->OnTextCommitted.AddDynamic(this, &USPSelectWidget::OnNickNameCommitted);
	GreenBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickGreenBut);
	OrangeBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickOrangeBut);
	PurpleBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickPurpleBut);
	ReadyBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickReadyBut);
	ManBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickManBut);
	WomanBut->OnClicked.AddDynamic(this, &USPSelectWidget::OnClickWomanBut);

	bIsSelectColor = false;
	bIsSelectGender = false;
	bIsReady = false;
	bIsSendNickName = false;

	if(PlayerSelecText)
	{
		PlayerSelecText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USPSelectWidget::OnNickNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (!Text.IsEmpty() && bIsSendNickName == false) //빈 문자열이 아닌 경우 전송한다. 
		{
			ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
			if (MyPlayerState)
			{
				MyPlayerState->SetPlayerNickName(Text.ToString());
				bIsSendNickName = true;
			}
		}
	}
}

void USPSelectWidget::OnClickGreenBut()
{
	if (bIsSelectColor == false)
	{
		ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
		if (MyPlayerState)
		{
			MyPlayerState->SetPlayerColor(ColorType::Green);
			bIsSelectColor = true;
			GreenBut->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USPSelectWidget::OnClickOrangeBut()
{
	if (bIsSelectColor == false)
	{
		ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
		if (MyPlayerState)
		{
			MyPlayerState->SetPlayerColor(ColorType::Orange);
			bIsSelectColor = true;
			OrangeBut->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USPSelectWidget::OnClickPurpleBut()
{
	if (bIsSelectColor == false)
	{
		ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
		if (MyPlayerState)
		{
			MyPlayerState->SetPlayerColor(ColorType::Purple);
			bIsSelectColor = true;
			PurpleBut->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USPSelectWidget::OnClickManBut()
{
	if (bIsSelectGender == false)
	{
		ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
		if (MyPlayerState)
		{
			MyPlayerState->SetPlayerGender(GenderType::Man);
			bIsSelectGender = true;
			ManBut->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USPSelectWidget::OnClickWomanBut()
{
	if (bIsSelectGender == false)
	{
		ASPPlayerState* MyPlayerState = Cast<ASPPlayerState>(GetOwningPlayerState());
		if (MyPlayerState)
		{
			MyPlayerState->SetPlayerGender(GenderType::Woman);
			bIsSelectGender = true;
			WomanBut->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USPSelectWidget::OnClickReadyBut()
{
	if (bIsReady == false)
	{
		if (bIsSendNickName && bIsSelectColor && bIsSelectGender)
		{
			ASPLobbyPlayerController* PC = Cast<ASPLobbyPlayerController>(GetOwningPlayer());
			if(PC)
			{
				PlayerSelecText->SetVisibility(ESlateVisibility::Visible);
				ReadyBut->SetVisibility(ESlateVisibility::Collapsed);
				PC->ServerRPCReady();
			}
			bIsReady=true;
		}
	}
}

// void USPSelectWidget::ServerRPC_Implementation()
// {
// 	ASPGameState* GameState=Cast<ASPGameState>(GetOwningPlayer()->GetWorld()->GetGameState());
// 	if(GameState)
// 	{
// 		UE_LOG(LogTemp,Log,TEXT("2"));
//
// 		GameState->Ready();
// 	}
// }
