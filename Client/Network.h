#pragma once
#include "headers.h"
#include "GameInfo.h"
#define SERVERIP "127.0.0.1"
//#define SERVERIP "192.168.35.175"


class Network
{
public:
	//Network();
	//~Network();

public:
	void	Connect(/*std::string* ipAddr*/);
	void	Recv();
	void	Send(char buf);
	void	ProcessData(char* buf, size_t io_byte);
	void	ProcessPacket(char* packet_buffer);
	void	InitSocket();

public:

public:
	void	ErrorQuit(const char* msg);
	void	ErrorDisplay(const char* msg);

public:
	SOCKET m_ClientSocket;
	SOCKADDR_IN serveraddr;

	int		m_Retval;
	char	m_Buffer[BUFSIZE];
	int m_timer;
	char* ptr;

	DirectX::XMFLOAT3 GetPlayerPos(int num)const;
	DirectX::XMFLOAT3 GetBlockPos(int num)const;
	DirectX::XMFLOAT3 GetHailPos(int num)const;
	int GetiglooLocation(int num)const;
	int GetSnowmanLocation(int num)const;
	int GetPlayerDir(int num)const;
	int GetBlockDestructionCnt(int num) const;

	int GetPlayerAnim(int num)const;
	char GetCharacterType(int num)const;

	bool GetCharacterFall(int num)const;
	void SetCharacterFall(int num);

	bool GetCharacterReset(int num)const;
	void SetCharacterReset(int num);

	bool GetPlayerHide(int num)const;
	int GetPlayerSnowmanNum(int num)const;

	int Gettime()const;
private:
	DirectX::XMFLOAT3 PlayerPos[3];
	int PlayerAnim[3];
	char Player_Type[3];
	bool PlayerHide[3];
	int PlayerSnowmanHide[3];

	int iglooLocation[2];
	int SnowmanLocation[4];
	char dir[3];

	DirectX::XMFLOAT3 BlockPos[25];
	int BlockDestructionCnt[25];

	char CharacterType[3];

	bool IsFall[3]{ false,false,false };
	bool isReset[3]{ false, false, false };

	DirectX::XMFLOAT3 HailPos[5];
public:
	GameInfo* m_pGameInfo;
};