// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/AISkillActor/SPAIMagicSkill.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPAIMagicSkill::ASPAIMagicSkill()
{

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollision->SetBoxExtent(FVector(84, 31, 29));
	BoxCollision->SetCollisionProfileName(TEXT("PropCollision"));

	SetRootComponent(BoxCollision); //루트 컴포넌트로 만들기

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	
	MainVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainVFX"));
	MainVFX->SetupAttachment(BoxCollision);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VfxRef(TEXT("/Script/Engine.ParticleSystem'/Game/Box/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile'"));
	if (VfxRef.Succeeded())
	{
		MainVFX->SetTemplate(VfxRef.Object);
		MainVFX->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));
		// MainVFX->SetRelativeRotation(FRotator(0.0f,0.0f,180.f));
		// MainVFX->SetRelativeScale3D(FVector(0.5f,0.5f,0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HitRef(
	TEXT("/Script/Engine.ParticleSystem'/Game/Box/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld'"));

	if (HitRef.Succeeded())
	{
		EmitterHit = HitRef.Object;
	}
	

	Speed = 1500.f;
	Gravity = 0.0f;
	
}

// Called when the game starts or when spawned
void ASPAIMagicSkill::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	BoxCollision->OnComponentHit.AddDynamic(this, &ASPAIMagicSkill::OnBoxCollisionHit);

	BoxCollision->IgnoreActorWhenMoving(GetOwner(),true); // 이것을 발사하는 캐릭터는 충돌이 안되도록 설정

	
}

void ASPAIMagicSkill::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitLocation = Hit.ImpactPoint;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterHit, HitLocation, FRotator::ZeroRotator,
											 FVector(1.0f), true, EPSCPoolMethod::None, true);
	Destroy();
}


