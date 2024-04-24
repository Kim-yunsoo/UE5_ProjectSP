// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "Skill/SPSkillCastComponent.h"
//
// #include "SpectrumLog.h"
// #include "SPSkillBase.h"
//
// // Sets default values for this component's properties
// USPSkillCastComponent::USPSkillCastComponent()
// {
// 	PrimaryComponentTick.bCanEverTick = true;
// }
//
//
// // Called when the game starts
// void USPSkillCastComponent::BeginPlay()
// {
// 	Super::BeginPlay();
// 	// this->SetIsReplicated(true);
// }
//
//
// // Called every frame
// void USPSkillCastComponent::TickComponent(float DeltaTime, ELevelTick TickType,
//                                           FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// }
//
// void USPSkillCastComponent::SetActiveSkill(UActorComponent* SkillType)
// {
// 	USPSkillBase* Skillbase = Cast<USPSkillBase>(SkillType);
//
// 	if (Skillbase)
// 	{
// 		SP_SUBLOG(LogSPNetwork, Log, TEXT("SetActiveSkill "));
//
// 		// Skillbase->SkillAction(GetOwner());
// 	}
// }
//
//
