// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPPotionSpawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPPotionSpawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPPotionSpawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	virtual void SpawnEvent() = 0;
public:
};
