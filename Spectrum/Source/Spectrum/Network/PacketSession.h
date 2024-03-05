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

	// NetworkThread에서 사용하는 변수들.
	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;	// SendPacketQueue에 있는 패킷을 하나씩 보내주는 스레드

	// GameThread와 NetworkThread가 데이터 주고 받는 공용자원 큐.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
