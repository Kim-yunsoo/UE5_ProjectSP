#pragma once
class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

public:
	bool IsPlayer() { return isPlayer; }

public:
	Protocol::PlayerInfo* objectInfo;
	string player_id;
	Protocol::PlayerType membership_type;



public:
	atomic<weak_ptr<Lobby>> lobby;

protected:
	bool isPlayer = false;
};


