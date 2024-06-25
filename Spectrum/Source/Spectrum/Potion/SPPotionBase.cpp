// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPotionBase.h"

#include "SpectrumLog.h"
#include "Component/SPExplosionComponent.h"
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
	ExplosionComponent = CreateDefaultSubobject<USPExplosionComponent>(TEXT("ExplosionComponent"));

	//EmitterHit = CreateDefaultSubobject<UParticleSystem>(TEXT("EmitterHit"));
	//BaseDamage=10.0f; 
}

// Called when the game starts or when spawned
void ASPPotionBase::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
		ExplosionComponent->SetIsReplicated(true);
	}
}



void ASPPotionBase::Throw(const FVector& PotionDirection)
{
	SP_LOG(LogTemp,Log,TEXT("Throw"));
	if (false == bHasExecutedOnce)
	{
		this->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		MovementComponent->Velocity = PotionDirection;
		MovementComponent->Activate(false);
		//SphereComponent->SetCollisionProfileName(TEXT("BlackItemCollision"), true);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASPPotionBase::ChangeCollisionProfile,0.1f, false);
		MoveTo();
		bHasExecutedOnce = true;
	}
}
void ASPPotionBase::ChangeCollisionProfile()
{
	SphereComponent->SetCollisionProfileName(TEXT("BlackItemCollision"), true);
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
