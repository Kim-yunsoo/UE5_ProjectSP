#pragma once

class ObjectUtils
{
public:
	static PlayerRef CreatePlayer(GameSessionRef session);
	static ThingRef CreateThing();

private:
	static atomic<int64> s_idGenerator;
};

