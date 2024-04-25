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
	// virtual void Tick(float DeltaSeconds) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// virtual void Tick(float DeltaSeconds) override;
public:
	virtual void SkillAction(ASPCharacterPlayer* MyOwner) override;
	
	
	UFUNCTION(NetMulticast, Unreliable)
	void TEXTRPCSkill();
	// UFUNCTION(Server, Unreliable)
	// void ServerRPC( const TArray<FHitResult>& OutHits);

	FGameTime GameTime;
	float ActivetedTimeStamp;
	UPROPERTY(Replicated)
	uint8 bIsActiveSlowSkill : 1;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UFUNCTION()
	// void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	// 					   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	// 					   const FHitResult& Hit);
	// UPROPERTY(Replicated)
	// TObjectPtr<AActor> TargetActor;
	// void RotateToTarget();
};
