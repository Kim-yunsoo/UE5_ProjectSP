// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/SPExplosionComponent.h"
#include "Potion/SPPotionBase.h"
#include "SPBlackPotion.generated.h"
//enum class EColorType;


UCLASS()
class SPECTRUM_API ASPBlackPotion : public ASPPotionBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPBlackPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USPExplosionComponent> ExplosionComponent ;
	//RPC


	UFUNCTION()
	void HandleActorHit(AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> WaterSound;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> CrushSound;

};
