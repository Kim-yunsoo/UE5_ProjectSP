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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnExplosionHit(float Damage) = 0;
	//virtual void OnChangeColorGreen() = 0;
};
