// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPObject.h"
#include "GameFramework/Actor.h"
#include "SPSimulateObject.generated.h"

UCLASS()
class SPECTRUM_API ASPSimulateObject : public ASPObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPSimulateObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
