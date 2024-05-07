#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "Player.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
	cout << "add session" <<endl;
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
		PlayerRef player = session->player.load();
		cout << "Broadcast " << "id : " << player->player_id << endl;

	}
}

void GameSessionManager::RoomBroadcast(SendBufferRef sendBuffer, const string membership_id)
{

	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		PlayerRef player = session->player.load();
		if (player->player_id == membership_id)
		{
			session->Send(sendBuffer);
			cout << "RoomBroadcast id : " << player->player_id << endl;

		}
	}	
}
