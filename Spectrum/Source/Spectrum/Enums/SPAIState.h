#pragma once
#include "CoreMinimal.h"
#include "SPAIstate.generated.h"


UENUM(BlueprintType,Blueprintable)
enum class AIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead")
};
