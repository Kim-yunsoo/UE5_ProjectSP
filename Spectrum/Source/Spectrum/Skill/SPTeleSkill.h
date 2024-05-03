// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SPSkillBase.h"
#include "SPTeleSkill.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeleCDChangedDelegate, float /*CurrentHp*/);

UCLASS()
class SPECTRUM_API USPTeleSkill : public USPSkillBase
{
	GENERATED_BODY()

	USPTeleSkill();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FOnTeleCDChangedDelegate OnTeleCDChange;
	FORCEINLINE void TeleCoolDown(float CDtime){OnTeleCDChange.Broadcast(CDtime);}
	virtual void SkillAction() override;

protected:
	UFUNCTION(Client,Unreliable)
	void ClientSkillRPC(float Time);
	
};
