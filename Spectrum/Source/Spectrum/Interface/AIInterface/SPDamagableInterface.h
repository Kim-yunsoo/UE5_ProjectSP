// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPDamagableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPDamagableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetCurrentHealth() =0 ;
	virtual float GetMaxHealth() =0; 
	virtual float Heal(float Amount) =0;
	virtual bool TakeDamage(float Amount,bool ShouldForceInterrupt)=0; // ShouldForceInterrupt�ִϸ��̼� �ߴܿ� ��� 
};
