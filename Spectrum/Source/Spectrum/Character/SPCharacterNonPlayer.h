// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SPCharacterAIInterface.h"
#include "Interface/SPNonCharacterWidgetInterface.h"
#include "SPCharacterNonPlayer.generated.h"

UCLASS()
class SPECTRUM_API ASPCharacterNonPlayer : public ACharacter ,public ISPNonCharacterWidgetInterface, public ISPCharacterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPCharacterNonPlayer();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
	

	 void AttackHitCheck() ;
	void SetDead();
	void PlayDeadAnimation();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	float DeadEventDelayTime = 5.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USPNonCharacterStatComponent> Stat;

	//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USPWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class USPUserWidget* InUserWidget) override;

	//AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
};
