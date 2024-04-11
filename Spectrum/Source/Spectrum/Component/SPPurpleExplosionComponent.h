// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SPComponentBase.h"
#include "SPPurpleExplosionComponent.generated.h"


UCLASS()
class SPECTRUM_API USPPurpleExplosionComponent : public USPComponentBase
{
	GENERATED_BODY()
	// Sets default values for this component's properties
	USPPurpleExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Explode() override;
	
};
