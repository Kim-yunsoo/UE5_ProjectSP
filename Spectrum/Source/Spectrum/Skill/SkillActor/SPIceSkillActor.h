// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActor/SPSkillActorBase.h"
#include "SPIceSkillActor.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPIceSkillActor : public ASPSkillActorBase
{
	GENERATED_BODY()
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ASPIceSkillActor();

protected:
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCIceSkill(const FHitResult& Hit );

	UFUNCTION()
	void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						   const FHitResult& Hit);
	
};
