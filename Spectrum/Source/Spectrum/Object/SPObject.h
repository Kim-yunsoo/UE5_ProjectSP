// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPDamageInterface.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionObject.h
//#include "GeometryCollection\GeometryCollectionObject.h"GeometryCollectionEngine
#include "Protocol.pb.h"
#include "SPObject.generated.h"


UENUM()
enum class ColorType :uint8
{
	
	Green=0,
	Orange=1,
	Purple=2,
	None
};


UCLASS()
class SPECTRUM_API ASPObject : public AActor , public ISPDamageInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPObject();
	virtual ~ASPObject() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected: //ISPDamageInterface
	virtual void OnExplosionHit();
	virtual void OnChangeColorGreen();


	//TObjectPtr<class UGeometryCollection> GeometryCollection ;

public:
	class Protocol::ThingInfo* ObjectInfo; // 현재 위치
	class Protocol::ThingInfo* DestInfo;	// 가려는 위치

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//UPROPERTY(EditAnywhere);
	//TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bHasBeenCalled;
	bool bIsFrist;

	void SetPostionInfo(const Protocol::ThingInfo& Info);
	void SetDestInfo(const Protocol::ThingInfo& Info);
	void SetBurst(const bool burst);

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<UStaticMeshComponent> ObjectMesh ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "ture"))
	FVector ObjectLocation;

protected:// chaos distruction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UGeometryCollection> GeometryCollection;
protected://DynamicMaterial
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ObjectDynamic ;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ChaosDynamic ;
	
	UPROPERTY()
	ColorType MyColorType;

	UPROPERTY()
	TArray<FLinearColor> LinearColors;

	// UPROPERTY()
	// FLinearColor GreenLinearColor{0.03f,1.0f,0.181f,1.0f};
	// UPROPERTY()
	// FLinearColor OrangeLinearColor{ 0.942f ,0.0266f ,0.0f,1.0f};
	// UPROPERTY()
	// FLinearColor PurpleLinearColor{ 0.263f,0.0f,0.6f,1.0f};
};
