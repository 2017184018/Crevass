#include "GameInfo.h"
#include "PlayerInfo.h"


void GameInfo::Initialize()
{
	for (int i = 0; i < TOTAL_PLAYER_NUM; ++i)
	{
		m_PlayersInfo[i] = nullptr;
	}

	m_ClientsNum = 0;
	m_IsConnect = false;
	m_GameStart = false;
}

void GameInfo::CreatePlayerInfo(char id, bool isMe)
{
	m_ClientsNum++;
	PlayerInfo* pPlayerInfo = new PlayerInfo;
	pPlayerInfo->SetID(id, isMe);
	m_PlayersInfo[static_cast<int>(id)] = pPlayerInfo;
}

void GameInfo::DeletePlayerInfo(char id)
{
	m_ClientsNum--;
	delete m_PlayersInfo[static_cast<int>(id)];
	m_PlayersInfo[static_cast<int>(id)] = nullptr;
}

