
#pragma once
#include "CoreMinimal.h"
#include "Data/SPTeleportData.h"
#include "GameFramework/Actor.h"
#include "SPPortalTrigger.generated.h"

UCLASS()
class SPECTRUM_API ASPPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DestroyEffect(UParticleSystemComponent* ParticleComponent);

public:	

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> Trigger;


	UPROPERTY()
	TObjectPtr<UDataTable>  PortalData;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	FVector LocationResult ; 

	FPortalDataBase* LocationData;

	

	UFUNCTION(Server,Unreliable)
	void ServerRPC(AActor* OverlapActor,const FVector& Location);

	UPROPERTY()
	TObjectPtr<USoundWave> PortalSound;

	UFUNCTION(Client,Unreliable)
	void ClientRPCSound(USoundWave* Sound);

	UPROPERTY()
	TObjectPtr<UParticleSystem> Effect;


};
