// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"

#include "SpectrumLog.h"
#include "UI/SPHUDWidget.h"
#include "SPGameModeBase.h"
#include "UI/ReturnMain/SPReturnToMainMenu.h"
ASPPlayerController::ASPPlayerController()
{
	static ConstructorHelpers::FClassFinder<USPHUDWidget> SPHUDWidgetRef(
		TEXT("/Game/Spectrum/UMG/WBP_SPHUD.WBP_SPHUD_C"));
	if (SPHUDWidgetRef.Class)
	{
		SPHUDWidgetClass = SPHUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<USPReturnToMainMenu> EndWidgetClassRef(
		TEXT("/Game/Spectrum/Yunsoo/UI/WBP_ReturnMain.WBP_ReturnMain_C"));
	if (EndWidgetClassRef.Class)
	{
		EndWidgetClass = EndWidgetClassRef.Class;
	}
	bReturnToMainMenuOpen = false;
}

void ASPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	if (IsLocalPlayerController())
	{
		SPHUDWidget = CreateWidget<USPHUDWidget>(this, SPHUDWidgetClass);
		if (SPHUDWidget)
		{
			SPHUDWidget->AddToViewport();
			SPHUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ASPPlayerController::ClientRPCReturnToMainMenu_Implementation(const TArray<FColorScoreData>& ColorScoreDataArray)
{
	if (EndWidgetClass == nullptr) return;
	if (EndWidget == nullptr)
	{
		EndWidget = CreateWidget<USPReturnToMainMenu>(this, EndWidgetClass);
	}

	if (EndWidget)
	{
		EndWidget->MenuSetup(ColorScoreDataArray);
	}
}

void ASPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

USPHUDWidget* ASPPlayerController::GetSPHUDWidget() const
{
	return SPHUDWidget;
}

void ASPPlayerController::ClientAIAlarmUI_Implementation()
{
	SPHUDWidget->UpdateAIInfoText();
}

void ASPPlayerController::ClientRCPMathState_Implementation(FName State)
{
	if (State == MatchState::InProgress)
	{
		if (SPHUDWidget)
		{
			SPHUDWidget->HideLoadingWidget();
		}
	}
}


void ASPPlayerController::ClientRPCSpawnUI_Implementation(const int32 Index)
{
	SPHUDWidget->UpdatePotionUI(Index);
}


void ASPPlayerController::ServerRPCSendMessage_Implementation(const FString& Sender, const FString& Message)
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		ASPGameModeBase* MyGameMode = Cast<ASPGameModeBase>(GameMode);
		if (MyGameMode)
		{
			MyGameMode->SendMessagesToEveryOne(Sender, Message);
		}
	}
}

void ASPPlayerController::ClientRPCAddMessageToChat_Implementation(const FString& Sender, const FString& Message)
{
	SPHUDWidget->UpdateChatting(Sender, Message);
}
