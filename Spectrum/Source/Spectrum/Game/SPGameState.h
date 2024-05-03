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
public:
	UPROPERTY(Transient, Replicated) //게임 시간 
	int32 RemainingTime;
	
	int32 MatchPlayTime = 10.f; // 2분

protected: //Timer

	// virtual void PostInitializeComponents() override; //여기서 타이머 가동한다. 
	// virtual void DefaultGameTimer(); //타이머로 사용할 함수 
	// FTimerHandle GameTimerHandle;
	//
	// UFUNCTION(Client,Unreliable)
	// void ClientRPC();
	

// public:
// 	UFUNCTION()
// 	void OnRep_AddSocreEnvet();
	
};
