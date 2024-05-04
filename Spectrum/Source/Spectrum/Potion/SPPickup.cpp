// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPickup.h"
#include "Component/SPInventoryComponent.h"
#include "Potion/SPItemBase.h"
#include "SpectrumLog.h"
#include "SPPotionSpawner.h"
#include "Components/BoxComponent.h"

// Sets default values
ASPPickup::ASPPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTriggerComponent"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/Item.Item'"));
	if (DataTableFinder.Succeeded())
	{
		ItemDataTable = DataTableFinder.Object;
	}
	PickupMesh->SetSimulatePhysics(false);
	Trigger->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ASPPickup::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}
	InitializePickup(USPItemBase::StaticClass(), ItemQuantity);

	FVector ActorLocation = GetActorLocation();
	Trigger->SetRelativeLocation(ActorLocation);
	Trigger->SetRelativeScale3D(FVector(1, 1, 3));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPPickup::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPPickup::OnTriggerExit);
}

void ASPPickup::InitializePickup(const TSubclassOf<USPItemBase> BaseClass, const int32 InQuantity)
{
	DesiredItemID = FName(TEXT("R_Mini"));
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
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void ASPPickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void ASPPickup::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) //서버에서 
{
	//SetOwner(PlayerCharacter);
	//ServerRPCInteract(PlayerCharacter, HUDWidget);
	if (PlayerCharacter)
	{
		MyPlayerOwner=PlayerCharacter;
		//this->SetOwner(PlayerCharacter);
		TakePickup(PlayerCharacter);
	}
}
void ASPPickup::Interact2(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) //클라에서 ,인터페이스라서 구현만 해놓은 것이다.
{
	//쓰이는 곳 : 제작대랑 설명서 볼 때 UI -> 클라이언트의 UI여야하니까! 
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

void ASPPickup::TakePickup(ASPCharacterPlayer* Taker) //서버 
{
	if (!IsPendingKillPending()) //IsPendingKillPending()
	{
		if (ItemReference)
		{
			if (USPInventoryComponent* PlayerInvetory = Taker->GetInventory())
			{
				FTimerHandle TimerHandle;
				PlayerInvetory->HandleAddItem(ItemReference);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					//여기서 스포너한테 신호 보내야겠다.
					ASPPotionSpawner* PotionSpawner = Cast<ASPPotionSpawner>(GetOwner());
					if(PotionSpawner)
					{
						PotionSpawner->SpawnEvent();
					}
					this->SetOwner(MyPlayerOwner);
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

void ASPPickup::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PerformInteractionCheck();
	}
}

void ASPPickup::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->NoInteractableFound();
	}
}

void ASPPickup::ServerRPCInteract_Implementation(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget)
{
	SP_LOG(LogSPNetwork, Log, TEXT("ServerRPCInteract_Implementation"));
}

void ASPPickup::ClientRPCUpdateWidget_Implementation(ASPCharacterPlayer* Taker)
{
	SP_LOG(LogSPNetwork, Log, TEXT("ClientRPC"));
}
