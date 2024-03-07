// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOblyInputMode;
	SetInputMode(GameOblyInputMode);

}
