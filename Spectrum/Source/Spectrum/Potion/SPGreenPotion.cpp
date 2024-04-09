// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPGreenPotion.h"
#include "Component/SPExplosionComponent.h"


ASPGreenPotion::ASPGreenPotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/G_Potion/G_Potion1.G_Potion1'"));

	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GreenPotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		PotionMesh->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	ExplosionComponent = CreateDefaultSubobject<USPExplosionComponent>(TEXT("ExplosionComponent"));

}

void ASPGreenPotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPGreenPotion::HandleActorHit);
}

void ASPGreenPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ExplosionComponent->Explode();
	this->SetLifeSpan(0.1f);
}
