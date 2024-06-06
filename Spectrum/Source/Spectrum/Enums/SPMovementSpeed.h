#pragma once
#include "CoreMinimal.h"
#include "SPMovementSpeed.generated.h"


UENUM(BlueprintType)
enum class MovementSpeed : uint8
{
	Idle,
	Walking,
	Jogging,
	Sprinting
};
