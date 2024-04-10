#include "pch.h"
#include "Object.h"

Object::Object()
{
	objectInfo = new Protocol::ObjectInfo();
	posInfo = new Protocol::PositionInfo();
	thingInfo = new Protocol::ThingInfo();
	objectInfo->set_allocated_pos_info(posInfo);

}

Object::~Object()
{
	delete objectInfo;

}
