// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillActor/SPSlowSkillActor.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "Data/SPSkillFVXStructs.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

struct FSkillFVXData;
// Sets default values
ASPSlowSkillActor::ASPSlowSkillActor()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> VFXDataRef(
		TEXT("/Script/Engine.DataTable'/Game/Spectrum/Data/DT_SkillFVXData.DT_SkillFVXData'"));
	if (VFXDataRef.Object)
	{
		VFXDataTable = VFXDataRef.Object;
	}
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// static ConstructorHelpers::FObjectFinder<UParticleSystem> GreenVfxRef(TEXT(
	// 	"/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Projectiles/CP_GreenProjectile.CP_GreenProjectile'"));
	// if (GreenVfxRef.Succeeded())
	// {
	// 	MainVFX->SetTemplate(GreenVfxRef.Object);
	// 	MainVFX->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	// }
	//
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> GreenHitRef(
	// 	TEXT("/Script/Engine.ParticleSystem'/Game/MagicProjectilesVol2/Particles/Hits/CP_GreenHit.CP_GreenHit'"));
	//
	// if (GreenHitRef.Succeeded())
	// {
	// 	EmitterHit = GreenHitRef.Object;
	// }

	// HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticle"));

	SetReplicates(true);
	bIsHoming = false;
}

void ASPSlowSkillActor::MultiRPCSlowSkillEffect_Implementation(const FVector& InHitLoction)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, InHitLoction, FRotator::ZeroRotator,
	                                         FVector(1.0f), true, EPSCPoolMethod::None, true);
}

void ASPSlowSkillActor::MultiRPCSlowSkill_Implementation(ASPCharacterPlayer* Player)
{
	ISPSkillInterface* CheckSlowAction = Cast<ISPSkillInterface>(Player);
	if (CheckSlowAction)
	{
		CheckSlowAction->HitSlowSkillResult();
	}
}

// Called when the game starts or when spawned
void ASPSlowSkillActor::BeginPlay()
{
	Super::BeginPlay();

	ASPCharacterPlayer* MyPlayer = Cast<ASPCharacterPlayer>(GetOwner());

	if (MyPlayer)
	{
		FString ScoolColor = UEnum::GetValueAsString(MyPlayer->SchoolAffiliation);

		FSkillFVXData* FvxData = VFXDataTable->FindRow<FSkillFVXData>(FName(*ScoolColor), "");
		if (FvxData)
		{
			MainVFX->SetTemplate((FvxData->AssetData.ProjectileFVX));
			MainVFX->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

			EmitterHit = FvxData->AssetData.HitFVX;
		}
	}

	BoxCollision->OnComponentHit.AddDynamic(this, &ASPSlowSkillActor::OnBoxCollisionHit);

	// if (TargetActor) // ???????? ????? ??? ??? ? 
	// {
	// 	RotateToTarget();
	// 	if (bIsHoming)
	// 	{
	// 		ProjectileMovement->bIsHomingProjectile = true;
	// 		ProjectileMovement->HomingAccelerationMagnitude= 2000.0f;
	// 		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	// 	}
	// }

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
		if(bIsOnce)
		{
			MultiRPCSlowSkillEffect(Hit.ImpactPoint);

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
						MultiRPCSlowSkill(Player);
					}
				}
			}

			this->SetActorHiddenInGame(true);
			this->SetLifeSpan(0.1f);
			bIsOnce=false;
			
		}
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


// void ASPSlowSkillActor::MultiRPCSlowSkill_Implementation(const FHitResult& Hit)
// {
// 	// ASPCharacterPlayer* palyer=Cast<ASPCharacterPlayer>(GetOwner());
// 	// if(palyer)
// 	// {
// 	// 	palyer->bIsDamage=true;
// 	// }
// 	FVector HitLocation = Hit.ImpactPoint;
// 	if(bIsOnce)
// 	{
// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
// 	                                         FVector(1.0f), true, EPSCPoolMethod::None, true);
// 		bIsOnce=false;
// 	}
// 	ISPSkillInterface* CheckSlowAction = Cast<ISPSkillInterface>(Hit.GetActor());
// 	if (CheckSlowAction)
// 	{
// 		CheckSlowAction->HitSlowSkillResult();
// 	}
// }
