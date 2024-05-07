// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakingPotionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "UI/Inventory/SPInventoryItemSlot.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "UI/SPHUDWidget.h"
#include "UI/Inventory/SPItemDragDropOperation.h"

class ASPMakePotion;

void USPMakingPotionWidget::ClearWidget()
{
	TArray<USizeBox*> DropWidgets = {Drop1, Drop2, Drop3};
	
	for(USizeBox* Drop : DropWidgets)
	{
		Drop->ClearChildren();
	}
	Make->ClearChildren();
	
	UE_LOG(LogTemp, Log, TEXT("SpectrumLocationCallBack!!!"));

	
}

void USPMakingPotionWidget::MakingPotion(USPItemBase* Item)
{
	USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);
	if(!ItemSlot)
		UE_LOG(LogTemp, Warning, TEXT("No ItemSlot"))
	else
	{
		ItemSlot->SetItemReference(Item);
		//ItemSlot->ItemReference->Quantity = 1;
		ItemSlot->HideText();
	}
	UE_LOG(LogTemp, Warning, TEXT("MakingPotion"))
	Make->AddChild(ItemSlot);
}

void USPMakingPotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	Back = Cast<UButton>(GetWidgetFromName(TEXT("Back")));
	Back->OnClicked.AddDynamic(this, &USPMakingPotionWidget::BackCallBack);
}

bool USPMakingPotionWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
    
	if (ItemDragDrop && PlayerCharacter && ItemDragDrop->SourceItem->Quantity != 0)
	{
		FVector2D DropLocation = InDragDropEvent.GetScreenSpacePosition();

		TArray<USizeBox*> DropWidgets = {Drop1, Drop2, Drop3};

		USizeBox* ClosestDropWidget = nullptr;
		float MinDistanceSquared = TNumericLimits<float>::Max();

		for (USizeBox* DropWidget : DropWidgets)
		{
			if (DropWidget)
			{
				
				FVector2D DropWidgetLocation = DropWidget->GetCachedGeometry().GetAbsolutePosition();

				float DistanceSquared = FVector2D::DistSquared(DropLocation, DropWidgetLocation);

				if (DistanceSquared < MinDistanceSquared)
				{
					MinDistanceSquared = DistanceSquared;
					ClosestDropWidget = DropWidget;
				}
			}
		}
		if (ClosestDropWidget)
		{
			ClosestDropWidget->ClearChildren();
			USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);
			if(!ItemSlot)
				UE_LOG(LogTemp, Warning, TEXT("No ItemSlot"))
			else
			{
				ItemSlot->SetItemReference(ItemDragDrop->SourceItem);
				ItemSlot->HideText();
			}
			ClosestDropWidget->AddChild(ItemSlot);
			//인벤토리 숫자 줄게 하기
			//여기서 서버로 가야함!
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwningPlayer()->GetName());
			PlayerCharacter->DragItem(ItemDragDrop->SourceItem,1);
			return true;
		}
	}
	return false;
}

// FReply USPMakingPotionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
// {
// 	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
//
// 	return Reply.Unhandled();
//
// }

void USPMakingPotionWidget::BackCallBack()
{
	UE_LOG(LogTemp, Log, TEXT("SpectrumLocationCallBack!!!"));
	PlayerCharacter->HUDWidget->UpdateMakingPotionWidget(false);
}
