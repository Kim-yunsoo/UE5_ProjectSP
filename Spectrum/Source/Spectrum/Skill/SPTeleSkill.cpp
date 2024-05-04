// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPTeleSkill.h"

#include "Character/SPCharacterPlayer.h"
#include "SkillActor/SPTeleSkillActor.h"

USPTeleSkill::USPTeleSkill()
{
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	UActorComponent::SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled	= true;
	bAutoActivate = true;
	CoolDown = 10;
}

void USPTeleSkill::BeginPlay()
{
	Super::BeginPlay();
}

void USPTeleSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	
	if (Owner-> bIsActiveTeleSkill) 
	{
		return;
	}
	FGameTime CurrentGameTime = GetWorld()->GetTime();
	if (CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolDown)
	{

		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;
		float CDTime= FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f);
		ClientSkillRPC(CDTime);
	}
	else
	{
		float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolDown;
		float CDTime=  FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f)/CoolDown;
		ClientSkillRPC(CDTime);
		Owner->bIsActiveTeleSkill = true;
	}
}

void USPTeleSkill::SkillAction()
{
	Super::SkillAction();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	ASPTeleSkillActor* MyActor =GetWorld()->SpawnActor<ASPTeleSkillActor>(ASPTeleSkillActor::StaticClass(),Owner->SkillLocation->GetComponentLocation(),
											   Owner->SkillLocation->GetComponentRotation(), SpawnParams);
	MyActor->SetOwner(Owner);
}

void USPTeleSkill::ClientSkillRPC_Implementation(float Time)
{
	TeleCoolDown(Time);
}
