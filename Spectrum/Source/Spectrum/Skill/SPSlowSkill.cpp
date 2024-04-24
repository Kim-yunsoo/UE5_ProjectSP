// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SPSlowSkill.h"

#include "SpectrumLog.h"
#include "Character/SPCharacterPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

USPSlowSkill::USPSlowSkill()
{
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	UActorComponent::SetComponentTickEnabled(true);
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled	= true;
	// PrimaryActorTick.bCanEverTick = true;
	bAutoActivate = true;
	CoolDown = 10;
	// this->SetIsReplicated(true); // ..?
	bIsActiveSlowSkill=true;
}

void USPSlowSkill::BeginPlay()
{
	Super::BeginPlay();
	// BoxCollision->OnComponentHit.AddDynamic(this,&ASPSkillBase::OnBoxCollisionHit);

	// if(HasAuthority())
	// {
	// 	this->SetReplicates(true);
	// 	this->AActor::SetReplicateMovement(true);
	// 	// ExplosionComponent->SetIsReplicated(true);
	// }
}

void USPSlowSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime,  TickType,ThisTickFunction);

	if (!Owner)
	{
		return;
	}
	
	if (bIsActiveSlowSkill) //참이라면? 
	{
		UE_LOG(LogTemp, Log, TEXT("return"));
		return;
	}

	FGameTime CurrentGameTime = GetWorld()->GetTime();
	if (CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolDown)
	{
		float ElapsedTime = CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp;
		UE_LOG(LogTemp, Log, TEXT("%f"), ElapsedTime);
	}
	else
	{
		bIsActiveSlowSkill = true;
	}
}


// //
// void USPSlowSkill::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	if(!Owner)
// 	{
// 		return;
// 	}
// 	
// 	if(bIsActiveSlowSkill) //참이라면? 
// 	{
// 		UE_LOG(LogTemp,Log,TEXT("return"));
// 		return;
// 	}
// 	
// 	FGameTime CurrentGameTime = GetWorld()->GetTime();
// 	if(CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolDown)
// 	{
// 		float ElapsedTime =CurrentGameTime.GetWorldTimeSeconds()-ActivetedTimeStamp;
// 		UE_LOG(LogTemp,Log,TEXT("%f"),ElapsedTime);
// 		// bIsActiveSlowSkill=true;
// 		// Owner->SetIsActiveSlowSkill(true);
// 	}
// 	else
// 	{
// 		bIsActiveSlowSkill=true;
// 	}
// }

void USPSlowSkill::SkillAction(ASPCharacterPlayer* MyOwner)
{
	// OnSkillCool.Broadcast();
	UE_LOG(LogTemp, Log, TEXT("SkillAction"));
	

	GameTime = GetWorld()->GetTime();
	ActivetedTimeStamp = GameTime.GetWorldTimeSeconds();

	Super::SkillAction(MyOwner);

	FVector TracePointStart = Owner->GetActorLocation();
	//this->GetOuter()
	FVector TracePointEnd = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 4000;
	float Radius = 100.f;
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
	
	//
	// if(OutHits.Num()>0)
	// {
	// 	TargetActor=OutHits[0].GetActor();
	// 	RotateToTarget();
	// }
	// FTimerHandle Handle;
	// GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
	// 										   {
	// 	UE_LOG(LogTemp,Log,TEXT("Ready"));
	// 			bIsActiveSlowSkill=true;
	// 										   }
	// 									   ), 5, false, 0.0f);


	if (Success) //서버에서 판정 성공하면 판정된 배열 모두 멀티로 작업
	{
		SP_SUBLOG(LogSPNetwork, Log, TEXT("USPSlowSkill"));
		MultiRPCSkill(OutHits);
	}
}

void USPSlowSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USPSlowSkill, bIsActiveSlowSkill);
}

// void USPSlowSkill::OnBoxCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, //서버 RPC를 걸어줄 필요가 있어보인다. 
//                                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	
// 	UE_LOG(LogTemp,Log,TEXT("HIT!!!!"));
// 	ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(Hit.GetActor());
// 	if(Player)
// 	{
// 		MultiRPCSkill(Player);
// 	}
// 	//Hit.GetActor()->GetActorLocation()
// 	FVector HitLocation = Hit.GetActor()->GetActorLocation();
// 	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),EmitterHit,HitLocation,FRotator::ZeroRotator,
// 		FVector(1.0f),true,EPSCPoolMethod::None, true);
// 	this->DestroyComponent();
// }


// void USPSlowSkill::ServerRPC_Implementation( const TArray<FHitResult>& OutHits)
// {
// 	MultiRPCSkill(OutHits);
// }

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
		ASPCharacterPlayer* HitCharacter = Cast<ASPCharacterPlayer>(HitResult.GetActor());
		if (HitCharacter)
		{
			HitCharacter->SlowAction();
		}
	}
}

