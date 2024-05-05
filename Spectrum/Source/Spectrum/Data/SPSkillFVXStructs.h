#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPGlobalEnum.h"
#include "SPSkillFVXStructs.generated.h"



USTRUCT()
struct FFVXAssetData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ProjectileFVX;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> HitFVX;
};


USTRUCT()
struct FSkillFVXData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Asset Data")
	ColorType MyColor;

	UPROPERTY(EditAnywhere, Category = "Asset Data")
	FFVXAssetData AssetData;
	
};