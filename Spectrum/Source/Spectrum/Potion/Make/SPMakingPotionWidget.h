// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/SPInteractionInterface.h"
#include "Potion/SPItemBase.h"
#include "SPMakingPotionWidget.generated.h"


class USPInventoryItemSlot1;
class USPInventoryComponent;
class ASPCharacterPlayer;
class USPInventoryComponent;
class USPInventoryItemSlot;
class UWrapBox;
class USizeBox;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPMakingPotionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<USizeBox> YellowPotion;
	//
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<USizeBox> RedPotion;
	//
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<USizeBox> BluePotion;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop3;

	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<USPInventoryComponent> InventoryReference;
	
	// UPROPERTY(meta=(BindWidget))
	// TObjectPtr<UWrapBox> InventoryPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USPInventoryItemSlot> InventorySlotClass;
	
	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<USPInventoryItemSlot> InventorySlotClass;

	// UPROPERTY(meta=(BindWidget))
	// TObjectPtr<UWrapBox> InventoryPanel;
	
	void NativeConstruct() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//void UpdatePotionWidget(bool IsVisible);

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	TObjectPtr<USPItemBase> ItemReference;
	
};
