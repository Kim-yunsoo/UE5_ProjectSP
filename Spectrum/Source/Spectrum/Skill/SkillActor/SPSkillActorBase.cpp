// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPSkillActorBase.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPSkillActorBase::ASPSkillActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetBoxExtent(FVector(84, 31, 29));
	BoxCollision->SetCollisionProfileName(TEXT("PropCollision"));
	// BoxCollision->IgnoreActorWhenMoving(this,true);
	// BoxCollision->ignore
	SetRootComponent(BoxCollision); //루트 컴포넌트로 만들기

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	MainVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainVFX"));
	MainVFX->SetupAttachment(BoxCollision);
	Speed = 1500.f;
	Gravity = 0.0f;
	// HitParticle->SetIsReplicated(true);
	// BoxCollision->SetIsReplicated(true);
	bIsOnce = true;

}

// Called when the game starts or when spawned
void ASPSkillActorBase::BeginPlay()
{
	Super::BeginPlay();

	this->SetLifeSpan(10.f); // 10초 뒤 사라지도록 함 
	BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->HomingAccelerationMagnitude = 7000.f;
	
}

void ASPSkillActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPSkillActorBase,MainVFX);
	DOREPLIFETIME(ASPSkillActorBase,EmitterHit);
}

