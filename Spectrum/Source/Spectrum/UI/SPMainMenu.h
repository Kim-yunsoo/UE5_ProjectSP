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
	virtual void NativeConstruct() override; // �ʱ�ȭ
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	//���󿡼��� ������ ����� ���� �޴� -> �츰 ���۴뿡 ���°ɷ� ����ϸ� �ɵ�?
	//�� ������ ������ ���忡 ���͸� �����ؼ� �� ��ġ�� ���� �� �ְ� �����.

public:
	UPROPERTY()
	TObjectPtr<ASPCharacterPlayer> PlayerCharacter; // ����� �÷��̾� ĳ������ �κ��丮�� ����Ǵ� ���!
	
};
