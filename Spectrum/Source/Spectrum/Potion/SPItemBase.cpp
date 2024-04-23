// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPItemBase.h"

USPItemBase::USPItemBase()
{
	
}

USPItemBase* USPItemBase::CreatePotionCopy()
{
	//�κ��丮�� �ִ°� �����͸� ���ؼ� ������ ����
	//�߰��� �� �����ؾ��Ѵ�.
	USPItemBase* PotionCopy = NewObject<USPItemBase>(StaticClass());

	PotionCopy->ID = this->ID;
	PotionCopy->Quantity = this->Quantity;
	PotionCopy->ItemType = this->ItemType;
	PotionCopy->ItemTextData = this->ItemTextData;
	PotionCopy->ItemNumericData = this->ItemNumericData;
	PotionCopy->ItemAssetData = this->ItemAssetData;

	return PotionCopy;
}

void USPItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		//10������ �����ϰ�!
		Quantity = FMath::Clamp(NewQuantity, 0 , ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		// if(OwningInventory)
		// {
		// 	if(Quantity <= 0)
		// 	{
		// 		OwningInventory->RemoveItem(this);
		// 	}
		// }
	}
}

void USPItemBase::Use(ASPCharacterPlayer* Character)
{
}
