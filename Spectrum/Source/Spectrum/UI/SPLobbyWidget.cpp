// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPLobbyWidget.h"
#include "Protocol.pb.h"
#include "Spectrum.h"


bool USPLobbyWidget::ValidateID(const FString& ID)
{
    // ���̵� ȭ�鿡 ���(Ȯ�ο�)
    //if( !ID.IsEmpty())
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), *ID));

	std::string myid = TCHAR_TO_UTF8(*ID);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s"), myid));

	return myid == "a";

	// ���̵� DB�� �ִ��� Ȯ���ϱ� ���� ������ ����
	{ // �α���	��Ŷ ������
		Protocol::C_LOGIN Pkt;
		Pkt.set_membership_id("test");

		SEND_PACKET(Pkt);
	}

	// ������ ȸ������ �������� �Ѿ false ��ȯ
	//return false;
	
	//������ true ��ȯ
	//return true;
}


void USPLobbyWidget::Membership(const FString& ID, const FString& SCHOOL, const FString& GENDER)
{
	// ȸ������ ��Ŷ�� �־ ������




}

void USPLobbyWidget::Matching()
{
	// ��Ī ��Ŷ�� �־ ������

}

void USPLobbyWidget::PublicChat(const FString& chat)
{
	// ä�� ��Ŷ�� �־ ������
}



void USPLobbyWidget::PrivateChat(const FString& school_type,const FString& chat)
{
	//if(school_type == �ڽ��� �б�)
	// ä�� ��Ŷ�� �־ ������
	// else
	// ���� �Ұ� �޼��� ���
}

void USPLobbyWidget::AddFriend(const FString& ID)
{
	// ģ�� �߰� ��Ŷ�� �־ ������

}