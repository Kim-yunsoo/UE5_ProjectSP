// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
//#include "GeometryCollection/GeometryCollection.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionComponent.h
#include "SpectrumLog.h"
#include "SPGlobalEnum.h"
#include "GeometryCollection\GeometryCollectionComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPObject::ASPObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	// PrimaryActorTick.bCanEverTick = true;
	MyColorType = ColorType::None;
	//bHasBeenCalled = false; 
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetCollisionProfileName(TEXT("PropCollision"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);
	ObjectMesh->SetUseCCD(true);
	ObjectMesh->SetRenderCustomDepth(true);
	SetRootComponent(ObjectMesh);
	//ObjectMesh->SetIsReplicated(true);
	//this->SetReplicates(true);
	// ObjectMesh->SetIsReplicated(true);

	bHasBeenCalled = true;
	bIsScoreReflected = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> GreenDataRef(
		TEXT("/Game/Spectrum/ColorData/DT_Green.DT_Green"));
	if (GreenDataRef.Object)
	{
		GreenData = GreenDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> OrangeDataRef(
		TEXT("/Game/Spectrum/ColorData/DT_Orange.DT_Orange"));
	if (OrangeDataRef.Object)
	{
		OrangeData = OrangeDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> PurpleDataRef(
		TEXT("/Game/Spectrum/ColorData/DT_Purple.DT_Purple"));
	if (PurpleDataRef.Object)
	{
		PurpleData = PurpleDataRef.Object;
	}

	bReplicates = true;
}

ASPObject::~ASPObject()
{
}

// Called when the game starts or when spawned
void ASPObject::BeginPlay()
{
	Super::BeginPlay();
	RootComponent->SetMobility(EComponentMobility::Movable);
	ObjectLocation = GetActorLocation();
	
	OriginMaterial = ObjectMesh->GetMaterial(ElementIndex); 
	ObjectDynamic = ObjectMesh->CreateDynamicMaterialInstance(ElementIndex, nullptr, FName(TEXT("None")));
	
	ChaosDynamic = UMaterialInstanceDynamic::Create(OriginMaterial, nullptr, NAME_None);
	
	if (HasAuthority())
	{
		this->AActor::SetReplicateMovement(true);
	}
}

void ASPObject::OnChangeColorBlack()
{
	if (bHasBeenCalled)
	{
		ObjectMesh->SetHiddenInGame(true);
		ObjectMesh->SetSimulatePhysics(true);
		ObjectMesh->SetCollisionProfileName(TEXT("OnlyStaticCollision"));
		
		UGeometryCollectionComponent* Geometry = NewObject<UGeometryCollectionComponent>(
			this, UGeometryCollectionComponent::StaticClass(), TEXT("GeometryComponent"));
		if (Geometry)
		{
			Geometry->SetupAttachment(RootComponent);
			Geometry->SetRestCollection(GeometryCollection);
			Geometry->SetCollisionProfileName(TEXT("DestructionCollision"));
			Geometry->SetMaterial(ElementIndex, ChaosDynamic);
		
			Geometry->RegisterComponent();
			Geometry->AddImpulse(FVector(0.0f, 0.0f, 125.f));
			
			this->SetLifeSpan(5.0f);
		}
		bHasBeenCalled = false;
	}
}


// Called every frame
// void ASPObject::Tick(float DeltaTime)
// {
// 	// Super::Tick(DeltaTime);
// 	//
// 	// static float DelayTime = 1.0;
// 	// DelayTime -= DeltaTime;
// 	// if (DelayTime > 0.0f)
// 	// {
// 	// 	return;
// 	// }
// 	// DelayTime = 1.0;
// 	// bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(ObjectLocation, GetActorLocation(), 0.0);
// 	// if (Equal)
// 	// {
// 	// 	ObjectMesh->SetSimulatePhysics(false);
// 	// 	ObjectLocation = GetActorLocation();
// 	// }
// 	// else
// 	// {
// 	// 	ObjectLocation = GetActorLocation();
// 	// }
// }


void ASPObject::MultiRPCExplosionHit_Implementation()
{
}

void ASPObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPObject, bIsScoreReflected);
}

void ASPObject::SetDynamicColor(const UDataTable* Table)
{
	FColorData* ColorRow = Table->FindRow<FColorData>(RowName, "");
	MyColorType = ColorRow->Color;

	ObjectDynamic->SetVectorParameterValue(DynamicParam, ColorRow->ColorData);
	ChaosDynamic->SetVectorParameterValue(DynamicParam, ColorRow->ColorData);
}

void ASPObject::SetObjectCollisionType(const FName& CollisionType)
{
	ObjectMesh->SetCollisionProfileName(CollisionType);
}


// Called every frame

void ASPObject::OnChangeColorGreen()
{
	if (ObjectDynamic)
	{
		SetDynamicColor(GreenData);
	}
}

void ASPObject::OnChangeColorOrange()
{
	if (ObjectDynamic)
	{
		SetDynamicColor(OrangeData);
	}
}

void ASPObject::OnChangeColorPurple()
{
	if (ObjectDynamic)
	{
		SetDynamicColor(PurpleData);
	}
}

