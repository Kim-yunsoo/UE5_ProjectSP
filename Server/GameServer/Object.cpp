#include "pch.h"
#include "Object.h"

Object::Object()
{
	objectInfo = new Protocol::PlayerInfo();

}

Object::~Object()
{
	delete objectInfo;

}
