// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Spectrum.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "GeometryCollection/GeometryCollection.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionComponent.h
#include "SpectrumLog.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "GeometryCollection\GeometryCollectionComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPObject::ASPObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
	MyColorType = ColorType::None;
	//bHasBeenCalled = false; 
	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetCollisionProfileName(TEXT("PropCollision"));
	ObjectMesh->SetMobility(EComponentMobility::Movable);
	ObjectMesh->SetUseCCD(true);
	ObjectMesh->SetRenderCustomDepth(true);
	ObjectMesh->SetIsReplicated(true);
	bHasBeenCalled = true;
	this->SetReplicates(true);
	this->AActor::SetReplicateMovement(true);

	LinearColors.Add(FLinearColor(0.043333, 0.390625, 0.0f, 1.0f));
	LinearColors.Add(FLinearColor(0.973958f, 0.036101f, 0.0f, 1.0f));
	LinearColors.Add(FLinearColor(0.263f, 0.0f, 0.6f, 1.0f));
}

ASPObject::~ASPObject()
{
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


	OriginMaterial = ObjectMesh->GetMaterial(ElementIndex); // mesh origin
	ObjectDynamic = ObjectMesh->CreateDynamicMaterialInstance(ElementIndex, nullptr, FName(TEXT("None")));
	ChaosDynamic = UMaterialInstanceDynamic::Create(OriginMaterial, nullptr, NAME_None);
}

void ASPObject::OnExplosionHit()
{
	if (bHasBeenCalled)
	{
		ObjectMesh->SetHiddenInGame(true);
		ObjectMesh->SetSimulatePhysics(true);
		ObjectMesh->SetCollisionProfileName(TEXT("DestructionCollision"));
		SP_LOG(LogSPNetwork, Log, TEXT("DestructionCollision !!!"));

		//FName ComponentName = FName(TEXT("GC_Cone"));
		UGeometryCollectionComponent* Geometry = NewObject<UGeometryCollectionComponent>(
			this, UGeometryCollectionComponent::StaticClass(), TEXT("GeometryComponent"));
		if (Geometry)
		{
			Geometry->SetupAttachment(RootComponent);
			Geometry->SetRestCollection(GeometryCollection);
			Geometry->SetCollisionProfileName(TEXT("DestructionCollision"));
			Geometry->SetMaterial(ElementIndex, ChaosDynamic);
			if (MyColorType != ColorType::None)
			{
				ChaosDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),
				                                      LinearColors[static_cast<uint8>(MyColorType)]);
			}
			Geometry->RegisterComponent();
			Geometry->AddImpulse(FVector(0.0f, 0.0f, 125.f));
			FTimerHandle ChangeCollisionProfileTimer;
			float DelayInSeconds = 1.0f;
			GetWorld()->GetTimerManager().SetTimer(ChangeCollisionProfileTimer, [this, Geometry]()
			{
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
	}
	else
	{
		ObjectLocation = GetActorLocation();
	}
}


void ASPObject::MultiRPCExplosionHit_Implementation()
{
}

void ASPObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ASPObject, bHasBeenCalled);
}

// Called every frame

void ASPObject::OnChangeColorGreen()
{
	if (ObjectDynamic)
	{
		
		MyColorType = ColorType::Green;
		ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),
		                                       LinearColors[static_cast<uint8>(MyColorType)]);
	}
}

void ASPObject::OnChangeColorOrange()
{
	if (ObjectDynamic)
	{
		MyColorType = ColorType::Orange;
		ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),
		                                       LinearColors[static_cast<uint8>(MyColorType)]);
	}
}

void ASPObject::OnChangeColorPurple()
{
	if (ObjectDynamic)
	{
		MyColorType = ColorType::Purple;
		ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),
		                                       LinearColors[static_cast<uint8>(MyColorType)]);
	}
}
