#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SPWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
