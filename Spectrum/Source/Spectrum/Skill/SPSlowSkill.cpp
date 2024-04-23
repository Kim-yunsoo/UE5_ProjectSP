// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSlowSkill.h"

#include "SpectrumLog.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

USPSlowSkill::USPSlowSkill()
{
	CoolDown = 10;
	this->SetIsReplicated(true);
}

void USPSlowSkill::BeginPlay()
{
	Super::BeginPlay();
}


void USPSlowSkill::SkillAction(AActor* MyOwner)
{
	Super::SkillAction(MyOwner);

	FVector TracePointStart = Owner->GetActorLocation();
	FVector TracePointEnd = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100;
	float Radius = 20.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	TArray<FHitResult> OutHits;
	FLinearColor GreenColor(0.0f, 1.0f, 0.0f);
	FLinearColor RedColor(1.0f, 0.0f, 0.0f);
	float DrawTime = 3.0f;
	bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TracePointStart, TracePointEnd,
	                                                                Radius, ObjectTypes, false, ActorsToIgnore,
	                                                                EDrawDebugTrace::ForDuration, OutHits, true,
	                                                                GreenColor, RedColor, DrawTime);
	if (Success) //서버에서 판정 성공하면 판정된 배열 모두 멀티로 작업
	{
		SP_SUBLOG(LogSPNetwork,Log,TEXT("USPSlowSkill")); 
		MultiRPCSkill(OutHits);
	}
	
}

void USPSlowSkill::MultiRPCSkill_Implementation(const TArray<FHitResult>& OutHits)
{
	SP_SUBLOG(LogSPNetwork,Log,TEXT("USPSlowSkill")); //.. 왜 서버에서 호출하면 서버만 부르는건지 ..? 
	for (const FHitResult& HitResult : OutHits)
	{
		TArray<AActor*> ActorArray;
		AActor* HitPawn = HitResult.GetActor();
		FVector Location = HitPawn->GetActorLocation();
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		FVector Scale{1.0f, 1.0f, 1.0f};
		FTransform SpawnTransform{Rotation, Location, Scale};
		ACharacter* HitCharacter = Cast<ACharacter>(HitPawn);
		HitCharacter->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

// void USPSlowSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// 	// DOREPLIFETIME_CONDITION(USPSlowSkill, bReplicates, COND_None);
// 	// DOREPLIFETIME(USPSlowSkill,this);
// 	// DOREPLIFETIME(USPSlowSkill, this);
// }
