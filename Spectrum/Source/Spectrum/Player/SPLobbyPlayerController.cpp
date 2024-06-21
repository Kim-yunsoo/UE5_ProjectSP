// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPLobbyPlayerController.h"

#include "SpectrumLog.h"
#include "SPLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "UI/Select/SPSelectWidget.h"

ASPLobbyPlayerController::ASPLobbyPlayerController()
{
	ReadyCount=0;
	
	static ConstructorHelpers::FClassFinder<USPSelectWidget> LobbyWidgetRef(
	TEXT("/Game/Spectrum/Yunsoo/UI/WBP_SelectWidget.WBP_SelectWIdget_C"));
	if (LobbyWidgetRef.Class)
	{
		LobbyWidgetClass = LobbyWidgetRef.Class;
	}
}

void ASPLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 게임 모드 설정
	FInputModeGameAndUI GameInputMode;
	SetInputMode(GameInputMode);
	SetShowMouseCursor(true);

	if (IsLocalPlayerController())
	{
		LobbyWidget = CreateWidget<USPSelectWidget>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			LobbyWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ASPLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPLobbyPlayerController, ReadyCount);

}

void ASPLobbyPlayerController::ServerRPCReady_Implementation()
{
	ASPLobbyGameMode* LobbyMode = Cast<ASPLobbyGameMode>(GetWorld()->GetAuthGameMode());
	if(LobbyMode)
	{
		LobbyMode->Ready();
	}
	
}

