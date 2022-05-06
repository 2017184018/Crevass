#pragma once
#include "headers.h"
#include "GameInfo.h"
#define SERVERIP "127.0.0.1"
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

	char* ptr;

	DirectX::XMFLOAT3 GetPlayerPos(int num)const;
	DirectX::XMFLOAT3 GetBlockPos(int num)const;
	int GetSnowmanLocation(int num)const;
	int GetPlayerDir(int num)const;
	int GetBlockDestructionCnt(int num) const;

	int GetPlayerAnim(int num)const;

private:
	DirectX::XMFLOAT3 PlayerPos[3];
	int PlayerAnim[3];
	int SnowmanLocation[2];
	char dir[3];

	DirectX::XMFLOAT3 BlockPos[25];
	int BlockDestructionCnt[25];
public:
	GameInfo* m_pGameInfo;
};