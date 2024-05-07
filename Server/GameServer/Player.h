#pragma once
#include "Creature.h"

class GameSession;
class Lobby;

class Player : public Creature
{
public:
	Player();
	virtual ~Player();


	weak_ptr<GameSession> session;

};

