// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPReturnToMainMenu.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnDestorySession(bool bWasSuccessful);

public:
	void MenuSetup();
	void MenuTearDown();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UFUNCTION()
	void ReturnButtonClicked();

	UPROPERTY()
	TObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;
	UPROPERTY()
	APlayerController* PlayerController;

};
