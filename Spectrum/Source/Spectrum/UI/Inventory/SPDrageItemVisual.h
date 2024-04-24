// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPUserWidget.h"
#include "SPDrageItemVisual.generated.h"

class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPDrageItemVisual : public USPUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual" , meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual" , meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};
