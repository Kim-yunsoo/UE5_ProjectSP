// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPSlowSkillActor.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterMovementComponent.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPSlowSkillActor::ASPSlowSkillActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Log, TEXT("NO Parm"));




	static ConstructorHelpers::FObjectFinder<UParticleSystem> GreenVfxRef(TEXT(
		"/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/CP_GreenProjectile.CP_GreenProjectile'"));
	if (GreenVfxRef.Succeeded())
	{
		MainVFX->SetTemplate(GreenVfxRef.Object);
		MainVFX->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> GreenHitRef(
		TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_GreenHit.CP_GreenHit'"));

	if (GreenHitRef.Succeeded())
	{
		EmitterHit = GreenHitRef.Object;
	}

	// HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticle"));

	bIsHoming = false;
}


void ASPSlowSkillActor::ServerRPCSlowSkill_Implementation(const FHitResult& Hit)
{
}

// Called when the game starts or when spawned
void ASPSlowSkillActor::BeginPlay()
{
	Super::BeginPlay();

	
	//BoxCollision->IgnoreActorWhenMoving(GetOwner(),true);

	BoxCollision->OnComponentHit.AddDynamic(this, &ASPSlowSkillActor::OnBoxCollisionHit);

	if (TargetActor) // 타겟해야할 액터를 찾은 경우 ? 
	{
		RotateToTarget();
		if (bIsHoming)
		{
			ProjectileMovement->bIsHomingProjectile = true;
			ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
		}
	}

	if (HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}
}

void ASPSlowSkillActor::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ServerRPCSlowSkill(Hit);
	if (HasAuthority())
	{
		MultiRPCSlowSkill(Hit);
		
		this->SetActorHiddenInGame(true);
		this->SetLifeSpan(0.1f);
	}
}

void ASPSlowSkillActor::RotateToTarget()
{
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(MyLocation, TargetLocation);

	ProjectileMovement->Velocity = UnitVector * Speed;
}

void ASPSlowSkillActor::InitTarget(AActor* TargetPlayer)
{
	TargetActor = TargetPlayer;
	bIsHoming = true;
}

void ASPSlowSkillActor::MultiRPCSlowSkill_Implementation(const FHitResult& Hit)
{
	FVector HitLocation = Hit.ImpactPoint;
	if(bIsOnce)
	{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
	                                         FVector(1.0f), true, EPSCPoolMethod::None, true);
		bIsOnce=false;
	}
	ISPSkillInterface* CheckSlowAction = Cast<ISPSkillInterface>(Hit.GetActor());
	if (CheckSlowAction)
	{
		CheckSlowAction->HitSlowSkillResult();
	}
}
