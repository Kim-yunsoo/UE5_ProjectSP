// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Spectrum.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASPObject::ASPObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
	bHasBeenCalled = false; // �ѹ��� �����ϱ� ���� ����
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetCollisionProfileName(TEXT("PropCollision"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);
	RootComponent = ObjectMesh;
	ObjectInfo = new Protocol::PositionInfo();
	DestInfo = new Protocol::PositionInfo();

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
	if (false == bHasBeenCalled)
	{
		ObjectMesh->SetHiddenInGame(true);
		//ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//this->SetLifeSpan(1.0f);
		bHasBeenCalled = true;
	}
}
// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 위치 보정
	//if(bIsMoving == true)	// 한번이라도 이동했으면
	{
		SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
		FVector Location(DestInfo->x(), DestInfo->y(), DestInfo->z());
		FVector TargetLocation = Location;// 목표 위치
		float InterpSpeed = 1.0f; // 보간 속도

		// 현재 위치와 목표 위치 사이를 보간하여 부드러운 이동을 구현
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
	if (!Equal && MovePacketSendTimer <= 0)	// 0.1초마다 물건의 위치가 다르면 C_O_MOVE 패킷	전송
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

	//else
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%lld %f %f %f"),
	//					ObjectInfo->object_id(), ObjectInfo->x(), ObjectInfo->y(), ObjectInfo->z()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%f %f %f"),
	//	GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	//UE_LOG(LogTemp, Log, TEXT("%s"), *ObjectLocation.ToString());

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
