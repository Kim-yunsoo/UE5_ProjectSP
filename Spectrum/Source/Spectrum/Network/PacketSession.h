// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spectrum.h"
/**
 * 
 */
class SPECTRUM_API PacketSession : public TSharedFromThis <PacketSession>
{
public:
	PacketSession(class FSocket* Socket);
	~PacketSession();


	void Run();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	void Disconnect();

public:
	class FSocket* Socket;

	// NetworkThread���� ����ϴ� ������.
	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;	// SendPacketQueue�� �ִ� ��Ŷ�� �ϳ��� �����ִ� ������

	// GameThread�� NetworkThread�� ������ �ְ� �޴� �����ڿ� ť.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
