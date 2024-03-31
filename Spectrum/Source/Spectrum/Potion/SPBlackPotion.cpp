// Fill out your copyright notice in the Description page of Project Settings.
#include "Potion/SPBlackPotion.h"
#include "Component/SPExplosionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>



// Sets default values
ASPBlackPotion::ASPBlackPotion()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(35.460735f);
	SphereComponent->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Potion/B_Potion/B_Potion.B_Potion'"));
	if (SphereMeshRef.Object)
	{
		BlackPotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackPotionMesh"));
		BlackPotionMesh->SetStaticMesh(SphereMeshRef.Object);
		BlackPotionMesh->SetupAttachment(SphereComponent);
		BlackPotionMesh->SetRelativeLocation(FVector(2.982332, 0.746419, -20.102119));
		BlackPotionMesh->SetRelativeScale3D(FVector(1.840461, 1.184211, 1.746711));
	}

	ExplosionComponent = CreateDefaultSubobject<USPExplosionComponent>(TEXT("ExplosionComponent"));
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	bHasExecutedOnce = false;
}

// Called when the game starts or when spawned
void ASPBlackPotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPBlackPotion::HandleActorHit);
}



void ASPBlackPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosionComponent->Explode();
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, Hit.Component->K2_GetComponentLocation(), 100.0f, nullptr, TArray<AActor*>(), this, nullptr, true, ECollisionChannel::ECC_Pawn);
	BlackPotionMesh->SetVisibility(false);
	this->SetLifeSpan(1.0f);
	
}

void ASPBlackPotion::Throw(const FVector& PotionDirection)
{
	if (false == bHasExecutedOnce)
	{
		this->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		MovementComponent->Velocity = PotionDirection;
		MovementComponent->Activate(false);
		SphereComponent->SetCollisionProfileName(TEXT("BlackItemCollision"), true);
		MoveTo();
		bHasExecutedOnce= true;
	}
}

void ASPBlackPotion::MoveTo()
{
	FLatentActionInfo LatentInfo;
	FVector TargetRelativeLocation{ 0,0,0 };
	FRotator TargetRelativeRotation = UKismetMathLibrary::RandomRotator(false);
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(BlackPotionMesh, TargetRelativeLocation, TargetRelativeRotation, false, false, 0.5, false, EMoveComponentAction::Type::Move, LatentInfo);
}

