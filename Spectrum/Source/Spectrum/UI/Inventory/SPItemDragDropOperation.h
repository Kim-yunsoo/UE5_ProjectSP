// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SPItemDragDropOperation.generated.h"

class USPInventoryComponent;
class USPItemBase;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<USPItemBase> SourceItem;

	UPROPERTY()
	TObjectPtr<USPInventoryComponent>	SourceInventory;
	
};
