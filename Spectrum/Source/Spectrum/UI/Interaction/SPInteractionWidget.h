// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPUserWidget.h"
#include "SPInteractionWidget.generated.h"

class UProgressBar;
class UTextBlock;
struct FInteractableData;
class ASPCharacterPlayer;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPInteractionWidget : public USPUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	TObjectPtr<ASPCharacterPlayer> PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData);


protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UProgressBar> InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interation Widget | Interactable Data")
	float UpdateInteractionProgress();
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
