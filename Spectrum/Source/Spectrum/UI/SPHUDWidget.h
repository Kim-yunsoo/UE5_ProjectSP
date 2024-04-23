// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPHUDWidget.generated.h"


class USPInteractionWidget;
class USPMainMenu;

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USPHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;


protected:
	UPROPERTY()
	TObjectPtr<class USPTargetUI> TargetUI;

	UPROPERTY()
	TObjectPtr<class USPMainMenu> MainMenuWidget;
	
	UPROPERTY()
	TObjectPtr<class USPInteractionWidget> InteractionWidget;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USPMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USPInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;


	/////////////////
	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
protected:
};
