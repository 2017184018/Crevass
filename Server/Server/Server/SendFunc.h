#pragma once
#include "pch.h"
#include "Global.h"

void SendPacket(void* buff)
{
	char* packet = reinterpret_cast<char*> (buff);
	int packet_size = MAKEWORD(packet[0], packet[1]);

	for (auto& cl : g_clients)
		int retval = send(cl.second, packet, packet_size, 0);

}

void SendLoginOkPacket(char id)
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;

	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
	//SendPacket( id, &packet );
}

void SendReadyPacket(char id, char other, char ready)
{
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;

	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
	//SendPacket( id, &packet );
}

void SendGameStartPacket()
{
	std::cout << "Send Game STart!" << std::endl;
	// player에 초기화 좌표를 다 넣어줘야한다
	// 현재 들어온 클라이언트 정보만 넣어서 보내주기.
	g_initialPosLock.lock();
	Pro_Player tempPlayer[3] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]} }; // 일단 복사.
	g_initialPosLock.unlock();

	g_playerReadyInfoLock.lock();
	PlayerReadyInfo tempInfo[3] = { {g_playerReadyInfo[0]},{g_playerReadyInfo[1]},{g_playerReadyInfo[2]} };
	g_playerReadyInfoLock.unlock();
	for (int i = 0; i < numOfCls; ++i) 
	{
		if (tempInfo[i].id == -1) 
		{
			tempPlayer[i].id = -1;
		}
	}

	sc_packet_game_start packet;
	packet.size = sizeof(packet);
	packet.type = SC_GAMESTART;
	memcpy(&packet.players, &tempPlayer, sizeof(tempPlayer));
	SendPacket(&packet);
}

void SendGameOverPacket(char id)
{
	std::cout << "Send Game over" << std::endl;
	sc_packet_gameover packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_GAMEOVER;

	SendPacket(&packet);
}

void SendPos(Pro_Player& players)
{
	sc_packet_pos packet;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	memcpy(&packet.players, &players, sizeof(packet.players));

	SendPacket(&packet);
}

void SendRemovePlayerPacket(char client, char leaver)
{
	sc_packet_remove_player packet;
	packet.id = leaver;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;
	SendPacket(&packet);
}
