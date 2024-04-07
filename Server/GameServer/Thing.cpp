#include "pch.h"
#include "Thing.h"

Thing::Thing()
{
	objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_THING);
}

Thing::~Thing()
{
}
