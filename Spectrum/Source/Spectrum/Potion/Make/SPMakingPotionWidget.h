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
	
public:
	UFUNCTION()
	void ClearWidget();

	UFUNCTION()
	void MakingPotion(USPItemBase* Item);
	
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Drop3;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> Make;


	
	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<USPInventoryComponent> InventoryReference;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USPInventoryItemSlot> InventorySlotClass;
	
	
	void NativeConstruct() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//void UpdatePotionWidget(bool IsVisible);

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	TObjectPtr<USPItemBase> ItemReference;

	UPROPERTY(EditAnywhere)
	USoundBase* ClickOn;

	UPROPERTY(EditAnywhere)
	USoundBase* DropSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* Back;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<USPItemBase*> InventoryMaking;

	UFUNCTION()
	void BackCallBack();

};
