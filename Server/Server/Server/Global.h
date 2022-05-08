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
ConnectClient g_connectedCls[3]{ false };

// =======================================

std::vector <Player> phyPlayers;

// =======================================

// �÷��̾���� �ʱ� ��ġ

Pro_Player g_initialPos[3] = { {0,DirectX::XMFLOAT3(200.0f,30.0f,0.0f),0,0,CHARACTER_HUSKY},
	{1,DirectX::XMFLOAT3(400.0f,30.0f,0.0f),0,0,CHARACTER_ARCTICFOX},
	{2,DirectX::XMFLOAT3(600.0f,200.0f,0.0f),0,0,CHARACTER_POLARBEAR} };

PlayerReadyInfo g_playerReadyInfo[3]{ {-1, 0}, {-1, 0}, {-1, 0} };

// =======================================

std::queue <Message> g_MsgQueue;

std::map <char, SOCKET> g_clients;


atomic_int numOfCls;				// ������ Ŭ���̾�Ʈ ��

map<string, DirectX::BoundingBox*> g_boundaries;

bool g_isPlaying = false;

mutex g_MsgQueueLock;
mutex g_ConnectedClsLock;
mutex g_SocketLock;
mutex g_PlayerReadyInfoLock;
mutex g_InitialPosLock;
mutex g_SnowmanPosLock;
mutex g_BlockInitialPosLock;
mutex g_player_lock;
