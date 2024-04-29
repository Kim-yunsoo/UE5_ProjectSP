// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSkillActorBase.generated.h"

UCLASS()
class SPECTRUM_API ASPSkillActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPSkillActorBase();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<class UBoxComponent> BoxCollision;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity ;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<UParticleSystemComponent> MainVFX;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TObjectPtr<UParticleSystem> EmitterHit;
	
	virtual void BeginPlay() override;
	
	uint8 bIsOnce :1 ;
};
