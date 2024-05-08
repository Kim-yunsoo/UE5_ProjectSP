// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

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

	EmitterHit = CreateDefaultSubobject<UParticleSystem>(TEXT("EmitterHit"));

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



