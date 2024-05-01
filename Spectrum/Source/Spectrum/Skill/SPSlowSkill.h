// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SPSkillBase.h"
#include "SPSlowSkill.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlowCDChangedDelegate, float /*CurrentHp*/);

UCLASS()
class SPECTRUM_API USPSlowSkill : public USPSkillBase
{
	GENERATED_BODY()
	USPSlowSkill();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FOnSlowCDChangedDelegate OnSlowCDChange;
	FORCEINLINE void SlowCoolDown(float CDtime){OnSlowCDChange.Broadcast(CDtime);}
	virtual void SkillAction() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client,Unreliable)
	void ClientSkillRPC(float Time);

	uint8 bIsOnce:1;
};
