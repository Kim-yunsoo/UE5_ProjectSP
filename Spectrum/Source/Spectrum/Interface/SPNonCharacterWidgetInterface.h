// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPNonCharacterWidgetInterface.generated.h"

class USPUserWidget;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPNonCharacterWidgetInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class SPECTRUM_API ISPNonCharacterWidgetInterface
{
	GENERATED_BODY()
public:
	virtual void SetupCharacterWidget(USPUserWidget* InUserWidget) = 0;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
