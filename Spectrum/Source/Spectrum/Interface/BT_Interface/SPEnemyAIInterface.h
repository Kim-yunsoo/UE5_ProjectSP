// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPEnemyAIInterface.generated.h"

enum class MovementSpeed : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//float SetMovementSpeed(const MovementSpeed MoveSpeed);
};
