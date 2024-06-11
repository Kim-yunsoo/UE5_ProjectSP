// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/SPWidgetInterface.h"
//#include "UI/SPUserWidget.h"
#include "SPInventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class USPDrageItemVisual;
class USPItemBase;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(USPItemBase* ItemIn) { ItemReference = ItemIn;};
	FORCEINLINE USPItemBase* GetItemReference() const {return ItemReference;}
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
public:
	void HideText();
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	TObjectPtr<USPItemBase> ItemReference;
protected:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<USPDrageItemVisual> DragItemVisualClass;
	


	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot" , meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot" , meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	FWindowsPlatformTime MouseDownTime;

	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter;

	UPROPERTY()
	bool ClickResult;
	
	UFUNCTION(Server, Unreliable)
	void ServerRPCButtonUp(int num);

	
	
	UPROPERTY(EditAnywhere)
	USoundBase* ClickOn;
	
};
