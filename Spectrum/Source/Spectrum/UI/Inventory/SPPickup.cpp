// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPPickup.h"

#include "Potion/SPPotionBase.h"

// Sets default values
ASPPickup::ASPPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true); //시뮬레이션 true 고민해보기
	SetRootComponent(PickupMesh);
}

void ASPPickup::InitializePickup(const TSubclassOf<ASPPotionBase> CaseClass, const int32 InQuantity)
{
	if(ItemDataTable&& !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()); // 원하는 항목 찾기

		//아이템 호출하기
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

