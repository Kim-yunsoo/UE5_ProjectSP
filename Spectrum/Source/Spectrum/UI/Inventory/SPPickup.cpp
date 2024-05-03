// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SPPickup.h"
#include "Component/SPInventoryComponent.h"
#include "Potion/SPItemBase.h"
#include "SpectrumLog.h"

// Sets default values
ASPPickup::ASPPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true); //�ùķ��̼� true ����غ���
	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void ASPPickup::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}
	InitializePickup(USPItemBase::StaticClass(), ItemQuantity);
}

void ASPPickup::InitializePickup(const TSubclassOf<USPItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		// ���ϴ� �׸� ã��

		ItemReference = NewObject<USPItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;

		//������ ȣ���ϱ�
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);


		UpdateInteractableData();
	}
}

void ASPPickup::InitializeDrop(USPItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	PickupMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);
	UpdateInteractableData();
}

void ASPPickup::BeginFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
	
}

void ASPPickup::EndFocus()
{
	if(PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void ASPPickup::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	//SetOwner(PlayerCharacter);
	//ServerRPCInteract(PlayerCharacter, HUDWidget);
	if(PlayerCharacter)
	{
		AActor* TEST = Cast<AActor>(PlayerCharacter);
		this->SetOwner(TEST);
		TakePickup(PlayerCharacter);
	}
}

void ASPPickup::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	ISPInteractionInterface::Interact2(PlayerCharacter, HUDWidget);
}

void ASPPickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.Description;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void ASPPickup::TakePickup(ASPCharacterPlayer* Taker)
{

	if (!IsPendingKillPending()) //IsPendingKillPending() �����Ǵ��� Ȯ��
	{
		if(ItemReference)
		{
			//�κ��丮 �ְ� ���� �Ǹ� �׸��� �����ϰų� �ı�
			if (USPInventoryComponent* PlayerInvetory = Taker->GetInventory())
			{
				FTimerHandle TimerHandle;
				PlayerInvetory->HandleAddItem(ItemReference);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this](){
					                                       this->Destroy();
				                                       }, 0.1f, false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory is null"));

			}
		}
		
	}
}

void ASPPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Super::PostEditChangeProperty(PropertyChangedEvent);
	//
	// const FName ChangedPropertyName = PropertyChangedEvent.Property?PropertyChangedEvent.Property->GetFName() : NAME_None;
	//
	//Todo �����Ϳ��� ���ϰ� �ϱ� ���ؼ�!
}

void ASPPickup::ServerRPCInteract_Implementation(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	SP_LOG(LogSPNetwork,Log,TEXT("ServerRPCInteract_Implementation"));

	
}

void ASPPickup::ClientRPCUpdateWidget_Implementation(ASPCharacterPlayer* Taker)
{
	SP_LOG(LogSPNetwork,Log,TEXT("ClientRPC"));
	
	// SP_LOG(LogSPNetwork, Log, TEXT("TEST"));
	// for(USPItemBase* const& InventoryItem : PlayerInvetory->GetInventoryContents())
	// {
	// 	SP_LOG(LogSPNetwork, Log, TEXT("%s"), *InventoryItem->ItemTextData.Name.ToString());
	// }
	
}



