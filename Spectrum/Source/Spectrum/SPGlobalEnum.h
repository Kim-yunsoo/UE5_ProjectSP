#pragma once
#include "CoreMinimal.h"
#include "SPGlobalEnum.generated.h"


UENUM(BlueprintType)
enum class ColorType : uint8
{
	Green = 0,
	Orange = 1,
	Purple = 2,
	Black,
	None
};

UENUM(BlueprintType)
enum class GenderType : uint8
{
	Man,
	Woman
};