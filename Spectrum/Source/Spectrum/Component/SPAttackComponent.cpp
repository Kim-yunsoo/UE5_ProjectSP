// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SPAttackComponent.h"

#include "Character/SPCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Skill/AISkillActor/SPAIMagicSkill.h"

// Sets default values for this component's properties
USPAttackComponent::USPAttackComponent()
{
	bWantsInitializeComponent = true;
}


// Called when the game starts
void USPAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}
void USPAttackComponent::InitializeComponent()
{
	SetIsReplicated(true);
	Super::InitializeComponent();
}

void USPAttackComponent::MagicSpell(AActor* Target, FTransform Transform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	SpawnParams.Owner = GetOwner();

	ASPAIMagicSkill* MyActor = GetWorld()->SpawnActorDeferred<ASPAIMagicSkill>(
		ASPAIMagicSkill::StaticClass(), Transform, GetOwner());
	if (MyActor)
	{
		//GetOwner()
		MyActor->InitTarget(Target);
		MyActor->FinishSpawning(Transform);
	}
	ASPCharacterNonPlayer* AICharacter = Cast<ASPCharacterNonPlayer>(GetOwner());
	if (AICharacter)
	{
		AICharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(MyActor, true);
	}
	// ASPCharacterNonPlayer* AIPlayer = Cast<ASPCharacterNonPlayer>(GetOwner());
	// AIPlayer->GetCapsuleComponent()->IgnoreActorWhenMoving(MyActor, true);
	//end 델리게이트 ?? 왜쓰는거지 ? 
}

