// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSkillBase.h"
//D:\UE_5.3\Engine\Source\Runtime\Engine\Classes\GameFramework\ProjectileMovementComponent.h
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

class ISPSkillInterface;

USPSkillBase::USPSkillBase()
{
	CoolDown = -1;
	// this->SetIsReplicated(true);
}

void USPSkillBase::BeginPlay()
{
	Super::BeginPlay();
}

void USPSkillBase::SkillAction()
{
	// Owner = MyOwner;
	Owner=Cast<ASPCharacterPlayer>(GetOwner());
}