// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SPSkillBase.h"
#include "SPSlowSkill.generated.h"

UCLASS()
class SPECTRUM_API USPSlowSkill : public USPSkillBase
{
	GENERATED_BODY()
	USPSlowSkill();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// virtual Tick
public:
	virtual void SkillAction(AActor* MyOwner) override;
	
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCSkill( const TArray<FHitResult>& OutHits);
	

protected:
	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
