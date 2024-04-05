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
	bHasBeenCalled = false; // 한번만 실행하기 위한 변수
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);

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

	{// 처음 위치를 설정해줌
		//ObjectInfo->set_x(ObjectLocation.X);
		//ObjectInfo->set_y(ObjectLocation.Y);
		//ObjectInfo->set_z(ObjectLocation.Z);
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
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->SetLifeSpan(1.0f);
		bHasBeenCalled = true;
	}
}
// Called every frame
void ASPObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static float DelayTime = 1.0;
	DelayTime -= DeltaTime;
	if (DelayTime > 0.0f)
	{
		return;
	}
	DelayTime = 1.0;
	bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(ObjectLocation, GetActorLocation(), 0.0);
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


		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%lld %f %f %f"),
		//	ObjectInfo->object_id(), ObjectInfo->x(), ObjectInfo->y(), ObjectInfo->z()));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%lld"), ObjectInfo->object_id()));


	if (ObjectInfo->is_holding()) {

		Protocol::C_O_MOVE MovePkt;
		{
			Protocol::PositionInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*ObjectInfo);
		}

		SEND_PACKET(MovePkt);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%lld %f %f %f"),
			ObjectInfo->object_id(), ObjectInfo->x(), ObjectInfo->y(), ObjectInfo->z()));

	}
		
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%lld %f %f %f"),
						ObjectInfo->object_id(), ObjectInfo->x(), ObjectInfo->y(), ObjectInfo->z()));
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

	FVector Location(Info.x(), Info.y(), Info.z());
	SetActorLocation(Location);
}

void ASPObject::SetDestInfo(const Protocol::PositionInfo& Info)
{
	//if (PlayerInfo->object_id() != 0)
	//{
	//	assert(PlayerInfo->object_id() == Info.object_id());
	//}

	//// Dest에 최종 상태 복사
	//DestInfo->CopyFrom(Info);
	//bIsAiming = Info.is_aiming();
	////bIsJumping = Info.is_jumping();
	//bIsHolding = Info.is_holding();

	//if (IsMyPlayer() == false && Info.is_jumping() == true) {
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Jump signal 2")));
	//	SetJumping();
	//}
	//else if (IsMyPlayer() == false && Info.is_jumping() == false) {
	//	ResetJumping();
	//}

	//// 상태만 바로 적용!
	//SetMoveState(Info.state());
}
