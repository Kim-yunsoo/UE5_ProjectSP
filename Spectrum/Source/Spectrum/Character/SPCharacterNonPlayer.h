// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SPCharacterAIInterface.h"
#include "Interface/SPNonCharacterWidgetInterface.h"
#include "Interface/AIInterface/SPDamagableInterface.h"
#include "SPCharacterNonPlayer.generated.h"

class ASPAIController;

UCLASS()
class SPECTRUM_API ASPCharacterNonPlayer : public ACharacter, public ISPNonCharacterWidgetInterface,
                                           public ISPCharacterAIInterface, public ISPDamagableInterface
{
	GENERATED_BODY()

public:
	ASPCharacterNonPlayer();

	virtual void PostInitializeComponents() override;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;


	void AttackHitCheck();
	void SetDead();
	void PlayDeadAnimation();
	void DamageResponse();
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	float DeadEventDelayTime = 5.0f;

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class USPNonCharacterStatComponent> Stat;

	//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USPWidgetComponent> HpBar;

	UPROPERTY()
	TObjectPtr<class USPDamageSystemComponent> DamageSystemComponent;

	UPROPERTY()
	TObjectPtr<class USPAttackComponent> AttackComponent;

	UPROPERTY()
	float DefendRadius;
	UPROPERTY()
	float AttackRadius;

	UPROPERTY(Replicated)
	TObjectPtr<ASPAIController> AIController;

	virtual void SetupCharacterWidget(class USPUserWidget* InUserWidget) override;

	//AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void SetAITeleportDelegate(const FAICharacterTeleportFinished& InOnTeleportFinished) override;

	virtual void Attack(AActor* Target)override;

	UPROPERTY()
	TObjectPtr<AActor> MyTarget;

	virtual void AttackEndDelegate(UAnimMontage* Montage, bool bInterrupted);
	//virtual void TeleportEndDelegate(UAnimMontage* Montage, bool bInterrupted);
	FAICharacterAttackFinished OnAttackFinished;
	FAICharacterTeleportFinished OnTeleportFinished;

	// float MaxHealth;
	// float Health;
	// uint8 bIsDead : 1;
	bool Attacking;
	bool IsTeleporting;

public: //interface
	virtual float SetMovementSpeed(const MovementSpeed MoveSpeed) override;
	virtual float GetIdealAttackRange() override;
	virtual float GetIdealDefendRange() override;


	virtual float GetCurrentHealth() override;
	virtual float GetMaxHealth() override;
	virtual float Heal(float Amount) override;
	

	UFUNCTION(BlueprintCallable)
	virtual bool TakeDamage(float Amount, bool ShouldForceInterrupt) override;
	virtual bool IsDead() override;

	virtual void HitResponse();

	void Teleport(FVector Location);
	void TeleportEnd();
	
	
	//Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireBallMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> TeleportBodyParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> TeleportTrailParticle;

	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> TeleportBodyComponent;
	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> TeleportTrailComponent;
	UFUNCTION()
	void HandleMontageAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void HitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//¼­¹ö
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCTeleport();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCTeleportEnd();

	// UFUNCTION(Client, Unreliable)
	// void ClientRPCTeleport();

	void DeletParticle();
};
