// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "ClientPacketHandler.h"


PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
	ClientPacketHandler::Init();	// �Լ� ������ ä��� �۾� ���⼭(��Ŷ �Լ� �ʱ�ȭ)
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	// RecvWorkerThread ����
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());	// �ڱ� �ڽſ� ���� SharedRef�� �Ѱ���.
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());

}

void PacketSession::HandleRecvPackets()
{
	// RecvWorkerThread�� ���� ��Ŷ�� ó���Ѵ�.
	while (true)
	{
		// ��Ŷ�� �ϳ��� ó���Ѵ�.
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		PacketSessionRef ThisPtr = AsShared();
		ClientPacketHandler::HandlePacket(ThisPtr, Packet.GetData(), Packet.Num());
		// ���⿡ case�� ������ ��Ŷ ���� �� ó���ص����� ���ŷο�ϱ� HandlePacket�Լ��� ���� ó��
	}
}

void PacketSession::SendPacket(SendBufferRef SendBuffer)
{
	// send Packet Queue�� ����
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

