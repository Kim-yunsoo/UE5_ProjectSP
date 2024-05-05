// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "GameFramework/Actor.h"
#include "SPScoreTrigger.generated.h"
class ASPObject;
enum class ColorType : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FScoreDelegate, const ColorType& /*MyColor*/);

UCLASS()
class SPECTRUM_API ASPScoreTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPScoreTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FScoreDelegate OnScore;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Color")
	ColorType Color;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UBoxComponent> Trigger;

	UFUNCTION(Server,Unreliable)
	void ServerRPC(ASPObject* Object);
	
	UFUNCTION(Server,Unreliable)
	void ServerSpectrumRPC(const TArray<AActor*>& ObjectArray);

	// UFUNCTION(NetMulticast,Unreliable)
	// void MultiSpectrumRPC(ASPObject* Object);
	FORCEINLINE void AddScore(const ColorType& MyColor) {OnScore.Broadcast(MyColor);}

	
	
};
