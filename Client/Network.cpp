#include "Network.h"
#include "GameInfo.h"
#include "PlayerInfo.h"
#include "CREVASS.h"

namespace Core
{
	extern std::map<int, Character*> m_Users;
}

void Network::InitSocket()
{
	// 논블록 소켓 생성
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ErrorQuit("error wsa Init");
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ClientSocket == INVALID_SOCKET) ErrorQuit("socket()");
	ULONG on = 1;
	BOOL optval = TRUE;
	setsockopt(m_ClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	ioctlsocket(m_ClientSocket, FIONBIO, &on);	//nonblock
	std::cout << "클라이언트 소켓 생성" << std::endl;

	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
}

void Network::Connect(/*std::string* ipAddr*/)
{
	// IP입력하고 Enter 버튼 눌렀을시 -> Connect()
	serveraddr.sin_addr.s_addr = inet_addr(/*ipAddr->c_str()*/SERVERIP);
	m_Retval = connect(m_ClientSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (m_Retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//std::cout << " error connect " << std::endl;
		}
	}
}

void Network::Recv()
{
	int retval;

	char buf[BUFSIZE];
	retval = recv(m_ClientSocket, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return;
		}
		//ErrorDisplay( "recv()!" );
		return;
	}
	else if (retval == 0) {
		std::cout << "SERVER FULL! CAN'T CONNECT!" << std::endl;
		PostQuitMessage(0);
		return;
	}
	else
		ProcessData(buf, retval);

}

void Network::Send(char buf)
{
	send(m_ClientSocket, &buf, sizeof(char), 0);
}

void Network::ProcessData(char* buf, size_t io_byte)
{
	char* ptr = buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUFSIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = MAKEWORD(ptr[0], ptr[1]); // 처음받은 메시지 크기를 저장
		if (io_byte + saved_packet_size >= in_packet_size) {   // 처음받은 메시지 크기보다 새로 받은 바이트 수 + 저장하고 있던 바이트 수가 크면 processPacket 호출, 넘겨준 바이트 만큼 버퍼 포인터, 받은 바이트 수 갱신
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);   // 받은 것 만큼만 임시 저장 버퍼에 담아두기.
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
	return;
}

/*===============================================================*/
void Network::ProcessPacket(char* packet_buffer)
{
	ptr = packet_buffer;

	switch (ptr[2])
	{
	case SC_LOGIN_OK:
	{
		std::cout << "login ok" << std::endl;
		sc_packet_login_ok packet;
		memcpy(&packet, ptr, sizeof(packet));
		//Info::GetInstance()->m_IsConnect = true;
		//Info::GetInstance()->m_ClientID = static_cast<int>(packet.id);
		//Info::GetInstance()->m_ClientsNum += 1;
		//Info::GetInstance()->CreatePlayerInfo(packet.id, true);
		m_pGameInfo->m_IsConnect = true;
		m_pGameInfo->m_ClientID = static_cast<int>(packet.id);
		//	m_pGameInfo->m_ClientsNum += 1;
		m_pGameInfo->CreatePlayerInfo(packet.id, true);

		break;
	}
	case SC_READY:
	{
		sc_packet_ready packet;
		memcpy(&packet, ptr, sizeof(packet));
		if (packet.id != -1)
		{
			//if (Info::GetInstance()->m_PlayersInfo[static_cast<int>(packet.id)])
			//{
			//	Info::GetInstance()->m_PlayersInfo[static_cast<int>(packet.id)]->SetReadyState(packet.ready);
			//}
			//else
			//{
			//	Info::GetInstance()->CreatePlayerInfo(packet.id, false);
			//	Info::GetInstance()->m_PlayersInfo[static_cast<int>(packet.id)]->SetReadyState(packet.ready);
			//}

			if (m_pGameInfo->m_PlayersInfo[static_cast<int>(packet.id)])
			{
				m_pGameInfo->m_PlayersInfo[static_cast<int>(packet.id)]->SetReadyState(packet.ready);
			}
			else
			{
				m_pGameInfo->CreatePlayerInfo(packet.id, false);
				m_pGameInfo->m_PlayersInfo[static_cast<int>(packet.id)]->SetReadyState(packet.ready);

			}
		}

		break;
	}
	case SC_GAMESTART:
	{

		std::cout << "game start" << std::endl;
		sc_packet_game_start packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_pGameInfo->m_GameStart = true;
		//for (int i = 0; i < TOTAL_PLAYER_NUM; ++i)
		//{
		//	if (packet.players[i].id != -1)
		//	{
		//		if (m_pGameInfo->m_PlayersInfo[i] != nullptr)
		//		{
		//			if (packet.players[i].posX != NULL && packet.players[i].posY != NULL)
		//			{
		//				Info::GetInstance()->m_PlayersInfo[static_cast<int>(packet.players[i].id)]->SetPosition(packet.players[i].posX, packet.players[i].posY);
		//			}
		//		}
		//	}
		//}
		PlayerPos = packet.players[m_pGameInfo->m_ClientID].pos;
		printf("x=%f, y=%f, z=%f\n", packet.players[0].pos.x, packet.players[0].pos.y, packet.players[0].pos.z);
		break;
	}
	case SC_POS:
	{
		sc_packet_pos packet;
		memcpy(&packet, ptr, sizeof(packet));
		PlayerPos = packet.players[m_pGameInfo->m_ClientID].pos;
		//for (int i = 0; i < 3; ++i)
		//{
		//	if (packet.players[i].id != -1)
		//	{
		//		if (Info::GetInstance()->m_PlayersInfo[i] != nullptr)
		//		{
		//			if (packet.players[i].posX != NULL && packet.players[i].posY != NULL)
		//			{
		//				Info::GetInstance()->m_PlayersInfo[static_cast<int>(packet.players[i].id)]->SetPosition(packet.players[i].posX, packet.players[i].posY);
		//			}
		//		}
		//	}
		//}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		std::cout << "remove player" << std::endl;
		sc_packet_remove_player packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_pGameInfo->DeletePlayerInfo(packet.id);
		break;
	}
	case SC_GAMEOVER:
	{
		std::cout << "game over" << std::endl;
		sc_packet_gameover packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_pGameInfo->m_GameStart = false;
		m_pGameInfo->m_WinnerID = static_cast<int>(packet.id);
		break;
	}
	default:
		std::cout << "None Receive Packet" << std::endl;
		break;
	}
	return;
}

/*===============================================================*/





void Network::ErrorQuit(const char* msg)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (WCHAR*)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void Network::ErrorDisplay(const char* msg)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	std::cout << "Error! - " << msg << "descripton: " << lpMsgBuf << std::endl;
}

DirectX::XMFLOAT3 Network::GetPlayerPos()
{
	return PlayerPos;
}
