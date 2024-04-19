// Fill out your copyright notice in the Description page of Project Settings.
#include "Potion/SPBlackPotion.h"
#include "Component/SPExplosionComponent.h"
#include <Kismet/GameplayStatics.h>

#include "SpectrumLog.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPBlackPotion::ASPBlackPotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/B_Potion/B_Potion.B_Potion'"));
	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackPotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		PotionMesh->SetRelativeScale3D(FVector(1.84f, 1.18f, 1.74f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	ExplosionComponent = CreateDefaultSubobject<USPExplosionComponent>(TEXT("ExplosionComponent"));
	this->SetReplicates(true);
	this->AActor::SetReplicateMovement(true);
	ExplosionComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ASPBlackPotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPBlackPotion::HandleActorHit);
}



void ASPBlackPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	//서버만 들어옴.. 
	// ServerRPCStopAiming();
	// MultiRPCExplosion();
	SP_LOG(LogSPNetwork,Log,TEXT("=========================================="));
	SP_LOG(LogSPNetwork,Log,TEXT("HandleActorHit!!! "));
	ExplosionComponent->Explode();
	this->SetLifeSpan(0.1f);
	
}

void ASPBlackPotion::MultiRPCExplosion_Implementation()
{
	SP_LOG(LogSPNetwork,Log,TEXT("=========================================="));
	SP_LOG(LogSPNetwork,Log,TEXT("HandleActorHit!!! "));
	ExplosionComponent->Explode();
	this->SetLifeSpan(0.1f);
}
void ASPBlackPotion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


