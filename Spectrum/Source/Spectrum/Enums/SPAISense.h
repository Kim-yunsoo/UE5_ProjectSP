#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "SPAISense.generated.h"


class UAISense_Damage;
class UAISense_Hearing;
class UAISense_Sight;

UENUM(BlueprintType,Blueprintable)
enum class EAISense : uint8
{
	None ,
	Sight ,
	Hearing ,
	Damage 
};

