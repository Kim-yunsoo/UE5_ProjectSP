// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SPLobbyWidgetGameModeBase.h"

ASPLobbyWidgetGameModeBase::ASPLobbyWidgetGameModeBase()
{
	bAlreadyTraveled = false;

}

void ASPLobbyWidgetGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FTimerHandle GameTimerHandle;
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPLobbyWidgetGameModeBase::DefaultGameTimer, 10.0f, false);
}

void ASPLobbyWidgetGameModeBase::DefaultGameTimer()
{
	if (bAlreadyTraveled == false) {
		bAlreadyTraveled = true;
		UE_LOG(LogTemp, Log, TEXT("Default"));
		//GetWorld()->ServerTravel(TEXT("/Game/Spectrum/Room/Map/Building?listen")); // 다른 월드로 이동한다.
		//okok = true;
	}
}

void ASPLobbyWidgetGameModeBase::goServer()
{
	UWorld* World = GetWorld();
	World->ServerTravel("192.168.41.20");

}
