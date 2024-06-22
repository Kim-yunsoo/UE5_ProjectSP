// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPickup.h"
#include "Component/SPInventoryComponent.h"
#include "Potion/SPItemBase.h"
#include "SPPotionSpawner.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPPickup::ASPPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	//PickupMesh->SetMobility(EComponentMobility::Static);
	//PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);


	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupTriggerComponent"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/ItemData/Item.Item'"));
	if (DataTableFinder.Succeeded())
	{
		ItemDataTable = DataTableFinder.Object;
	}

	PotionRange = 4;
	bIsSpectrumPotion = false;
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
	RowNames = ItemDataTable->GetRowNames(); //모든 데이터 이름 넣었음
	InitializePickup(USPItemBase::StaticClass(), 1);

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z = ActorLocation.Z + 45;
	Trigger->SetRelativeLocation(ActorLocation);
	Trigger->SetRelativeScale3D(FVector(1, 1, 2));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPPickup::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASPPickup::OnTriggerExit);
	
}

void ASPPickup::InitializePickup(const TSubclassOf<USPItemBase> BaseClass, const int32 InQuantity)
{
	if (HasAuthority())
	{
		if (RowNames.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 2);
			if (bIsSpectrumPotion) //스펙트럼 스폰 신호가 온다면? 
			{
				RandomIndex = RowNames.Find(FName(TEXT("S_Potion"))); // 있다면 인덱스를 반환한다.
				if (RandomIndex)
				{
					DesiredItemID = RowNames[RandomIndex];
				}
			}
			else
			{
				if (RandomIndex >= 0 && RandomIndex <= 2) //0~3의 범위
				{
					int32 Adjustment = FMath::RandRange(0, 1);
					if (Adjustment) //1이 나오면 확률 조정
					{
						//PotionRange
						RandomIndex += PotionRange;
						RandomIndex = FMath::Clamp(RandomIndex, 0, RowNames.Num() - 2);
					}
				}
				DesiredItemID = RowNames[RandomIndex];
			}
		}
	}
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<USPItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
		PickupMesh->SetMobility(EComponentMobility::Static);


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

bool ASPPickup::Interact(ASPCharacterPlayer* PlayerCharacter, USPHUDWidget* HUDWidget) //서버에서 호출된다. 
{
	if (PlayerCharacter)
	{
		MyPlayerOwner = PlayerCharacter;
		TakePickup(PlayerCharacter);
		return true;
	}
	return false;
}


void ASPPickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.Description;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void ASPPickup::TakePickup(ASPCharacterPlayer* Taker) //서버에서 호출된다. 
{
	if (!IsPendingKillPending()) //이 함수는 객체가 곧 삭제되는 상태인지 확인한다. 
	{
		if (ItemReference)
		{
			if (USPInventoryComponent* PlayerInvetory = Taker->GetInventory())
			{
				FTimerHandle TimerHandle;
				PlayerInvetory->HandleAddItem(ItemReference, 1); //이 함수로 인벤토리의 물량을 늘려준다. 

				
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					//여기서 스포너한테 신호 보내야겠다.
					ASPPotionSpawner* PotionSpawner = Cast<ASPPotionSpawner>(GetOwner());
					if (PotionSpawner)
					{
						OnItemPickedUp.Broadcast();
					}
					this->SetOwner(MyPlayerOwner);
					this->Destroy();
				}, 0.8f, false);
			}
		
		}
	}
}


void ASPPickup::OnTriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPCharacterPlayer* PlayerCharacter = Cast<ASPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PerformInteractionCheck(this);
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

void ASPPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPPickup, DesiredItemID);
}

