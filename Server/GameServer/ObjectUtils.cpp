#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Thing.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	// ID 积己扁
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->objectInfo->set_object_id(newId);
	player->posInfo->set_object_id(newId);

	player->session = session;
	session->player.store(player);

	return player;
}

ThingRef ObjectUtils::CreateThing()
{
	// ID 积己扁
	const int64 newId = s_idGenerator.fetch_add(1);

	ThingRef thing = make_shared<Thing>();
	thing->objectInfo->set_object_id(newId);
	thing->posInfo->set_object_id(newId);
	thing->thingInfo->set_object_id(newId);
	//thing->session = session;
	//session->thing.store(player);

	return thing;
}
