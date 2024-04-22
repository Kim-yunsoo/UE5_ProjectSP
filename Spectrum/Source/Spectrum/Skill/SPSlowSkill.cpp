// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSlowSkill.h"

#include "SpectrumLog.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

USPSlowSkill::USPSlowSkill()
{
	CoolDown=10;
}

void USPSlowSkill::SkillAction(AActor* MyOwner)
{
	Super::SkillAction(MyOwner);
	if (bIsActiveSlowSkill)
	{
		bIsActiveSlowSkill = false; //���ø�����Ʈ ó���� �ѹ� �ؾ� �Ѵ�. 
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
		if (Success) //�������� ���� �����ϸ� ������ �迭 ��� ��Ƽ�� �۾�
		{
			MultiRPCSkill(OutHits);
		}
	}
}

void USPSlowSkill::MultiRPCSkill_Implementation(const TArray<FHitResult>& OutHits)
{
	for (const FHitResult& HitResult : OutHits)
	{
		TArray<AActor*> ActorArray;
		AActor* HitPawn = HitResult.GetActor();
		FVector Location = HitPawn->GetActorLocation();
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		FVector Scale{1.0f, 1.0f, 1.0f};
		FTransform SpawnTransform{Rotation, Location, Scale};
		
		ACharacter* Character = Cast<ACharacter>(HitPawn);
		Character->GetCharacterMovement()->MaxWalkSpeed=100.f;
	}
}