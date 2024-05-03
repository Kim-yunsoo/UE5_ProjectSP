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

	if(IsLocalPlayerController())
	{
		SPHUDWidget = CreateWidget<USPHUDWidget>(this, SPHUDWidgetClass);
		if (SPHUDWidget)
		{
			SPHUDWidget->AddToViewport();
			SPHUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}

USPHUDWidget* ASPPlayerController::GetSPHUDWidget() const
{
	return SPHUDWidget;
}

void ASPPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	// SPHUDWidget* SPHUD = SPHUD== nullptr? Cast<USPHUDWidget>(GetHUD()) : SPHUD; 
	// int32 Minutes = FMath::FloorToInt(CountdownTime/60.f);
	//
	// int32 Seconds = CountdownTime - Minutes *60;
	//
	// FString CountdownText = FString::Printf(TEXT("%02d:%02d"),Minutes,Seconds);
	// // if(MatchCountDownText)
	// // {
	// // 	MatchCountDownText->SetText(FText::FromString(CountdownText));
	// // }
}
