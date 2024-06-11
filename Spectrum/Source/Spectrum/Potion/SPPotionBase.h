// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/SPGetInterface.h"
#include "SPPotionBase.generated.h"
enum class ColorType : uint8;

UCLASS()
class SPECTRUM_API ASPPotionBase : public AActor, public ISPGetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPotionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PotionMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USPExplosionComponent> ExplosionComponent ;

	float BaseDamage; 

	bool bHasExecutedOnce;
public:
	void Throw(const FVector& PotionDirection);

	void MoveTo();

	virtual  void GetPotion() override;

	ColorType MyColor;
};
