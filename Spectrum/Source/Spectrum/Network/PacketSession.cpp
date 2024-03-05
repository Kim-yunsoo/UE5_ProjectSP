// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "ClientPacketHandler.h"


PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
	ClientPacketHandler::Init();	// 함수 포인터 채우는 작업 여기서(패킷 함수 초기화)
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	// RecvWorkerThread 생성
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());	// 자기 자신에 대한 SharedRef를 넘겨줌.
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());

}

void PacketSession::HandleRecvPackets()
{
	// RecvWorkerThread가 받은 패킷을 처리한다.
	while (true)
	{
		// 패킷을 하나씩 처리한다.
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		PacketSessionRef ThisPtr = AsShared();
		ClientPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
		// 여기에 case문 돌려서 패킷 구분 후 처리해되지만 번거로우니까 HandlePacket함수로 따로 처리
	}
}

void PacketSession::SendPacket(SendBufferRef SendBuffer)
{
	// send Packet Queue에 넣음
	SendPacketQueue.Enqueue(SendBuffer);
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

