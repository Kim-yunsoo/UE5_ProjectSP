// Fill out your copyright notice in the Description page of Project Settings.


#include "SPLobbyGameMode.h"

#include "SpectrumLog.h"
#include "SPGameState.h"
#include "SPPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/SPLobbyPlayerController.h"

ASPLobbyGameMode::ASPLobbyGameMode()
{
	PlayerControllerClass = ASPLobbyPlayerController::StaticClass();
	PlayerStateClass = ASPPlayerState::StaticClass();
	GameStateClass = ASPGameState::StaticClass();
	bUseSeamlessTravel = true;

	ReadyCount=0;
}

void ASPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ASPLobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPLobbyGameMode, ReadyCount);
}

void ASPLobbyGameMode::Ready()
{
	++ReadyCount;
	if(ReadyCount==2)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen"));
	}
}





