// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPPickup.h"

#include "Potion/SPPotionBase.h"

// Sets default values
ASPPickup::ASPPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true); //�ùķ��̼� true ����غ���
	SetRootComponent(PickupMesh);
}

void ASPPickup::InitializePickup(const TSubclassOf<ASPPotionBase> CaseClass, const int32 InQuantity)
{
	if(ItemDataTable&& !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()); // ���ϴ� �׸� ã��

		//������ ȣ���ϱ�
		ItemReference = NewObject<ASPPotionBase>(this, ASPPotionBase::StaticClass());
	}
}

void ASPPickup::InitializeDrop(ASPPotionBase* ItemToDrop, const int32 InQuantity)
{
}

void ASPPickup::BeginFocus()
{

}

void ASPPickup::EndFocus()
{
	
}

void ASPPickup::Interact(ASPCharacterPlayer* PlayerCharacter)
{
	
}

void ASPPickup::TakePickup(const ASPCharacterPlayer* Taker)
{
}

// Called when the game starts or when spawned
void ASPPickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(ASPPotionBase::StaticClass(), ItemQuantity);
}

