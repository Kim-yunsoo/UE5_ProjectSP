// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPOrangePotion.h"

#include "SPGlobalEnum.h"
#include "Component/SPOrangeExplosionComponent.h"


ASPOrangePotion::ASPOrangePotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/O_Potion/O_Potion.O_Potion'"));
	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrangePotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		PotionMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	OrangeExplosionComponent = CreateDefaultSubobject<USPOrangeExplosionComponent>(TEXT("ExplosionComponent"));
	MyColor = ColorType::Orange;

}

void ASPOrangePotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPOrangePotion::HandleActorHit);

	if(HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
		OrangeExplosionComponent->SetIsReplicated(true);
	}
}

void ASPOrangePotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{

	OrangeExplosionComponent->Explode(MyColor);
	this->SetLifeSpan(0.1f);
}
