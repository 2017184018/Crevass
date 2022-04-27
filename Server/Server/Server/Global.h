#pragma once
#include "Player.h"
#include <DirectXMath.h>
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

Pro_Player g_initialPos[3] = { {0,DirectX::XMFLOAT3(200.0f,30.0f,0.0f)},{1,DirectX::XMFLOAT3(300.0f,30.0f,0.0f)},{2,DirectX::XMFLOAT3(200.0f,200.0f,0.0f)} };

PlayerReadyInfo g_playerReadyInfo[3]{ {-1, 0}, {-1, 0}, {-1, 0} };

// =======================================

std::queue <Message> g_MsgQueue;

std::map <char, SOCKET> g_clients;


atomic_int numOfCls;				// ������ Ŭ���̾�Ʈ ��

bool g_isPlaying = false;

mutex g_MsgQueueLock;
mutex g_connectedClsLock;
mutex g_socketLock;
mutex g_playerReadyInfoLock;
mutex g_initialPosLock;