// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
public:
	FGameTime GameTime;
	float ActivetedTimeStamp;

	virtual void SkillAction();

};
