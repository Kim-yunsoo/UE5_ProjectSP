// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SPGlobalEnum.h"
#include "Interface/SPScoreInterface.h"
#include "Data/SPColorScoreData.h"

#include "SPGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnScore, const ColorType& /*MyColor*/,const int32 /*Score*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTime,float /*Time*/);

UCLASS()
class SPECTRUM_API ASPGameState : public AGameState, public ISPScoreInterface
{
	
	GENERATED_BODY()
	
public:
	FOnScore OnScore;
	FOnTime OnTime;
	ASPGameState();
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(ReplicatedUsing=On_RapGreenScore)
	int32 GreenScore;

	UPROPERTY(ReplicatedUsing=On_RapOrangeScore)
	int32 OrangeScore;

	UPROPERTY(ReplicatedUsing=On_RapPurpleScore)
	int32 PurpleScore;
	
	UPROPERTY(ReplicatedUsing=OnInGamePlaySound)
	int8 bIsInGame;


	UPROPERTY()
	TObjectPtr<UDataTable> PositionTable;

	virtual void AddScore(const ColorType& MyColor,EScoreType ScoreType) override;


	
public:
	UPROPERTY(Transient, ReplicatedUsing= OnRapTime) //게임 시간 
	int32 RemainingTime;
	
	int32 MatchPlayTime = 30.f; // 3분
	int32 AISpawnTime= 15.0f; //30초 남았을 때 
	int32 SpectrumPotionSpawnTime = 120.0; //2분전
	
protected: //Timer
	UPROPERTY()
	TObjectPtr<USoundWave> BackGroundMusic;
	
	virtual void DefaultGameTimer(); //타이머로 사용할 함수 
	FTimerHandle GameTimerHandle;
	UFUNCTION()
	void On_RapGreenScore();

	UFUNCTION()
	void On_RapOrangeScore();

	UFUNCTION()
	void On_RapPurpleScore();
	
	UFUNCTION()
	void OnRapTime();

	UFUNCTION()
	void OnInGamePlaySound();

	UPROPERTY(Replicated)
	uint32 ReadyCount;

public:
	FORCEINLINE int32 GetGreenScore() const {return GreenScore; };
	FORCEINLINE int32 GetOrangeScore() const {return OrangeScore; };
	FORCEINLINE int32 GetPurpleScore() const {return PurpleScore; };
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Ready();
	void SpectrumPotionSpawn();

	void StartTimer();

	void OnMathStateSet(FName State); // 매치 상태를 알려주는 함수를 만든다.
	
};
