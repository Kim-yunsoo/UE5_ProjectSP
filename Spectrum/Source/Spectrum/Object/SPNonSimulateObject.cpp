// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/SPNonSimulateObject.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASPNonSimulateObject::ASPNonSimulateObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; 

	//NetDormancy=DORM_Initial;
}

// Called when the game starts or when spawned
void ASPNonSimulateObject::BeginPlay()
{
	Super::BeginPlay();

	// if(HasAuthority())
	// {
	// 	bReplicates = true; 
	// 	this->AActor::SetReplicateMovement(true);
	// }
	if(HasAuthority())
	{
		//this->SetReplicates(true);
		this->AActor::SetReplicateMovement(true);
	}

}

// Called every frame
void ASPNonSimulateObject::Tick(float DeltaTime)
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
	}
	else
	{
		ObjectLocation = GetActorLocation();
	}
}

