// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPInteractionInterface.generated.h"

class ASPCharacterPlayer;
class USPHUDWidget;

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"), //�̰� ���ص� �ɵ�
	Device UMETA(DisplayName = "Device"), // ���մ�, npc
	Toggle UMETA(DisplayName = "Toggle"), //����, �̰͵� �ʿ� ������
	Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData() :
	InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration(0.0f)
	{
		
	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	//pick up�� ���
	UPROPERTY(EditInstanceOnly)
	int32 Quantity;

	//���� ������ ����ϴµ� ������ �ʿ� ���� ��
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USPInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECTRUM_API ISPInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget);
	virtual void Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget);
	FInteractableData InteractableData;
};
