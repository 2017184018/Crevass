#pragma once
#include "Player.h"

struct Message 
{
	char id;
	char type;
	char dir;
	bool isPushed;
};

struct PlayerReadyInfo
{
	char id;
	char ready;
};

// ==클라이언트 관리 구조체===

struct ConnectClient
{
	bool is_connected;
};
ConnectClient g_connectedCls[3]{ false };

// =======================================

std::vector <Player> phyPlayers;

// =======================================

// 플레이어들의 초기 위치

Pro_Player g_initialPos[3] = { {0, 30, 60}, {1, 460, 60}, {2, 200, 450} };

PlayerReadyInfo g_playerReadyInfo[3]{ {-1, 0}, {-1, 0}, {-1, 0} };

// =======================================

std::queue <Message> g_MsgQueue;

std::map <char, SOCKET> g_clients;


atomic_int numOfCls;				// 접속한 클라이언트 수

bool g_isPlaying = false;

mutex g_MsgQueueLock;
mutex g_connectedClsLock;
mutex g_socketLock;
mutex g_playerReadyInfoLock;
mutex g_initialPosLock;