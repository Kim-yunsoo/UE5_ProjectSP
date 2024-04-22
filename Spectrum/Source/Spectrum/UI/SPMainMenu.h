// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPUserWidget.h"
#include "SPMainMenu.generated.h"

class ASPCharacterPlayer;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPMainMenu : public USPUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override; // 초기화
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	//영상에서는 월드의 드롭을 위한 메뉴 -> 우린 제작대에 놓는걸로 사용하면 될듯?
	//이 위젯에 놓으면 월드에 액터를 생성해서 그 위치에 나올 수 있게 만든다.

public:
	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter; // 드롭은 플레이어 캐릭터의 인벤토리에 연결되는 기능!
	
};
