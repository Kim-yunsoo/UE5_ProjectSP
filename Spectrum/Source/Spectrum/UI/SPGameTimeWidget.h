// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPGameTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPGameTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USPGameTimeWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	TObjectPtr<class UTextBlock> MatchCountDownText;
	
	void UpdateTime(const float Time);

protected:
	virtual void NativeConstruct() override;
	
};
