// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SPTeleportData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTeleportRangeRowBase : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float MinRow ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float MaxRow ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float MinCol ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float MaxCol ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range")
	float TeleportZ ;

	
};
