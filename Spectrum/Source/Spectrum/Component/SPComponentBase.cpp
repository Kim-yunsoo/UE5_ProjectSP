// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/SPComponentBase.h"

#include "SpectrumLog.h"
#include "SPGlobalEnum.h"
#include "Data/SPExplosionData.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USPComponentBase::USPComponentBase()
{
	bHasExecutedOnce = false;
	static ConstructorHelpers::FObjectFinder<UDataTable>TableRef(TEXT("/Script/Engine.DataTable'/Game/Spectrum/ColorData/DT_ExplosionData.DT_ExplosionData'"));
	if(TableRef.Object)
	{
		DataAsset = TableRef.Object;
	}
}


// Called when the game starts
void USPComponentBase::BeginPlay()
{
	Super::BeginPlay();
}

void USPComponentBase::Explode(ColorType& MyColor)
{
	//Spawn Emitter at Location�� ���� ����
	FVector ActorLocation = GetOwner()->GetActorLocation(); //������ ������ ��ġ 
	FVector Location = ActorLocation;
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector Scale{ 1.0f,1.0f,1.0f };
	FTransform SpawnTransform{ Rotation,Location,Scale };
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, SpawnTransform, true, EPSCPoolMethod::None, true);
	//여기서 로드 에셋
	
	RoadAsset(MyColor);
	
}

void USPComponentBase::RoadAsset(ColorType& MyColor)
{
	FString AssetColor = UEnum::GetValueAsString(MyColor);
	SP_SUBLOG(LogSPNetwork,Log,TEXT("%s"), *AssetColor);

	FExplosionData* Data = DataAsset->FindRow<FExplosionData>( FName(*AssetColor),"");
	if(Data)
	{
		EmitterHit= Data->EmitterHit;
		WaterSound = Data->WaterSound;

		if(MyColor == ColorType::Black)
		{
			CrushSound = Data->CrushSound ;
		}
	}
}

void USPComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USPComponentBase, EmitterHit);
	DOREPLIFETIME(USPComponentBase, WaterSound);
	DOREPLIFETIME(USPComponentBase, CrushSound);
}
