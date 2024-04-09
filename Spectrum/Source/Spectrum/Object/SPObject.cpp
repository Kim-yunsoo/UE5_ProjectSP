// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Spectrum.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "GeometryCollection/GeometryCollection.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionComponent.h
#include "Kismet/KismetMathLibrary.h"
#include "GeometryCollection\GeometryCollectionComponent.h"

// Sets default values
ASPObject::ASPObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
	//bHasBeenCalled = false; 
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetCollisionProfileName(TEXT("PropCollision"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);
	bHasBeenCalled = true;
	ObjectInfo = new Protocol::PositionInfo();
	DestInfo = new Protocol::PositionInfo();
	bIsFrist = false;

}

ASPObject::~ASPObject()
{
	delete ObjectInfo;
	delete DestInfo;
	ObjectInfo = nullptr;
	DestInfo = nullptr;
}

// Called when the game starts or when spawned
void ASPObject::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp,Log,TEXT("MyActor's name is: %s, and its mesh is: %s"),*GetName(), *GetMesh()->GetName());
	//UE_LOG(LogTemp,Log,TEXT("MyActor's name is : %s, and its mesh is: %s"),*GetName(),*(ObjectMesh->GetStaticMesh()->GetName()));
	//UE_LOG(LogTemp,Log,TEXT("It was called from C++"));
	//GEngine->AddOnScreenDebugMessage(1,10,FColor::Blue,TEXT("It was called from C++"));
	RootComponent->SetMobility(EComponentMobility::Movable);
	ObjectLocation = GetActorLocation();
	//ObjectInfo->set_object_id(20);

	{// 
		ObjectInfo->set_x(ObjectLocation.X);
		ObjectInfo->set_y(ObjectLocation.Y);
		ObjectInfo->set_z(ObjectLocation.Z);
		ObjectInfo->set_is_aiming(false);
		ObjectInfo->set_is_jumping(false);
		ObjectInfo->set_is_holding(false);

		DestInfo->set_x(ObjectLocation.X);
		DestInfo->set_y(ObjectLocation.Y);
		DestInfo->set_z(ObjectLocation.Z);
	}
}

void ASPObject::OnExplosionHit(float Damage)
{
	if (bHasBeenCalled)
	{
		ObjectMesh->SetHiddenInGame(true);
		ObjectMesh->SetSimulatePhysics(true);
		ObjectMesh->SetCollisionProfileName(TEXT("DestructionCollision"));
		//FName ComponentName = FName(TEXT("GC_Cone"));
		UGeometryCollectionComponent* Geometry = NewObject<UGeometryCollectionComponent>(this, UGeometryCollectionComponent::StaticClass(), TEXT("GeometryComponent"));
		if (Geometry)
		{
			Geometry->SetupAttachment(RootComponent);
			Geometry->SetRestCollection(GeometryCollection);
			Geometry->SetCollisionProfileName(TEXT("DestructionCollision"));
			Geometry->RegisterComponent();
			Geometry->AddImpulse(FVector(0.0f, 0.0f, 125.f));
			FTimerHandle ChangeCollisionProfileTimer;
			float DelayInSeconds = 1.0f;
			GetWorld()->GetTimerManager().SetTimer(ChangeCollisionProfileTimer, [this, Geometry]() {
				Geometry->SetCollisionProfileName(TEXT("OnlyStaticCollision"));
				ObjectMesh->SetCollisionProfileName(TEXT("OnlyStaticCollision"));
				}, DelayInSeconds, false);
			this->SetLifeSpan(5.0f);
		}
		bHasBeenCalled = false;
	}
}
// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ?���? 보정
	//if(bIsMoving == true)	// ?��번이?��?�� ?��?��?��?���?
	{
		SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
		FVector Location(DestInfo->x(), DestInfo->y(), DestInfo->z());
		FVector TargetLocation = Location;// 목표 ?���?
		float InterpSpeed = 1.0f; // 보간 ?��?��

		// ?��?�� ?��치�?? 목표 ?���? ?��?���? 보간?��?�� �??��?��?�� ?��?��?�� 구현
		FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, DeltaTime * InterpSpeed);
		SetActorLocation(NewLocation);

		FRotator Rotation(0, DestInfo->yaw(), 0);
		SetActorRotation(Rotation);
	}

	static float DelayTime = 1.0;
	DelayTime -= DeltaTime;
	if (DelayTime > 0.0f)
	{
		return;
	}
	DelayTime = 1.0;
	bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(ObjectLocation, GetActorLocation(), 0.0);
	//UE_LOG(LogTemp, Log, TEXT("%s"), *GetActorLocation().ToString());
	if (Equal)
	{
		ObjectMesh->SetSimulatePhysics(false);
		ObjectLocation = GetActorLocation();
		ObjectInfo->set_is_holding(false);
	}
	else
	{
		ObjectLocation = GetActorLocation();
		ObjectInfo->set_x(ObjectLocation.X);
		ObjectInfo->set_y(ObjectLocation.Y);
		ObjectInfo->set_z(ObjectLocation.Z);
		ObjectInfo->set_yaw(GetActorRotation().Yaw);

	}

	MovePacketSendTimer -= DeltaTime;
	if (!Equal && MovePacketSendTimer <= 0&& bIsFrist == false)	
		// 0.1초마?�� 물건?�� ?��치�?? ?��르면 C_O_MOVE ?��?��	?��?��
		// ?���?�? ?��?��?�� ?���?
	{
		Protocol::C_O_MOVE MovePkt;
		{
			Protocol::PositionInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*ObjectInfo);
		}

		SEND_PACKET(MovePkt);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%lld %f %f %f"),
			ObjectInfo->object_id(), ObjectInfo->x(), ObjectInfo->y(), ObjectInfo->z()));
	}

	if (bIsFrist == true)
	{
		ObjectMesh->SetHiddenInGame(true);
		bHasBeenCalled = true;

		Protocol::C_O_BURST BurstPkt;
		{
			Protocol::BurstInfo* Info = BurstPkt.mutable_info();
			Info->set_object_id(ObjectInfo->object_id());
			Info->set_is_burst(true);
		}

		SEND_PACKET(BurstPkt);

	}
}


void ASPObject::SetPostionInfo(const Protocol::PositionInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	ObjectInfo->CopyFrom(Info);
	//bIsAiming = Info.is_aiming();
	//bIsJumping = Info.is_jumping();
	//bIsHolding = Info.is_holding();

	//FVector Location(Info.x(), Info.y(), Info.z());
	//SetActorLocation(Location);
	//FRotator Rotation(0, Info.yaw(), 0);
	//SetActorRotation(Rotation);

}

void ASPObject::SetDestInfo(const Protocol::PositionInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	DestInfo->CopyFrom(Info);
	
	//// Apply status right now
	//SetMoveState(Info.state());
}

void ASPObject::SetBurst(const bool burst)
{
	bHasBeenCalled = burst;
	bIsFrist = burst;
}
