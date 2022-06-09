#pragma once
#include "pch.h"
#include "Global.h"
#include <random>

random_device rd1;
default_random_engine dre2(rd1());
uniform_int_distribution<>uid5{ 0,8 };
uniform_int_distribution<>uid6{ 0,24 };
int TempiglooLocation[2];
int TempSnowmanLocation[4];

bool BlockCheck(int idx) {
	if (idx == 0 || idx == 2 || idx == 4 || idx == 10 || idx == 12 || idx == 14 || idx == 20 || idx == 22 || idx == 24)
		return false;
	return true;
}

void SendPacket(void* buff)
{
	char* packet = reinterpret_cast<char*> (buff);
	int packet_size = MAKEWORD(packet[0], packet[1]);
	//int packet_size = packet[0];

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

void SendReadyPenguin(char id, char other, char ready) {
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;
	packet.Character_type = CHARACTER_PENGUIN;
	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
}

void SendReadyHusky(char id, char other, char ready) {
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;
	packet.Character_type = CHARACTER_HUSKY;
	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
}

void SendReadyBear(char id, char other, char ready) {
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;
	packet.Character_type = CHARACTER_POLARBEAR;
	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
}

void SendReadyFox(char id, char other, char ready) {
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;
	packet.Character_type = CHARACTER_ARCTICFOX;
	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
}

void SendReadySeal(char id, char other, char ready) {
	sc_packet_ready packet;
	packet.id = other;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	//mutex 필요
	packet.ready = g_playerReadyInfo[other].ready;
	packet.Character_type = CHARACTER_SEAL;
	int retval = send(g_clients[id], (char*)&packet, sizeof(packet), 0);
}

void SendGameStartPacket()
{
	std::cout << "Send Game Start!" << std::endl;
	// player에 초기화 좌표를 다 넣어줘야한다
	// 현재 들어온 클라이언트 정보만 넣어서 보내주기.
	g_InitialPosLock.lock();
	Pro_Player tempPlayer[3] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]} }; // 일단 복사.
//	cout << static_cast<int>(tempPlayer[1].Character_type) << endl;
	g_InitialPosLock.unlock();

	g_PlayerReadyInfoLock.lock();
	PlayerReadyInfo tempInfo[3] = { {g_playerReadyInfo[0]},{g_playerReadyInfo[1]},{g_playerReadyInfo[2]} };
	g_PlayerReadyInfoLock.unlock();

	g_iglooPosLock.lock();
	for (int i = 0; i < 2; ++i)
	{
		do
		{
			TempiglooLocation[i] = uid5(dre2);
		} while (i == 1 && TempiglooLocation[0] == TempiglooLocation[1]);
	}
	g_iglooPosLock.unlock();

	g_SnowmanPosLock.lock();
	{
		do
		{
			TempSnowmanLocation[0] = uid6(dre2);
		} while (!BlockCheck(TempSnowmanLocation[0]));
		
		do
		{
			TempSnowmanLocation[1] = uid6(dre2);
		} while (TempSnowmanLocation[0] == TempSnowmanLocation[1] || !BlockCheck(TempSnowmanLocation[1]));

		do
		{
			TempSnowmanLocation[2] = uid6(dre2);
		} while (TempSnowmanLocation[0] == TempSnowmanLocation[2] || TempSnowmanLocation[1] == TempSnowmanLocation[2] || !BlockCheck(TempSnowmanLocation[2]));

		do
		{
			TempSnowmanLocation[3] = uid6(dre2);
		} while (TempSnowmanLocation[0] == TempSnowmanLocation[3] || TempSnowmanLocation[1] == TempSnowmanLocation[3] || 
			TempSnowmanLocation[2] == TempSnowmanLocation[3] || !BlockCheck(TempSnowmanLocation[3]));
	}
	g_SnowmanPosLock.unlock();

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
	for (int i = 0; i < 2; ++i)
		packet.iglooLocation[i] = TempiglooLocation[i];

	for (int i = 0; i < 4; ++i)
		packet.SnowmanLocation[i] = TempSnowmanLocation[i];

	g_BlockInitialPosLock.lock();
	for (int i = 0; i < 25; ++i) {
		packet.blocks[i].id = i;
		packet.blocks[i].destuctioncnt = 0;
		packet.blocks[i].pos.x = i / 5 * 200;
		packet.blocks[i].pos.y = -30.0;
		packet.blocks[i].pos.z = i % 5 * 200;
	}
	g_BlockInitialPosLock.unlock();

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

void SendAnim(Pro_Player& players)
{
	sc_packet_anim packet;
	packet.size = sizeof(packet);
	packet.type = SC_ANIM;
	memcpy(&packet.players, &players, sizeof(packet.players));

	SendPacket(&packet);
}

void SendFall(int client)
{
	sc_packet_fall packet;
	packet.size = sizeof(packet);
	packet.type = SC_FALL;
	packet.id = client;

	SendPacket(&packet);
}

void SendReset(int client)
{
	sc_packet_reset packet;
	packet.size = sizeof(packet);
	packet.type = SC_RESET;
	packet.id = client;

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

void SendBlockPacket(Block& blocks) {
	sc_packet_block packet;
	packet.size = sizeof(packet);
	packet.type = SC_BLOCK;
	memcpy(&packet.blocks, &blocks, sizeof(packet.blocks));

	SendPacket(&packet);
}

void SendTime(int time)
{
	sc_packet_time packet;
	packet.size = sizeof(packet);
	packet.type = SC_TIME;
	packet.time = time;

	SendPacket(&packet);
}