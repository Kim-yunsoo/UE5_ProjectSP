// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

FItemAddResult UInventoryComponent::HandleAddItem(USPItemBase* ItemIn)
{
}

USPItemBase* UInventoryComponent::FindMatchingItem(USPItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if(InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

USPItemBase* UInventoryComponent::FindNextItemByID(USPItemBase* ItemIn) const
{
	
}

USPItemBase* UInventoryComponent::FindNextPartialStack(USPItemBase* ItemIn) const
{
}

void UInventoryComponent::RemoveSingleinstanceOfItem(USPItemBase* ItemIn)
{
}

int32 UInventoryComponent::RemoveAmountOfItem(USPItemBase* ItemIn, int32 DesiredAmountToRemove)
{
}

void UInventoryComponent::SplitExistingStack(USPItemBase* ItemIn, const int32 AmountToSplit)
{
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(USPItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::HandleStackableItems(USPItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::CalculateWeightAddAmount(USPItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::CalculatenumberForFullStack(USPItemBase* ExistingItem, int32 InirialRequestedAddAmount)
{
}

void UInventoryComponent::AddNewItem(USPItemBase* Item, const int32 AmountToAdd)
{
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

