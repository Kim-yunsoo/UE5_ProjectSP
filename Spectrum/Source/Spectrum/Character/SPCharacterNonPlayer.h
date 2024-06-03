// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SPCharacterNonPlayer.generated.h"

UCLASS()
class SPECTRUM_API ASPCharacterNonPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPCharacterNonPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 void AttackHitCheck() ;
	void SetDead();
	void PlayDeadAnimation();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	float DeadEventDelayTime = 5.0f;

};
