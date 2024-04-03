#pragma once

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

public:
	bool EnterRoom(ObjectRef object, bool randPos = true);
	bool LeaveRoom(ObjectRef object);


	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);

	void HandleMoveLocked(Protocol::C_MOVE& pkt);

	void updateTick();

private:
	bool EnterObject(ObjectRef object);
	bool LeaveObject(uint64 objectId);

	USE_LOCK;

private:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	//unordered_map<uint64, PlayerRef> _players;
	unordered_map<uint64, ObjectRef> _objects;
};

extern RoomRef GRoom;