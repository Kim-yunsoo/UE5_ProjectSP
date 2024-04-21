// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPPlayerController.h"
#include "UI/SPHUDWidget.h"

ASPPlayerController::ASPPlayerController()
{
	static ConstructorHelpers::FClassFinder<USPHUDWidget> SPHUDWidgetRef(TEXT("/Game/Spectrum/UMG/WBP_SPHUD.WBP_SPHUD_C"));
	if (SPHUDWidgetRef.Class)
	{
		SPHUDWidgetClass = SPHUDWidgetRef.Class;
	}
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOblyInputMode;
	SetInputMode(GameOblyInputMode);

	if(!HasAuthority() || (HasAuthority() && GetRemoteRole() == ROLE_SimulatedProxy))
	{
		SPHUDWidget = CreateWidget<USPHUDWidget>(this, SPHUDWidgetClass);
		if (SPHUDWidget)
		{
			SPHUDWidget->AddToViewport();
			SPHUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}
