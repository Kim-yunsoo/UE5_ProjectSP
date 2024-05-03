// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SPGlobalEnum.h"
#include "Interface/SPScoreInterface.h"
#include "SPGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnScore, const ColorType& /*MyColor*/,const int32 /*Score*/)

UCLASS()
class SPECTRUM_API ASPGameState : public AGameState, public ISPScoreInterface
{
	
	GENERATED_BODY()
	
public:
	FOnScore OnScore;
	ASPGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
protected:
	
	UPROPERTY(Replicated)
	int32 GreenScore;

	UPROPERTY(Replicated)
	int32 OrangeScore;

	UPROPERTY(Replicated)
	int32 PurpleScore;

	virtual void AddScore(const ColorType& MyColor) override;

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC(const ColorType& Mycolor, const int Score);
	

// public:
// 	UFUNCTION()
// 	void OnRep_AddSocreEnvet();
	
};
