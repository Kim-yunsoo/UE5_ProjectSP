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
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UParticleSystem> ProjectileVfx;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UParticleSystem> HitVfx;
};
