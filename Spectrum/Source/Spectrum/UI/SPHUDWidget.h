// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/SPCharacterPlayer.h"
#include "SPHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USPHUDWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;


protected:
	UPROPERTY()
	TObjectPtr<class USPTargetUI> TargetUI;

	UPROPERTY()
	TObjectPtr<class USPInventoryWidget> Inventory;
};
