// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/SPPurplePotion.h"

#include "SPGlobalEnum.h"
#include "Component/SPExplosionComponent.h"


ASPPurplePotion::ASPPurplePotion()
{
	SphereComponent->SetSphereRadius(35.46f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Spectrum/Assets/Potion/P_Potion/P_Potion1.P_Potion1'"));
	if (PotionMeshRef.Object)
	{
		PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrangePotionMesh"));
		PotionMesh->SetStaticMesh(PotionMeshRef.Object);
		PotionMesh->SetupAttachment(SphereComponent);
		PotionMesh->SetRelativeLocation(FVector(2.98f, 0.74f, -20.10f));
		PotionMesh->SetRelativeScale3D(FVector(0.023f, 0.023f, 0.023f));
		PotionMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	//PurpleExplosionComponent = CreateDefaultSubobject<USPPurpleExplosionComponent>(TEXT("ExplosionComponent"));
	// this->SetReplicates(true);
	// this->AActor::SetReplicateMovement(true);
	// PurpleExplosionComponent->SetIsReplicated(true);

	MyColor = ColorType::Purple;
}

void ASPPurplePotion::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASPPurplePotion::HandleActorHit);

	// if(HasAuthority())
	// {
	// 	this->SetReplicates(true);
	// 	this->AActor::SetReplicateMovement(true);
	// 	ExplosionComponent->SetIsReplicated(true);
	// }
}

void ASPPurplePotion::HandleActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	ExplosionComponent->Explode(MyColor);
	this->SetLifeSpan(0.1f);
}
