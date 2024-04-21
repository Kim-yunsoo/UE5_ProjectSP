// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SPDamageInterface.h"
//D:\UE_5.3\Engine\Source\Runtime\Experimental\GeometryCollectionEngine\Public\GeometryCollection\GeometryCollectionObject.h
//#include "GeometryCollection\GeometryCollectionObject.h"GeometryCollectionEngine
#include "DataTable/SPColorDataTable.h"
#include "SPObject.generated.h"


// UENUM()
// enum class ColorType :uint8
// {
// 	Green=0,
// 	Orange=1,
// 	Purple=2,
// 	None
// };


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
	virtual void OnExplosionHit() override;
	virtual void OnChangeColorGreen() override;
	virtual void OnChangeColorOrange() override;
	virtual void OnChangeColorPurple() override;

	//TObjectPtr<class UGeometryCollection> GeometryCollection ;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	//UPROPERTY(EditAnywhere);
	//TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bHasBeenCalled;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.2f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Meta = (AllowPrivateAccess = "ture"))
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
	TObjectPtr<class UMaterialInterface> OriginMaterial ;
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ChaosDynamic ;
	
	UPROPERTY()
	ColorType MyColorType;

	//Get Material Index
	int32 ElementIndex = 0;

	//RPC
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCExplosionHit();

	//funtion
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Data Table
	UPROPERTY()
	TObjectPtr<UDataTable> GreenData;

	UPROPERTY()
	TObjectPtr<UDataTable> OrangeData;

	UPROPERTY()
	TObjectPtr<UDataTable> PurpleData;

	UPROPERTY()
	FName RowName{TEXT("ColorType")};

	UPROPERTY()
	FName DynamicParam{TEXT("Base Color Tint")};

	//DataFunction
	void DynamicSetColor(const UDataTable* Table);
	
};
