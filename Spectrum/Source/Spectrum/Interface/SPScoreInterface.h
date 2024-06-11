// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPGlobalEnum.h"
#include "SPScoreInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPScoreInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPScoreInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddScore(const ColorType& MyColor,EScoreType ScoreType) = 0;
};
