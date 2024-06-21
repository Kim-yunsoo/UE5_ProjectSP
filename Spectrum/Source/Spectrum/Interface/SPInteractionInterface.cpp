// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SPInteractionInterface.h"


// Add default functionality here for any ISPInteractionInterface functions that are not pure virtual.
void ISPInteractionInterface::BeginFocus()
{
}

void ISPInteractionInterface::EndFocus()
{
}

void ISPInteractionInterface::BeginInteract()
{
}

void ISPInteractionInterface::EndInteract()
{
}

bool ISPInteractionInterface::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	return false;
}

