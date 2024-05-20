#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPExplosionData.generated.h"


enum class ColorType : uint8;

USTRUCT(BlueprintType)
struct FExplosionData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	ColorType Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
	TObjectPtr<USoundWave> WaterSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
	TObjectPtr<USoundWave> CrushSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
	TObjectPtr<UParticleSystem> EmitterHit;
};
