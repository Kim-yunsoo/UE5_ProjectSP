// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPGlobalEnum.h"
#include "SPScoreInterface.generated.h"

enum class EScoreType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPScoreInterface : public UInterface
{
	GENERATED_BODY()
};

class SPECTRUM_API ISPScoreInterface
{
	GENERATED_BODY()

public:
	virtual void AddScore(const ColorType& MyColor,EScoreType ScoreType) = 0;
};
