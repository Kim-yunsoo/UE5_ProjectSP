// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderTargetParameters.h"
#include "SPGlobalEnum.h"
#include "Blueprint/UserWidget.h"
#include "SPScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USPScoreWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateScore(const ColorType& MyColor,const int32 MyScore );

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TMap<ColorType, class UTextBlock*> Score;
	
};
