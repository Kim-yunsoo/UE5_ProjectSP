// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPGlobalEnum.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "SPGameModeBase.generated.h"

class ASPPlayerController;
enum class GenderType : uint8;
enum class ColorType : uint8;



UCLASS()
class SPECTRUM_API ASPGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ASPGameModeBase();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	void SpawnPlayerCharacter(APlayerController* MyController, ColorType& MyColor, GenderType& MyGender);
	virtual void PostLogin(APlayerController* NewPlayer) override;


private:
	TSubclassOf<UUserWidget> SPLobbyWidgetClass;
	
public:
	UPROPERTY()
	float WarmupTime = 10.0f; // 로딩 시간

	float LevelStartingTime = 0.f; //인게임에 진입한 시간 기록을 위한 변수
	
	void SendMessagesToEveryOne(const FString& Sender, const FString& Message);
	virtual void PostSeamlessTravel() override;
	
	UFUNCTION()
	void AISpawn();

	void FinishMatch(); //매치가 종료되면 호출한다. 
protected:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
	UPROPERTY()
	TSubclassOf<APawn> AIPawnClass;
	
	virtual void OnMatchStateSet() override;
	float CountdownTime = 0.f;

	UPROPERTY()
	TObjectPtr<AActor> AIPawn;
	void FinalizeMatchResults();
};