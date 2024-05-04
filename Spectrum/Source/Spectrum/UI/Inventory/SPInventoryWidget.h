// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPUserWidget.h"
#include "SPInventoryWidget.generated.h"

class USPItemBase;
class USPInventoryComponent;
class USPInventoryItemSlot;
class ASPCharacterPlayer;
class UWrapBox;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPInventoryWidget : public USPUserWidget
{
	GENERATED_BODY()

public:
	void RefreshInventory(TArray<USPItemBase*> Inventory);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> InventoryPanel;
	
	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter;
	
	UPROPERTY()
	TObjectPtr<USPInventoryComponent> InventoryReference;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USPInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText();
	virtual void NativeOnInitialized() override;
	// virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	// 	UDragDropOperation* InOperation) override;
};
