// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPItemBase.h"
#include "Component/SPInventoryComponent.h"

USPItemBase::USPItemBase() : bIsCopy(false), bIsPickup(false)
{
	
}

void USPItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

USPItemBase* USPItemBase::CreatePotionCopy() const
{
	//인벤토리에 있는건 포인터를 통해서 관리될 예정
	//추가할 때 복사해야한다.
	USPItemBase* PotionCopy = NewObject<USPItemBase>(StaticClass());

	PotionCopy->ID = this->ID;
	PotionCopy->Quantity = this->Quantity;
	PotionCopy->ItemType = this->ItemType;
	PotionCopy->ItemTextData = this->ItemTextData;
	PotionCopy->ItemNumericData = this->ItemNumericData;
	PotionCopy->ItemAssetData = this->ItemAssetData;
	PotionCopy->bIsCopy = true;
	return PotionCopy;
}

void USPItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		//10까지만 가능하게!
		Quantity = FMath::Clamp(NewQuantity, 0 , ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		if(OwningInventory)
		{
			// if(Quantity <= 0)
			// {
			// 	OwningInventory->RemoveSingleinstanceOfItem(this, this->ItemType);
			// }
		}
	}
}

void USPItemBase::Use(ASPCharacterPlayer* Character)
{
}
