#pragma once
#include "define.h"
#include "PlayerInfo.h"

class GameInfo
{
public:
	void Initialize();
	void CreatePlayerInfo(char id, bool isMe);
	void DeletePlayerInfo(char id);

public:
	PlayerInfo* m_PlayersInfo[TOTAL_PLAYER_NUM];

public:
	int	m_ClientsNum = 0;
	int m_ClientID = -1;
	int m_WinnerID = -1;

	// 게임 상태 변수
	bool	m_IsConnect = false;
	bool	m_GameStart = false;
	bool	m_GameOver = false;

public:
};