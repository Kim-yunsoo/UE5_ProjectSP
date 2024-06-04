// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "SPHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPHpBarWidget : public USPUserWidget
{
	GENERATED_BODY()

public:
	USPHpBarWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	//������ HP���� �������ֱ� ����
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
	
};
