// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SPSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPSkillBase : public UObject
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

	UPROPERTY(Replicated)
	uint8 bIsActiveSlowSkill : 1;

public:
	virtual void SkillAction(AActor* MyOwner);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
