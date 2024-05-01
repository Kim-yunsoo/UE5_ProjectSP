// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPSkillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable,MinimalAPI)
class USPSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPSkillInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HitSlowSkillResult()=0;
	virtual void HitIceSkillResult()=0;
	virtual void HitTeleSkillResult()=0;
public:
};
