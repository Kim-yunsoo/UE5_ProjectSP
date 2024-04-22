// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPUserWidget.h"
#include "SPTargetUI.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPTargetUI : public USPUserWidget
{
	GENERATED_BODY()
public:
	//USPTargetUI(const FObjectInitializer& ObjectInitializer);
	void UpdateTargetUI(bool IsAim);
protected:
	virtual void NativeConstruct() override;
	
	
protected:
	UPROPERTY()
	TObjectPtr<class UImage> TargetUIImage;

};
