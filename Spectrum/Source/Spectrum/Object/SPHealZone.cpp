// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPHealZone.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASPHealZone::ASPHealZone()
{
	HealEffect= CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MainVFX"));
	SetRootComponent(HealEffect);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> HealEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Box/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura'"));
	if(HealEffectRef.Succeeded())
	{
		HealEffect->SetTemplate(HealEffectRef.Object);
	}
}

// Called when the game starts or when spawned
void ASPHealZone::BeginPlay()
{
	Super::BeginPlay();

	float HealTimeDuration = 3.0f;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASPHealZone::SpawnHealSphere,
	HealTimeDuration, true);
	
	//UKismetSystemLibrary::SphereOverlapActors(const UObject* WorldContextObject, const FVector SpherePos, float SphereRadius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
	//float Radius = 300.0f;
	//UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation(),Radius, );
	
	//GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASPGameState::DefaultGameTimer,
	//GetWorldSettings()->GetEffectiveTimeDilation(), true);

	
	
}

void ASPHealZone::SpawnHealSphere()
{
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	
	bool Result = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation(),300.f,ObjectTypes,nullptr,ActorsToIgnore,OverlappedActors);
// 디버그 
	UWorld* World = GetWorld();
	if (World)
	{
		FColor SphereColor = Result ? FColor::Green : FColor::Red;
		DrawDebugSphere(
			World,
			GetActorLocation(),
			300.f,
			12,  // 구체의 세그먼트 수
			SphereColor,
			false,  // 지속 시간 (true면 지속적으로 그려짐)
			1.0f,  // 5초 동안 디버그 구체를 그립니다.
			0,     // 두께
			2.0f   // 라인 두께
		);
	}
	//
	

}
