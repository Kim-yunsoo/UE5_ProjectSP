// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/Make/SPMakingPotionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "UI/Inventory/SPInventoryItemSlot.h"
#include "Character/SPCharacterPlayer.h"
#include "Component/SPInventoryComponent.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/SPItemDragDropOperation.h"

class ASPMakePotion;

void USPMakingPotionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	

	// ASPMakePotion* PotionMaker = Cast<ASPMakePotion>(GetWorld()->SpawnActor(ASPMakePotion::StaticClass()));
	// if (PotionMaker)
	// {
	// 	// ��������Ʈ�� ������ �Լ��� ���ε�
	// 	PotionMaker->OnWidgetDelegate.AddDynamic(this, &USPMakingPotionWidget::UpdatePotionWidget);
	// }
}

bool USPMakingPotionWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	// const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
	// USPInventoryItemSlot* ItemSlot = CreateWidget<USPInventoryItemSlot>(this, InventorySlotClass);

	// if(Drop1)
	// {
	// 	ItemSlot->SetItemReference(ItemDragDrop->SourceItem);
	// 	Drop1->AddChild(ItemSlot);
	// }

	
	const USPItemDragDropOperation* ItemDragDrop = Cast<USPItemDragDropOperation>(InOperation);
    
	if (ItemDragDrop && PlayerCharacter && ItemDragDrop->SourceItem)
	{
		FVector2D DropLocation = InDragDropEvent.GetScreenSpacePosition();

		// �迭�� Drop �������� �߰��� �� �ֵ��� ����
		TArray<USizeBox*> DropWidgets = {Drop1, Drop2, Drop3};

		USizeBox* ClosestDropWidget = nullptr;
		float MinDistanceSquared = TNumericLimits<float>::Max();
		//UE_LOG(LogTemp, Warning, TEXT("HERE"));
		// ItemSlot�� ���� Drop ������ ã��
		for (USizeBox* DropWidget : DropWidgets)
		{
			//UE_LOG(LogTemp, Warning, TEXT("HERE1"));

			if (DropWidget)
			{
				//UE_LOG(LogTemp, Warning, TEXT("HERE2"));

				// Drop ������ ���� ��ġ�� ������
				FVector2D DropWidgetLocation = DropWidget->GetCachedGeometry().GetAbsolutePosition();

				// ��� ��ġ�� Drop ������ �Ÿ� ���
				float DistanceSquared = FVector2D::DistSquared(DropLocation, DropWidgetLocation);

				// ���� ����� Drop ������ ã��
				if (DistanceSquared < MinDistanceSquared)
				{
					//UE_LOG(LogTemp, Warning, TEXT("HERE3"));

					MinDistanceSquared = DistanceSquared;
					ClosestDropWidget = DropWidget;
				}
			}
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("HERE5"));
		if (ClosestDropWidget)
		{
			// if(ClosestDropWidget->IsChildOf(ClosestDropWidget))
			// {
			// 	ClosestDropWidget->ClearChildren();
			// 	UE_LOG(LogTemp, Warning, TEXT("Child"))
			// }
			// else
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("NoChild"))
			//
			// }
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
			//�κ��丮 ���� �ٰ� �ϱ�
			PlayerCharacter->DragItem(ItemDragDrop->SourceItem,1);
			return true;
		}
	}
	return false;
}

FReply USPMakingPotionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	// UE_LOG(LogTemp, Warning, TEXT("BACK Inventory1"));
	// if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	// {
	// 	ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(GetOwningPlayerPawn());
	// 	Player->BackItem(ItemReference, 1);
	// 	UE_LOG(LogTemp, Warning, TEXT("BACK Inventory2"));
	// 	return Reply.Handled();
	// }

	return Reply.Unhandled();

}

// void USPMakingPotionWidget::UpdatePotionWidget(bool IsVisible)
// {
// 	if(IsVisible)
// 		this->SetVisibility(ESlateVisibility::Visible);
// 	else
// 	{
// 		this->SetVisibility(ESlateVisibility::Hidden);
// 	}
// }

// UUserWidget* USPMakingPotionWidget::CreateItemWidget(USPItemBase* Item)
// {
// 	// Item�� ���� UI ���� ����
// 	UUserWidget* ItemWidget = CreateWidget<UUserWidget>(this, USPMakingPotionWidget);
// 	// ������ ������ ������ �����ϴ� �ڵ� �߰�
// 	return ItemWidget;
// }