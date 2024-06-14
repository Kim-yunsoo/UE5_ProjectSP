#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPGlobalEnum.h"
#include "SPColorScoreData.generated.h"

USTRUCT(BlueprintType)
struct FColorScoreData
{
	GENERATED_BODY()
public:
	FColorScoreData() : Color(ColorType::None), Score(0)
	{
	}

	FColorScoreData(const ColorType& InColor,const int32 InScore ) : Color(InColor), Score(InScore)
	{
		
	};
	UPROPERTY()
	ColorType Color;
	
	UPROPERTY()
	int32 Score;
};