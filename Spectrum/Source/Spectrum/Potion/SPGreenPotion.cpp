// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPGreenPotion.h"

#include "SPGlobalEnum.h"
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


	//ExplosionComponent = CreateDefaultSubobject<USPGreenExplosionComponent>(TEXT("ExplosionComponent"));
	MyColor = ColorType::Green;

}

void ASPGreenPotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPGreenPotion::HandleActorHit);
	// if(HasAuthority())
	// {
	// 	this->SetReplicates(true);
	// 	this->AActor::SetReplicateMovement(true);
	// 	ExplosionComponent->SetIsReplicated(true);
	// }
}

void ASPGreenPotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

	ExplosionComponent->Explode(MyColor);
	this->SetLifeSpan(0.1f);
}
