// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPComponentBase.h"
#include "Components/ActorComponent.h"
#include "SPExplosionComponent.generated.h"

//enum class ColorType:int8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPECTRUM_API USPExplosionComponent : public USPComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USPExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	uint8 bHasExecuted:1; 

public:
	virtual void Explode(ColorType& MyColor) override;
	//RPC
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCExplosion(const TArray<AActor*>& Array,const ColorType& MyColor);
};
