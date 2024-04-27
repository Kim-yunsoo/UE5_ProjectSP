// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SPSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	USPSkillBase();
	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	float CoolDown;

	UPROPERTY()
	TObjectPtr<UTexture> SkillTexture;

	UPROPERTY()
	TObjectPtr<class ASPCharacterPlayer> Owner;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<class UBoxComponent> BoxCollision;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
//D:\UE_5.3\Engine\Source\Runtime\Engine\Classes\Particles\ParticleSystemComponent.h
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<UParticleSystemComponent> VFX;



	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// TObjectPtr<UParticleSystem> EmitterHit;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	// uint8 bIsHoming:1;

	//projectileSpeed
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float ProjectileSpeed =900.f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// uint8 bIsHoming :1;

public:
	virtual void SkillAction(ASPCharacterPlayer* MyOwner);

};
