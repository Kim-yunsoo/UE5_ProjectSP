#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ObjectUtils.h"
#include "Thing.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
	//createAllObject();

}

Room::~Room()
{

}

bool Room::EnterRoom(ObjectRef object, bool randPos)
{
	WRITE_LOCK;

	bool success = EnterObject(object);

	// ���� ��ġ
	if(randPos)
	{
		object->posInfo->set_x(Utils::GetRandom(-100.f, 100.f));
		object->posInfo->set_y(0.f);
		object->posInfo->set_z(Utils::GetRandom(-100.f, 100.f));
		object->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));
	}
	

	// ���� ����� ���� �÷��̾�� �˸���
	if(auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);
		//enterGamePkt.release_player();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �ٸ� �÷��̾�� �˸���
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _player)
		{
			if (item.second->IsPlayer() == false)		// �÷��̾����׸� ����. ���ǵ����״� ���� �ʿ� �����ϱ�!
				continue;

			Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = object->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);
		if (auto player = dynamic_pointer_cast<Player>(object))
			if (auto session = player->session.lock())
				session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterObject(player);

	// �÷��̾� ��ġ
	player->posInfo->set_x(Utils::GetRandom(-100.f, -50.f));
	player->posInfo->set_y(0.f);
	player->posInfo->set_z(Utils::GetRandom(50.f, 100.f));
	player->posInfo->set_yaw(Utils::GetRandom(0.f, 100.f));

	// ���� ����� ���� �÷��̾�� �˸���
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* playerInfo = new Protocol::ObjectInfo();
		playerInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);
		//enterGamePkt.release_player();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �ٸ� �÷��̾�� �˸���
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->objectInfo->object_id());
	}

	// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _player)
		{
			if(item.second->IsPlayer() == false)		// �÷��̾� ������ ������. ���ǵ� ������ �ڿ��� ����
				continue;

			Protocol::ObjectInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

		//// �ʿ� ��ġ�� ���ǵ� ���� ����
	{
		// _objects�� �ִ� ���ǵ� ���� ����
		{
			Protocol::S_O_SPAWN Opkt;

			for (auto& item : _objects)
			{
				Protocol::ThingInfo* Info = new Protocol::ThingInfo();
				Info->CopyFrom(*item.second->thingInfo);
				Opkt.set_allocated_objects(Info);

				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(Opkt);
				if (auto session = player->session.lock())
					session->Send(sendBuffer);
			}
		}

	}

	return success;
	//return EnterRoom(player, true);
}


bool Room::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
	//return LeaveRoom(player);
}

void Room::HandleMoveLocked(Protocol::C_MOVE& pkt)
{
	WRITE_LOCK;

	const uint64 objectId = pkt.info().object_id();
	if (_player.find(objectId) == _player.end())
		return;

	// ����
	PlayerRef player = dynamic_pointer_cast<Player>(_player[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	// �̵� 
	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PositionInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandleMoveLocked(Protocol::C_O_MOVE& pkt)
{
	WRITE_LOCK;

	// �̵� 
	{
		Protocol::S_O_MOVE movePkt;
		{
			Protocol::ThingInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}

}

void Room::HandleTurnLocked(Protocol::C_TURN& pkt)
{
	WRITE_LOCK;

	const uint64 objectId = pkt.info().object_id();
	if (_player.find(objectId) == _player.end())
		return;

	// ȸ�� 
	{
		Protocol::S_TURN turnPkt;
		{
			Protocol::PlayerTurnInfo* info = turnPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(turnPkt);
		Broadcast(sendBuffer);
	}

}

void Room::HandleBurstLocked(Protocol::C_O_BURST& pkt)
{

	WRITE_LOCK;

	// �̵� 
	{
		Protocol::S_O_BURST burstPkt;
		{
			Protocol::BurstInfo* info = burstPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(burstPkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandlePotionLocked(Protocol::C_O_POTION& pkt)
{
	WRITE_LOCK;

	{
		Protocol::S_O_POTION potionPkt;
		{
			Protocol::PlayerPotionInfo* info = potionPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(potionPkt);
		Broadcast(sendBuffer);
	}
}

void Room::updateTick()
{
	cout<<"Room::updateTick"<<endl;
}

void Room::createAllObject() // ��ġ�� ���� �����ؼ� _objects�� �����ص�
{
	//ThingRef thing = ObjectUtils::CreateThing();

	//// ������ ��ġ, id ����
	//Protocol::PositionInfo* posInfo = new Protocol::PositionInfo();
	//posInfo->set_x(700.f);
	//posInfo->set_y(-180.f);
	//posInfo->set_z(120.f);
	//posInfo->set_yaw(4.f);
	//posInfo->set_object_id(thing->posInfo->object_id());		//

	////EnterObject(thing);
	//if (_objects.find(thing->objectInfo->object_id()) != _objects.end())
	//	cout << " �̹� �ִ� ������Ʈ ����� " << endl;
	//else
	//	_objects.insert(make_pair(thing->posInfo->object_id(), thing));

	//cout << thing->posInfo->object_id() << endl;

}

bool Room::EnterObject(ObjectRef object)
{
	// �ִٸ� ������ �ִ�.
	if (_player.find(object->objectInfo->object_id()) != _player.end())
		return false;

	_player.insert(make_pair(object->objectInfo->object_id(), object));
	
	object->room.store(shared_from_this());

	return true;
}


bool Room::LeaveObject(uint64 objectId)
{
	// ���ٸ� ������ �ִ�.
	if (_player.find(objectId) == _player.end())
		return false;

	ObjectRef object = _player[objectId];
	object->room.store(weak_ptr<Room>());

	_player.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{	// �÷��̾����׸� ����
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// �÷��̾�� �ٲ㼭 ������׸� ó��
		if(player == nullptr)
			continue;

		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}

void Room::ObjectBroadcast(SendBufferRef sendBuffer)
{	// �÷��̾����׸� ����
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// �÷��̾�� �ٲ㼭 ������׸� ó��
		if (player == nullptr)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}