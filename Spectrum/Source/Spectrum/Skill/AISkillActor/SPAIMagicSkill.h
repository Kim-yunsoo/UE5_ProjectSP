// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPAIMagicSkill.generated.h"



UCLASS()
class SPECTRUM_API ASPAIMagicSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPAIMagicSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<class UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity ;
	
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> MainVFX;
	
	UPROPERTY()
	TObjectPtr<UParticleSystem> EmitterHit;
	
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UPROPERTY()
	uint8 bIsHoming:1;

	uint8 bDoOnce :1 ;

	FVector TargetLocation;
	
	
	
	UFUNCTION()
	void OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						   const FHitResult& Hit);

public:
	void InitTarget(AActor* TargetPlayer);
	void RotateToTarget();
};
