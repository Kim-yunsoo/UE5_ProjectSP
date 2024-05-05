// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPSpectrumPotion.h"

ASPSpectrumPotion::ASPSpectrumPotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/S_Potion/Spectruem.Spectruem'"));
	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpectrumPotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		//PotionMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void ASPSpectrumPotion::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &ASPSpectrumPotion::HandleActorHit);

	if(HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
		//OrangeExplosionComponent->SetIsReplicated(true);
	}
}

void ASPSpectrumPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	this->SetLifeSpan(0.1f);
}
