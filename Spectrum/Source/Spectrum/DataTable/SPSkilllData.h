// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SPSkilllData.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPSkilllData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture> SkillTexture;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float CoolDown;

	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	// TObjectPtr<class UNiagaraSystem> Niagara;
};
