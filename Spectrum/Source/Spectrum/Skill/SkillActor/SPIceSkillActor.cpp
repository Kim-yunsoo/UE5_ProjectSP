// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPIceSkillActor.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
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
		MainVFX->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.f));
		MainVFX->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
		TEXT(
			"/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Muzzles/P_Muzzle_IceSpike01_Blue.P_Muzzle_IceSpike01_Blue'"));

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
		if(bIsOnce)
		{
			MultiRPCIceSkillEffect(Hit.ImpactPoint);

			TArray<AActor*> OverlappedActors;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
			TArray<AActor*> ActorsToIgnore;

			bool Result = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 100.f, ObjectTypes,
			                                                        nullptr, ActorsToIgnore, OverlappedActors);
			//DrawDebugSphere(GetWorld(), GetActorLocation(), 100, 32, FColor::Green, false, 5.0f);
			if (Result)
			{
				for (AActor* Actor : OverlappedActors)
				{
					ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(Actor);
					if (Player)
					{
						MultiRPCIceSkill(Player);
					}
				}
			}

			this->SetActorHiddenInGame(true);
			this->SetLifeSpan(0.1f);
			bIsOnce=false;
			
		}
	}
}

void ASPIceSkillActor::MultiRPCIceSkill_Implementation(ASPCharacterPlayer* Player)
{
	ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Player);
	if (CheckIceAction)
	{
		CheckIceAction->HitIceSkillResult();
	}
}

void ASPIceSkillActor::MultiRPCIceSkillEffect_Implementation(const FVector& InHitLoction)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, InHitLoction, FRotator::ZeroRotator,
	                                         FVector(1.0f), true, EPSCPoolMethod::None, true);

}
