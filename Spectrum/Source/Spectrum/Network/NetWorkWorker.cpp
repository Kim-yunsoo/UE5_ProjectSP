// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetWorkWorker.h"
#include "Sockets.h"
#include "Serialization/ArrayWriter.h"
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
{	// 여기서 여러 함수로 관리 ex) kill, wait
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
	return true;
}

uint32 RecvWorker::Run()
{	// 스레드 메인 함수
	// 받은 패킷 조립 여기서

	while (Running)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			// 패킷 처리(조립)
			// 조립 후 메인 스레드에 넘겨주려면 크러쉬 남
			// 패킷을 저장(큐에 저장)하는 함수를 만들어서 그 함수를 호출하는 방식으로 처리
			// 패킷을 저장하는 함수는 패킷을 저장하는 큐를 관리하는 클래스에 있어야 함
			// 패킷을 저장하는 큐를 관리하는 클래스는 게임의 메인 스레드에서 관리해야 함
			// PacketSession에 패킷을 저장하는 함수를 만들어서 호출

			// 패킷을 저장하는 함수를 호출
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin())	// Session이 유효한지 체크. TSharedPtr로 바꿔야 함
			{
				Session->RecvPacketQueue.Enqueue(Packet);	// 패킷을 저장
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
	// 패킷 헤더 공간 확보
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	// ID, Size 추출
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);		// 리더를 만들어서
		Reader << Header;					// 해당 부분만  추출
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
		// PacketSize는 헤더를 제외한 실제 데이터의 크기(앞으로 얼마나 읽어야 하는지 알려주는 역할)
		// PacketID는 패킷을 구분하는 역할(패킷이 다 왔을 때 그 패핏을 어떻게 분석해야 하는지 알려주는 역할)
	}

	// 패킷 헤더 복사
	OutPacket = HeaderBuffer;

	// 패킷 내용 파싱
	// 아래 코드는 PayloadSize만큼의 데이터를 받아서 OutPacket에 추가하는 코드
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0)	// 헤더만 있는 패킷
		return true;

	OutPacket.AddZeroed(PayloadSize);


	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;


	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{	// 원하는 크기의 패킷을 올 때까지 대기
	// 게임의 메인 스레드가 아니라서 블러킹이 가능

	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);	// 패킷이 끊어져서 올 수 있으므로 예외 체크

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
			if (Session->SendPacketQueue.Dequeue(OUT SendBuffer))	// 큐에서 패킷을 꺼내옴
			{
				SendPacket(SendBuffer);	// 패킷을 보냄
			}
		}

		// Sleep하려면 여기서
	}

	return 0;
}

void SendWorker::Exit()
{

}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{// 보내기만 함. 패킷 조립은 다른 곳에서 함. 이미 조립이 되어 있음
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{// 원하는 크기의 패킷을 보낼 때까지 대기
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