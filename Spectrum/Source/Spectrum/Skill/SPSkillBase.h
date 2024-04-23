// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SPSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	USPSkillBase();
	
protected:
	UPROPERTY()
	float CoolDown;

	UPROPERTY()
	TObjectPtr<UTexture> SkillTexture;

	UPROPERTY()
	TObjectPtr<AActor> Owner;



public:
	virtual void SkillAction(AActor* MyOwner);


};
