// Fill out your copyright notice in the Description page of Project Settings.
// 데이터의 송수신 담당 -> 역할 분담함
// 별도의 스레드로 동작
#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Spectrum.h"

class FSocket;

struct SPECTRUM_API FPacketHeader
{
	FPacketHeader() : PacketSize(0), PacketID(0)
	{
	}

	FPacketHeader(uint16 PacketSize, uint16 PacketID) : PacketSize(PacketSize), PacketID(PacketID)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}

	uint16 PacketSize; // 헤더를 제외할 지 포함할 지 정해야함. 여기서는 포함으로 가정
	uint16 PacketID;
};




class SPECTRUM_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef; // 세션을 가지고 있어야함 -> 세션을 통해 패킷을 처리해야함
	// TSharedPtr을 쓴 이유는 세션을 가지고 있는 것이 아니라 참조만 하고 있기 때문에




};



class SPECTRUM_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBufferRef SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};