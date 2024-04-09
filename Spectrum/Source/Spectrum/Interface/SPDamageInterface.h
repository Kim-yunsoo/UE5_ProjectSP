// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPDamageInterface
{
	GENERATED_BODY()

public:
	virtual void OnExplosionHit() = 0;
	virtual void OnChangeColorGreen() = 0;
};