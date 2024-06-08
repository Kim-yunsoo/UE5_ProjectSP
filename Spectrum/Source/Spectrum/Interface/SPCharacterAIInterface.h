// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPCharacterAIInterface.generated.h"

enum class MovementSpeed : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
DECLARE_DELEGATE(FAICharacterAttackFinished);


class SPECTRUM_API ISPCharacterAIInterface
{
	GENERATED_BODY()
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;

	virtual void AttackByAI() = 0; //Attack
	virtual float SetMovementSpeed(const MovementSpeed MoveSpeed)=0;
	virtual float GetIdealAttackRange()=0;
	virtual float GetIdealDefendRange()=0;

	//virtual float Heal(float Amount)=0;
};
