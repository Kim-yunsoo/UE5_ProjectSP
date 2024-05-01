// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SPSkillBase.h"
#include "SPIceSkill.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnIceCDChangedDelegate, float /*CurrentHp*/);

UCLASS()
class SPECTRUM_API USPIceSkill : public USPSkillBase
{
	GENERATED_BODY()
protected:
	USPIceSkill();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FOnIceCDChangedDelegate OnIceCDChange;
	FORCEINLINE void IceCoolDown(float CDtime){OnIceCDChange.Broadcast(CDtime);}
	virtual void SkillAction() override;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client,Unreliable)
	void ClientSkillRPC(float Time);
	
};
