#pragma once
class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

public:
	bool IsPlayer() { return isPlayer; }

public:
	Protocol::ObjectInfo* objectInfo;
	Protocol::PositionInfo* posInfo;
	Protocol::ThingInfo* thingInfo;


public:
	atomic<weak_ptr<Room>> room;

protected:
	bool isPlayer = false;
};

