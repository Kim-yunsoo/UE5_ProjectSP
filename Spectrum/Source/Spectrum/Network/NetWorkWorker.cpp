// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetWorkWorker.h"
#include "Sockets.h"
#include "PacketSession.h"

//////////////////// RecvWorker ////////////////////
RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));

}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{	// ���⼭ ���� �Լ��� ���� ex) kill, wait
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run()
{	// ������ ���� �Լ�
	// ���� ��Ŷ ���� ���⼭

	while (Running)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			// ��Ŷ ó��(����)
			// ���� �� ���� �����忡 �Ѱ��ַ��� ũ���� ��
			// ��Ŷ�� ����(ť�� ����)�ϴ� �Լ��� ���� �� �Լ��� ȣ���ϴ� ������� ó��
			// ��Ŷ�� �����ϴ� �Լ��� ��Ŷ�� �����ϴ� ť�� �����ϴ� Ŭ������ �־�� ��
			// ��Ŷ�� �����ϴ� ť�� �����ϴ� Ŭ������ ������ ���� �����忡�� �����ؾ� ��
			// PacketSession�� ��Ŷ�� �����ϴ� �Լ��� ���� ȣ��

			// ��Ŷ�� �����ϴ� �Լ��� ȣ��
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin())	// Session�� ��ȿ���� üũ. TSharedPtr�� �ٲ�� ��
			{
				Session->RecvPacketQueue.Enqueue(Packet);	// ��Ŷ�� ����
			}

		}
	}

	return 0;
}

void RecvWorker::Exit()
{
}

void RecvWorker::Destroy()
{
	Running = false;

}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{
	// ��Ŷ ��� ���� Ȯ��
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	// ID, Size ����
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);		// ������ ����
		Reader << Header;					// �ش� �κи�  ����
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
		// PacketSize�� ����� ������ ���� �������� ũ��(������ �󸶳� �о�� �ϴ��� �˷��ִ� ����)
		// PacketID�� ��Ŷ�� �����ϴ� ����(��Ŷ�� �� ���� �� �� ������ ��� �м��ؾ� �ϴ��� �˷��ִ� ����)
	}

	// ��Ŷ ��� ����
	OutPacket = HeaderBuffer;

	// ��Ŷ ���� �Ľ�
	// �Ʒ� �ڵ�� PayloadSize��ŭ�� �����͸� �޾Ƽ� OutPacket�� �߰��ϴ� �ڵ�
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0)	// ����� �ִ� ��Ŷ
		return true;

	OutPacket.AddZeroed(PayloadSize);


	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;


	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{	// ���ϴ� ũ���� ��Ŷ�� �� ������ ���
	// ������ ���� �����尡 �ƴ϶� ��ŷ�� ����

	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);	// ��Ŷ�� �������� �� �� �����Ƿ� ���� üũ

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}



//////////////////// SendWorker ////////////////////
SendWorker::SendWorker(FSocket* Socket, TSharedPtr<PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{

}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));

	return true;
}

uint32 SendWorker::Run()
{
	while (Running)
	{
		SendBufferRef SendBuffer;
		
		if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
		{
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))	// ť���� ��Ŷ�� ������
			{
				SendPacket(SendBuffer);	// ��Ŷ�� ����
			}
		}

		// Sleep�Ϸ��� ���⼭
	}

	return 0;
}

void SendWorker::Exit()
{

}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{// �����⸸ ��. ��Ŷ ������ �ٸ� ������ ��. �̹� ������ �Ǿ� ����
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{// ���ϴ� ũ���� ��Ŷ�� ���� ������ ���
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}