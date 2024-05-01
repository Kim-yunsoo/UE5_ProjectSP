// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPColorDataTable.generated.h"

UENUM(BlueprintType)
enum class ColorType : uint8
{
	Green = 0,
	Orange = 1,
	Purple = 2,
	None
};

USTRUCT(BlueprintType)
struct FColorData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	ColorType Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ColorData")
	FVector4 ColorData;
};
