// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPSkillActorBase.h"
#include "Character/SPCharacterPlayer.h"
#include "GameFramework/Actor.h"
#include "SPSlowSkillActor.generated.h"

class ISPSkillInterface;

UCLASS()
class SPECTRUM_API ASPSlowSkillActor : public ASPSkillActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPSlowSkillActor();
	// ASPSlowSkillActor(AActor* TargetPlayer);

protected:
	// UPROPERTY(Replicated)
	// TObjectPtr<UParticleSystemComponent> SlowVFX;
	//
	//
	// UPROPERTY(Replicated)
	// TObjectPtr<UParticleSystem> SlowEmitterHit;
	
	UPROPERTY()
	TObjectPtr<UDataTable> VFXDataTable;
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCSlowSkill(const FHitResult& Hit );

	
	UFUNCTION(Server, Unreliable)
	void ServerRPCSlowSkill(const FHitResult& Hit);
	
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<class UBoxComponent> BoxCollision;
	//
	//
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float Speed ;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float Gravity ;

	

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	uint8 bIsHoming:1;
	
	//collision 
	UFUNCTION()
	void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						   const FHitResult& Hit);

	void RotateToTarget();
public:
	void InitTarget( AActor* TargetPlayer);
	

	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	
};
