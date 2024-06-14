#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SchoolImagestruct.generated.h"


enum class ColorType : uint8;

USTRUCT(BlueprintType)
struct FSchoolColorData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	ColorType Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Image" )
	TObjectPtr<UTexture> Texture;
	
};
