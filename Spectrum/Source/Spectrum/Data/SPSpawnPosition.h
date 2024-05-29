#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPSpawnPosition.generated.h"

USTRUCT(BlueprintType)
struct FPosition : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly )
	FVector Position;
};
