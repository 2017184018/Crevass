#pragma once
#include "pch.h"
#include "Global.h"
#include "SendFunc.h"
#include "UtilFunc.h"

void Receiver(char id)
{
	// RecvThread 스레드 함수

	// RecvThread의 thread 함수 입니다.
	// 클라이언트로 부터 받은 패킷의 타입을 구분합니다.
	// cs_packet_ready은 함수 내부에서 처리합니다.
	// 그외의 cs 패킷은 Message 변수에 값을 넣어 MsgQueue에 push 합니다.
	// sc_gamestart_packet을 보내면 PhysicThread를 만듭니다.

	std::cout << "RecvThread is operating!" << std::endl;

	Message msg{};

	g_SocketLock.lock();
	SOCKET sock = g_clients[id];
	g_SocketLock.unlock();

	while (true)
	{
		// ID는 0부터 시작이므로 -1로 초기화 합니다.
		msg.id = -1;
		char buf[BUFSIZE];
		char retval = recv(sock, buf, 1, 0);

		// 클라이언트 접속 종료, recv 에러 처리
		if (retval == 0 || retval == SOCKET_ERROR)
		{
			closesocket(sock);

			g_SocketLock.lock();
			g_clients.erase(id);
			g_SocketLock.unlock();

			for (auto& cl : g_clients)
				SendRemovePlayerPacket(cl.first, id);

			g_ConnectedClsLock.lock();
			g_connectedCls[id].is_connected = false;
			g_ConnectedClsLock.unlock();
			--numOfCls;

			cout << "======================================================" << endl;
			cout << "ID " << (int)id << " is out. And this Id slot is empty" << endl;
			cout << "======================================================" << endl;

			if (retval == SOCKET_ERROR)
				ErrDisplay("RecvThread occured Error!");
			return;

		}
		// Message 재사용을 위한 초기화
		ZeroMemory(&msg, sizeof(Message));
		msg.id = -1;
		switch ((int)buf[0])
		{

		case CS_PLAYER_UP_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP;
			msg.isPushed = true;
			//printf("%d updown\n", msg.id);
			break;
		}
		case CS_PLAYER_UP_LEFT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP_LEFT;
			msg.isPushed = true;
			break;
		}
		case CS_PLAYER_UP_RIGHT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP_RIGHT;
			msg.isPushed = true;
			break;
		}

		case CS_PLAYER_DOWN_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN;
			msg.isPushed = true;

			break;
		}

		case CS_PLAYER_DOWN_LEFT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN_LEFT;
			msg.isPushed = true;
			break;
		}

		case CS_PLAYER_DOWN_RIGHT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN_RIGHT;
			msg.isPushed = true;
			break;
		}
		case CS_PLAYER_LEFT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_LEFT;
			msg.isPushed = true;
			//printf("%d leftdown\n",msg.id);

			break;
		}
		case CS_PLAYER_RIGHT_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_RIGHT;
			msg.isPushed = true;
			//printf("%d rightdown\n",msg.id);
			break;
		}
		case CS_PLAYER_UP_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP;
			msg.isPushed = false;
			//printf("%d upup\n",msg.id);

			break;
		}
		case CS_PLAYER_UP_LEFT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP_LEFT;
			msg.isPushed = false;
			//printf("%d upup\n",msg.id);

			break;
		}

		case CS_PLAYER_UP_RIGHT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_UP_RIGHT;
			msg.isPushed = false;
			//printf("%d upup\n",msg.id);

			break;
		}



		case CS_PLAYER_DOWN_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN;
			msg.isPushed = false;
			//printf("%d downup\n",msg.id);

			break;
		}

		case CS_PLAYER_DOWN_LEFT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN_LEFT;
			msg.isPushed = false;
			//printf("%d downup\n",msg.id);

			break;
		}

		case CS_PLAYER_DOWN_RIGHT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN_RIGHT;
			msg.isPushed = false;
			//printf("%d downup\n",msg.id);

			break;
		}


		case CS_PLAYER_LEFT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_LEFT;
			msg.isPushed = false;
			//printf("%d leftup\n",msg.id);

			break;
		}
		case CS_PLAYER_RIGHT_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_RIGHT;
			msg.isPushed = false;
			break;
		}



		case CS_READY:
		{
			g_PlayerReadyInfoLock.lock();
			char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			for (auto& cl : g_clients)
				SendReadyPacket(cl.first, id, ready);

			if (CheckGameStart())
			{
				SendGameStartPacket();
				g_isPlaying = true;

				// Physics Thread 생성
				std::cout << "physics thread 생성!" << std::endl;
				thread PhysicsThread(ProcessClients);
				PhysicsThread.detach();
			}
			break;
		}
		default:
			cout << "Packet Type Error! - " << buf[0] << endl;
			while (true);
		}

		if (msg.id != -1)
		{
			g_MsgQueueLock.lock();
			g_MsgQueue.emplace(msg);
			g_MsgQueueLock.unlock();
		}
	}
}