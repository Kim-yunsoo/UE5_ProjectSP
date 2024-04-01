// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPDamageInterface.h"
#include "SPObject.generated.h"

UCLASS()
class SPECTRUM_API ASPObject : public AActor , public ISPDamageInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnExplosionHit(float Damage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//UPROPERTY(EditAnywhere);
	//TObjectPtr<UStaticMeshComponent> MeshComponent;
	bool bHasBeenCalled;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<UStaticMeshComponent> ObjectMesh ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "ture"))
	FVector ObjectLocation;

};
