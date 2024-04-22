// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSkillBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


class ISPSkillInterface;

USPSkillBase::USPSkillBase()
{
	CoolDown = -1;
	bIsActiveSlowSkill = true;
}

void USPSkillBase::SkillAction(AActor* MyOwner)
{
	Owner = MyOwner;
}

void USPSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USPSkillBase, bIsActiveSlowSkill);
}
