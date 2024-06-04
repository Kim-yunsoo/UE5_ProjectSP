// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "UI/HpBar/SPUserWidget.h"
#include "SPDrageItemVisual.generated.h"

class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPDrageItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual" , meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual" , meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};
