// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPObject.h"
#include "Spectrum.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "GeometryCollection/GeometryCollection.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionComponent.h
#include "GeometryCollection/GeometryCollectionObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "GeometryCollection\GeometryCollectionComponent.h"

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
   bHasBeenCalled = true;

   LinearColors.Add(FLinearColor(0.043333, 0.390625, 0.0f, 1.0f));   
   LinearColors.Add(FLinearColor(0.973958f, 0.036101f, 0.0f, 1.0f)); 
   LinearColors.Add(FLinearColor(0.263f, 0.0f, 0.6f, 1.0f));    
   
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

   {
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
      //FName ComponentName = FName(TEXT("GC_Cone"));
      UGeometryCollectionComponent* Geometry = NewObject<UGeometryCollectionComponent>(this, UGeometryCollectionComponent::StaticClass(), TEXT("GeometryComponent"));
      if (Geometry)
      {
         Geometry->SetupAttachment(RootComponent);
         Geometry->SetRestCollection(GeometryCollection);
         Geometry->SetCollisionProfileName(TEXT("DestructionCollision"));
         Geometry->SetMaterial(ElementIndex, ChaosDynamic);
         if(MyColorType!=ColorType::None)
         {
            ChaosDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),LinearColors[static_cast<uint8>(MyColorType)]);
         }
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

   {
      // SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
      // FVector Location(DestInfo->x(), DestInfo->y(), DestInfo->z());
      // FVector TargetLocation = Location;
      // float InterpSpeed = 1.0f; 
      //
      // FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, DeltaTime * InterpSpeed);
      // SetActorLocation(NewLocation);
      //
      // FRotator Rotation(0, DestInfo->yaw(), 0);
      // SetActorRotation(Rotation);
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
   if (!Equal && MovePacketSendTimer <= 0 && bIsFrist == false)   
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
      //ObjectMesh->SetHiddenInGame(true);
      //ObjectMesh->SetSimulatePhysics(true);
      //ObjectMesh->SetCollisionProfileName(TEXT("DestructionCollision"));
      ////FName ComponentName = FName(TEXT("GC_Cone"));
      //UGeometryCollectionComponent* Geometry = NewObject<UGeometryCollectionComponent>(this, UGeometryCollectionComponent::StaticClass(), TEXT("GeometryComponent"));
      //if (Geometry)
      //{
      //   Geometry->SetupAttachment(RootComponent);
      //   Geometry->SetRestCollection(GeometryCollection);
      //   Geometry->SetCollisionProfileName(TEXT("DestructionCollision"));
      //   Geometry->SetMaterial(0, ChaosDynamic);
      //   if(MyColorType!=ColorType::None)
      //   {
      //      ChaosDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),LinearColors[static_cast<uint8>(MyColorType)]);
      //   }
      //   Geometry->RegisterComponent();
      //   Geometry->AddImpulse(FVector(0.0f, 0.0f, 125.f));
      //   FTimerHandle ChangeCollisionProfileTimer;
      //   float DelayInSeconds = 1.0f;
      //   GetWorld()->GetTimerManager().SetTimer(ChangeCollisionProfileTimer, [this, Geometry]() {
      //      Geometry->SetCollisionProfileName(TEXT("OnlyStaticCollision"));
      //      ObjectMesh->SetCollisionProfileName(TEXT("OnlyStaticCollision"));
      //      }, DelayInSeconds, false);
      //   this->SetLifeSpan(5.0f);
      //}
      //bHasBeenCalled = false;

      OnExplosionHit();

   }
}
void ASPObject::OnChangeColorGreen()
{
   if(ObjectDynamic)
   {
      MyColorType=ColorType::Green;
      ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),LinearColors[static_cast<uint8>(MyColorType)]);
   }
}

void ASPObject::OnChangeColorOrange()
{
   if(ObjectDynamic)
   {
      MyColorType=ColorType::Orange;
      ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),LinearColors[static_cast<uint8>(MyColorType)]);
   }
}

void ASPObject::OnChangeColorPurple()
{
   if(ObjectDynamic)
   {
      MyColorType=ColorType::Purple;
      ObjectDynamic->SetVectorParameterValue(FName(TEXT("Base Color Tint")),LinearColors[static_cast<uint8>(MyColorType)]);
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
   if (bHasBeenCalled == true)
   {
      bHasBeenCalled = false;
      bIsFrist = true;
   }
}