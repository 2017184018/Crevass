#pragma once
#include "Player.h"
#include "pch.h"
struct Message
{
	char id;
	char type;
	char dir;
	bool isPushed;
	char AnimType;
};

struct PlayerReadyInfo
{
	char id;
	char ready;
};

// ==Ŭ���̾�Ʈ ���� ����ü===

struct ConnectClient
{
	bool is_connected;
};
ConnectClient g_connectedCls[5]{ false };

// =======================================

std::vector <Player> phyPlayers;

// =======================================

// �÷��̾���� �ʱ� ��ġ

Pro_Player g_initialPos[5] = { 
	{0,DirectX::XMFLOAT3(0.0f,100.0f,0.0f),0,0,CHARACTER_NONE},
	{1,DirectX::XMFLOAT3(200.0f,100.0f,0.0f),0,0,CHARACTER_NONE},
	{2,DirectX::XMFLOAT3(400.0f,100.0f,0.0f),0,0,CHARACTER_NONE},
	{3,DirectX::XMFLOAT3(600.0f,100.0f,0.0f),0,0,CHARACTER_NONE} ,
	{4,DirectX::XMFLOAT3(800.0f,100.0f,0.0f),0,0,CHARACTER_NONE}
};

char g_characterTypeChoose[5] = { 0,0,0,0,0 };

PlayerReadyInfo g_playerReadyInfo[5]{ {-1, 0}, {-1, 0}, {-1, 0},{-1, 0},{-1, 0} };

// =======================================

std::queue <Message> g_MsgQueue;

std::map <char, SOCKET> g_clients;

int lose_count = 0;
int who_lose[5];


atomic_int numOfCls;				// ������ Ŭ���̾�Ʈ ��

map<string, DirectX::BoundingBox*> g_boundaries;

bool g_isPlaying = false;

mutex g_MsgQueueLock;
mutex g_ConnectedClsLock;
mutex g_SocketLock;
mutex g_PlayerReadyInfoLock;
mutex g_PlayerChooseInfoLock;
mutex g_InitialPosLock;
mutex g_PlayerLobbyStateInfoLock;
mutex g_iglooPosLock;
mutex g_SnowmanPosLock;
mutex g_BlockInitialPosLock;
mutex g_player_lock;
