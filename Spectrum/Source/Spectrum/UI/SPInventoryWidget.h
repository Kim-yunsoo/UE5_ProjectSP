// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPUserWidget.h"
#include "SPInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPInventoryWidget : public USPUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
};
