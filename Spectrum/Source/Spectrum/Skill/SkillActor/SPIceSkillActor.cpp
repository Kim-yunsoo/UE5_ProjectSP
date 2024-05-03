// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPIceSkillActor.h"

#include "Components/BoxComponent.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


class ISPSkillInterface;

ASPIceSkillActor::ASPIceSkillActor()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VfxRef(TEXT(
	"/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/P_Projectile_IceSpike01_Blue.P_Projectile_IceSpike01_Blue'"));
	if (VfxRef.Succeeded())
	{
		MainVFX->SetTemplate(VfxRef.Object);
		MainVFX->SetRelativeLocation(FVector(-36.0f, 0.0f, 0.0f));
		MainVFX->SetRelativeRotation(FRotator(0.0f,0.0f,180.f));
		MainVFX->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
		TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Muzzles/P_Muzzle_IceSpike01_Blue.P_Muzzle_IceSpike01_Blue'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}

}

void ASPIceSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentHit.AddDynamic(this, &ASPIceSkillActor::OnBoxCollisionHit);
	if (HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}
}

void ASPIceSkillActor::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		MultiRPCIceSkill(Hit);
		this->SetActorHiddenInGame(true);
		this->SetLifeSpan(0.1f);
	}
}

void ASPIceSkillActor::MultiRPCIceSkill_Implementation(const FHitResult& Hit)
{
	FVector HitLocation = Hit.ImpactPoint;
	if(bIsOnce)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
												 FVector(1.0f), true, EPSCPoolMethod::None, true);
		bIsOnce=false;
	}
	
	ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Hit.GetActor());
	if (CheckIceAction)
	{
		CheckIceAction->HitIceSkillResult();
	}
}
