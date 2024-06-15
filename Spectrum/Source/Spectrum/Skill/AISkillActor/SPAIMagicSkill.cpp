// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/AISkillActor/SPAIMagicSkill.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SPSkillInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

class ISPSkillInterface;
// Sets default values
ASPAIMagicSkill::ASPAIMagicSkill()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetBoxExtent(FVector(84, 31, 29));
	BoxCollision->SetCollisionProfileName(TEXT("PropCollision"));

	SetRootComponent(BoxCollision); 

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	


	MainVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainVFX"));
	MainVFX->SetupAttachment(BoxCollision);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VfxRef(TEXT(
		"/Script/Engine.ParticleSystem'/Game/Box/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile'"));
	if (VfxRef.Succeeded())
	{
		MainVFX->SetTemplate(VfxRef.Object);
		MainVFX->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));
		// MainVFX->SetRelativeRotation(FRotator(0.0f,0.0f,180.f));
		// MainVFX->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
		TEXT(
			"/Script/Engine.ParticleSystem'/Game/Box/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
	Speed = 1000.f;
	Gravity = 0.0f;
	bIsHoming = false;
	bDoOnce = true;
}

// Called when the game starts or when spawned
void ASPAIMagicSkill::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}


	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	BoxCollision->OnComponentHit.AddDynamic(this, &ASPAIMagicSkill::OnBoxCollisionHit);
	//BoxCollision->
	//GetWorld()->OverlapMultiByChannel()
	
	BoxCollision->IgnoreActorWhenMoving(GetOwner(), true); // 이것을 발사하는 캐릭터는 충돌이 안되도록 설정

	if (TargetActor) // 타겟해야할 액터를 찾은 경우 ? 
	{
		RotateToTarget();
		if (bIsHoming)
		{
			// ProjectileMovement->bIsHomingProjectile = true;
			// ProjectileMovement->HomingAccelerationMagnitude = 3000.0f;
			// ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
		}
	}
	SetLifeSpan(7.0f);
}

void ASPAIMagicSkill::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (bDoOnce)
		{
			MultiRPCMagicEffet(Hit.ImpactPoint);

			TArray<AActor*> OverlappedActors;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
			TArray<AActor*> ActorsToIgnore;
	
			bool Result = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation(),100.f,ObjectTypes,nullptr,ActorsToIgnore,OverlappedActors);
			if(Result)
			{
				for(AActor* Actor : OverlappedActors)
				{
					ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(Actor);
					if(Player)
					{
						MultiRPCMagicSlow(Player);
					}
				}
			}
			
			//ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(Hit.GetActor());
			this->SetActorHiddenInGame(true);
			this->SetLifeSpan(0.5f);
			bDoOnce = false;
		}
	}
}

void ASPAIMagicSkill::MultiRPCMagicEffet_Implementation(FVector HitLoction)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLoction, FRotator::ZeroRotator,
	                                         FVector(1.0f), true, EPSCPoolMethod::None, true);
	
}


void ASPAIMagicSkill::MultiRPCMagicSlow_Implementation(ASPCharacterPlayer* Player)
{
	 ISPSkillInterface* CheckIceAction = Cast<ISPSkillInterface>(Player);
	 if (CheckIceAction)
	 {
	 	CheckIceAction->HitSlowSkillResult();
	 }
}

void ASPAIMagicSkill::InitTarget(AActor* TargetPlayer)
{
	TargetActor = TargetPlayer;
	TargetLocation = TargetPlayer->GetActorLocation();
	bIsHoming = true;
}

void ASPAIMagicSkill::RotateToTarget()
{
	FVector MyTargetLocation{TargetLocation};
	FVector MyLocation = GetActorLocation();
	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(MyLocation, TargetActor->GetActorLocation());

	ProjectileMovement->Velocity = UnitVector * Speed;
}
