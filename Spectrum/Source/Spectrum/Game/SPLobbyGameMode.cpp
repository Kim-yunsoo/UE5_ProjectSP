// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPLobbyGameMode.h"

void ASPLobbyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPLobbyGameMode::DefaultGameTimer, 5.0f,false);
}

void ASPLobbyGameMode::DefaultGameTimer()
{
	UE_LOG(LogTemp,Log,TEXT("Default"));
	GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen")); //다른 월드로 이동한다.

}
