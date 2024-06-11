#pragma once

#include "CoreMinimal.h"
#include "SPScoreType.generated.h"

UENUM(BlueprintType,Blueprintable)
enum class EScoreType : uint8
{
	Default,
	AttackDefault,
	AttackSpecial
};

