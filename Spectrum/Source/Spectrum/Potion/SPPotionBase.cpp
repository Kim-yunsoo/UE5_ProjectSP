// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionBase.h"

#include "SpectrumLog.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/HUD.h"

// Sets default values
ASPPotionBase::ASPPotionBase()
{

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetAutoActivate(false);
	bHasExecutedOnce = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetMobility(EComponentMobility::Movable);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	PotionMesh->SetupAttachment(SphereComponent);
	PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ASPPotionBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASPPotionBase::Throw(const FVector& PotionDirection)
{
	if (false == bHasExecutedOnce)
	{
		this->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		MovementComponent->Velocity = PotionDirection;
		MovementComponent->Activate(false);
		SphereComponent->SetCollisionProfileName(TEXT("BlackItemCollision"), true);
		MoveTo();
		bHasExecutedOnce = true;
	}
}

void ASPPotionBase::MoveTo()
{
	FLatentActionInfo LatentInfo;
	FVector TargetRelativeLocation{ 0,0,0 };
	FRotator TargetRelativeRotation = UKismetMathLibrary::RandomRotator(false);
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(PotionMesh, TargetRelativeLocation, TargetRelativeRotation, false, false, 0.5, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ASPPotionBase::GetPotion()
{
	// APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// SP_LOG(LogSPNetwork, Log, TEXT("GetPotion"));
	// AHUD* PlayerHUD = PlayerController->GetHUD();
	// //USPInventoryWidget* InventoryWidget = Cast<USPInventoryWidget>(PlayerHUD->GetWidgetFromName(TEXT("Inventory")));
}

ASPPotionBase* ASPPotionBase::CreatePotionCopy()
{
	//인벤토리에 있는건 포인터를 통해서 관리될 예정
	//추가할 때 복사해야한다.
	ASPPotionBase* PotionCopy = NewObject<ASPPotionBase>(StaticClass());

	PotionCopy->ID = this->ID;
	PotionCopy->Quantity = this->Quantity;
	PotionCopy->ItemType = this->ItemType;
	PotionCopy->ItemTextData = this->ItemTextData;
	PotionCopy->ItemNumericData = this->ItemNumericData;
	PotionCopy->ItemAssetData = this->ItemAssetData;

	return PotionCopy;
}

void ASPPotionBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
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

void ASPPotionBase::Use(ASPCharacterPlayer* Character)
{
}


