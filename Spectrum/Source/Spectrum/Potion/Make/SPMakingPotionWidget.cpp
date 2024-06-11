// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakingPotionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UI/Inventory/SPInventoryItemSlot.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "UI/SPHUDWidget.h"
#include "UI/Inventory/SPItemDragDropOperation.h"

class ASPMakePotion;

void USPMakingPotionWidget::ClearWidget() // 완제품이 완성되면 3개 포션 위치를 지우는 로직 
{
	TArray<USizeBox*> DropWidgets = {Drop1, Drop2, Drop3};
	
	for(USizeBox* Drop : DropWidgets)
	{
		Drop->ClearChildren();
	}
	Make->ClearChildren();
}

void USPMakingPotionWidget::MakingPotion(USPItemBase* Item)
{
	USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);
	if(!ItemSlot)
		UE_LOG(LogTemp, Warning, TEXT("No ItemSlot"))
	else
	{
		ItemSlot->SetItemReference(Item);
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

	ClickOn = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/ClickOn.ClickOn'"));
	DropSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Spectrum/Sound/Drop.Drop'"));
}

bool USPMakingPotionWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
    
	if ((ItemDragDrop && PlayerCharacter && ItemDragDrop->SourceItem->Quantity != 0) || (ItemDragDrop && PlayerCharacter && PlayerCharacter->bIsSeven == true))
	{
		FVector2D DropLocation = InDragDropEvent.GetScreenSpacePosition();
		//내가 놓은 위치 

		TArray<USizeBox*> DropWidgets = {Drop1, Drop2, Drop3}; //위에있는거

		USizeBox* ClosestDropWidget = nullptr;
		float MinDistanceSquared = TNumericLimits<float>::Max();

		for (USizeBox* DropWidget : DropWidgets) //어느 위치에 드롭할건지 ? 
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
		
		if (ClosestDropWidget && ClosestDropWidget->GetChildrenCount() <=0) //위치가 정해졌으니까 
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
			UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwningPlayer()->GetName());
			PlayerCharacter->DragItem(ItemDragDrop->SourceItem,1);
			PlaySound(DropSound);
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
	//UE_LOG(LogTemp, Log, TEXT("SpectrumLocationCallBack!!!"));
	PlayerCharacter->HUDWidget->UpdateMakingPotionWidget(false);

	PlaySound(ClickOn);
}
