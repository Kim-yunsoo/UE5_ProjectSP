// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPDamageSystemComponent.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHpDamageResponseDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECTRUM_API USPDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	USPDamageSystemComponent();

	FOnHpZeroDelegate OnHpZero;
	FOnHpDamageResponseDelegate OnDamageResponse;
	FOnHpChangedDelegate OnHpChanged;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE float GetMaxHp() { return MaxHealth; }
	FORCEINLINE float GetCurrentHp() { return Health; }

	
	float Health;
	float MaxHealth;
	bool IsDead;
	bool IsInterruptible;
	void SetHp(float NewHp);
	float Heal(float Amount);


	
	bool TakeDamage(float Amount, bool ShouldForceInterrupt);
};
