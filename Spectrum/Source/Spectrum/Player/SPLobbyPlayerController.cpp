// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPLobbyPlayerController.h"

#include "SpectrumLog.h"
#include "SPLobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
//#include "UI/SPSelectWidget.h"

ASPLobbyPlayerController::ASPLobbyPlayerController()
{
	ReadyCount=0;
}

void ASPLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 게임 모드 설정
	FInputModeGameAndUI GameInputMode;
	SetInputMode(GameInputMode);
	SetShowMouseCursor(true);
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

