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

public:
	GameInfo* m_pGameInfo;
};