// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPSkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USPSkillWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
public:
	void UpdateSlowBar(float NewCurrentPercent);

protected:
	UPROPERTY()
	TObjectPtr<class UImage> SkillUI;
	
	UPROPERTY()
	TObjectPtr<class UProgressBar> SlowBar;
	
};
