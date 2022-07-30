#pragma once
#include "pch.h"
#include "Global.h"
#include "SendFunc.h"
#include "UtilFunc.h"

void Receiver(char id)
{
	// RecvThread ������ �Լ�

	// RecvThread�� thread �Լ� �Դϴ�.
	// Ŭ���̾�Ʈ�� ���� ���� ��Ŷ�� Ÿ���� �����մϴ�.
	// cs_packet_ready�� �Լ� ���ο��� ó���մϴ�.
	// �׿��� cs ��Ŷ�� Message ������ ���� �־� MsgQueue�� push �մϴ�.
	// sc_gamestart_packet�� ������ PhysicThread�� ����ϴ�.

	std::cout << "RecvThread is operating!" << std::endl;

	Message msg{};

	g_SocketLock.lock();
	SOCKET sock = g_clients[id];
	g_SocketLock.unlock();

	while (true)
	{
		// ID�� 0���� �����̹Ƿ� -1�� �ʱ�ȭ �մϴ�.
		msg.id = -1;
		char buf[BUFSIZE];
		char retval = recv(sock, buf, 1, 0);

		// Ŭ���̾�Ʈ ���� ����, recv ���� ó��
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
		// Message ������ ���� �ʱ�ȭ
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
		case CS_PLAYER_DOWN_DOWN:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN;
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



		case CS_PLAYER_DOWN_UP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = DIR_DOWN;
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
		case CS_PLAYER_IDLE:
		{
			msg.id = id;
			msg.type = TYPE_ANIMATION;
			msg.AnimType = ANIM_IDLE;
			break;
		}


		case CS_PLAYER_MOVE:
		{
			msg.id = id;
			msg.type = TYPE_ANIMATION;
			msg.AnimType = ANIM_MOVE;
			break;
		}


		case CS_PLAYER_ATTACK:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = KEY_ATTACK;
			break;
		}
		case CS_PLAYER_JUMP:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = KEY_JUMP;
			break;
		}
		case CS_PLAYER_INTERACT:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = KEY_INTERACT;
			break;
		}
		case CS_PLAYER_SKILL:
		{
			msg.id = id;
			msg.type = TYPE_PLAYER;
			msg.dir = KEY_SKILL;
			break;
		}
		case CS_READY_PENGUIN:
		{
			bool OverlapCheck = false;
			g_PlayerReadyInfoLock.lock();
			for (int i = 0; i < numOfCls; ++i)
			{
				if (g_initialPos[i].Character_type == CHARACTER_PENGUIN)
				{
					std::cout << "can't ready penguin" << std::endl;
					OverlapCheck = true;
					break;
				}
			}

			//char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			if (OverlapCheck == false)
			{

				if ((int)g_playerReadyInfo[id].ready == 0)
				{
					g_initialPos[id].Character_type = CHARACTER_PENGUIN;
				}
				else
				{
					g_initialPos[id].Character_type = -1;
				}

				char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;

				for (auto& cl : g_clients)
					SendReadyPenguin(cl.first, id, ready);

				if (CheckGameStart())
				{

					SendGameStartPacket();
					g_isPlaying = true;
					// Physics Thread ����
					std::cout << "physics thread ����!" << std::endl;
					thread PhysicsThread(ProcessClients);
					PhysicsThread.detach();
				}

			}

			break;
		}
		case CS_READY_HUSKY:
		{
			bool OverlapCheck = false;
			g_PlayerReadyInfoLock.lock();
			for (int i = 0; i < numOfCls; ++i)
			{
				if (g_initialPos[i].Character_type == CHARACTER_HUSKY)
				{
					std::cout << "can't ready husky" << std::endl;
					OverlapCheck = true;
					break;
				}
			}

			//char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			if (OverlapCheck == false)
			{
				if ((int)g_playerReadyInfo[id].ready == 0)
				{
					g_initialPos[id].Character_type = CHARACTER_HUSKY;
				}
				else
				{
					g_initialPos[id].Character_type = -1;
				}

				char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
				for (auto& cl : g_clients)
					SendReadyHusky(cl.first, id, ready);
				if (CheckGameStart())
				{

					SendGameStartPacket();
					g_isPlaying = true;
					// Physics Thread ����
					std::cout << "physics thread ����!" << std::endl;
					thread PhysicsThread(ProcessClients);
					PhysicsThread.detach();
				}

				//g_initialPos[id].Character_type = CHARACTER_HUSKY;
			}
			break;
		}
		case CS_READY_POLARBEAR:
		{
			bool OverlapCheck = false;
			g_PlayerReadyInfoLock.lock();
			for (int i = 0; i < numOfCls; ++i)
			{
				if (g_initialPos[i].Character_type == CHARACTER_POLARBEAR)
				{
					std::cout << "can't ready polarbear" << std::endl;
					OverlapCheck = true;
					break;
				}
			}

			//char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			if (OverlapCheck == false)
			{
				if ((int)g_playerReadyInfo[id].ready == 0)
				{
					g_initialPos[id].Character_type = CHARACTER_POLARBEAR;
				}
				else
				{
					g_initialPos[id].Character_type = -1;
				}

				char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
				for (auto& cl : g_clients)
					SendReadyBear(cl.first, id, ready);
				if (CheckGameStart())
				{

					SendGameStartPacket();
					g_isPlaying = true;
					// Physics Thread ����
					std::cout << "physics thread ����!" << std::endl;
					thread PhysicsThread(ProcessClients);
					PhysicsThread.detach();
				}

				//g_initialPos[id].Character_type = CHARACTER_POLARBEAR;
			}
			break;
		}
		case CS_READY_FOX:
		{
			bool OverlapCheck = false;
			g_PlayerReadyInfoLock.lock();
			for (int i = 0; i < numOfCls; ++i)
			{
				if (g_initialPos[i].Character_type == CHARACTER_ARCTICFOX)
				{
					std::cout << "can't ready fox" << std::endl;
					OverlapCheck = true;
					break;
				}
			}

			//char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			if (OverlapCheck == false)
			{
				if ((int)g_playerReadyInfo[id].ready == 0)
				{
					g_initialPos[id].Character_type = CHARACTER_ARCTICFOX;
				}
				else
				{
					g_initialPos[id].Character_type = -1;
				}

				char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
				for (auto& cl : g_clients)
					SendReadyFox(cl.first, id, ready);
				if (CheckGameStart())
				{

					SendGameStartPacket();
					g_isPlaying = true;
					// Physics Thread ����
					std::cout << "physics thread ����!" << std::endl;
					thread PhysicsThread(ProcessClients);
					PhysicsThread.detach();
				}

				//g_initialPos[id].Character_type = CHARACTER_ARCTICFOX;
			}
			break;
		}
		case CS_READY_SEAL:
		{
			bool OverlapCheck = false;
			g_PlayerReadyInfoLock.lock();
			for (int i = 0; i < numOfCls; ++i)
			{
				if (g_initialPos[i].Character_type == CHARACTER_SEAL)
				{
					std::cout << "can't ready seal" << std::endl;
					OverlapCheck = true;
					break;
				}
			}

			//char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_PlayerReadyInfoLock.unlock();

			if (OverlapCheck == false)
			{
				if ((int)g_playerReadyInfo[id].ready == 0)
				{
					g_initialPos[id].Character_type = CHARACTER_SEAL;
				}
				else
				{
					g_initialPos[id].Character_type = -1;
				}

				char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
				for (auto& cl : g_clients)
					SendReadySeal(cl.first, id, ready);
				if (CheckGameStart())
				{

					SendGameStartPacket();
					g_isPlaying = true;
					// Physics Thread ����
					std::cout << "physics thread ����!" << std::endl;
					thread PhysicsThread(ProcessClients);
					PhysicsThread.detach();
				}

				//g_initialPos[id].Character_type = CHARACTER_SEAL;
			}
			break;
		}

		case CS_CHOOSE_HUSKY:
		{
			SendChooseHusky(id);
			break;
		}
		case CS_CHOOSE_PENGUIN:
		{
			SendChoosePenguin(id);
			break;
		}

		case CS_CHOOSE_FOX:
		{
			SendChooseFox(id);
			break;
		}

		case CS_CHOOSE_POLARBEAR:
		{
			SendChooseBear(id);
			break;
		}

		case CS_CHOOSE_SEAL:
		{
			SendChooseSeal(id);
			break;
		}


		//case CS_PLAYER_LOSE:
		//{
		//	who_lose[lose_count] = id;
		//	lose_count += 1;
		//	//SendGameOverPacket(id);
		//	break;
		//}
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

		//if (CheckGameStart())
		//{

		//	SendGameStartPacket();
		//	g_isPlaying = true;
		//	// Physics Thread ����
		//	std::cout << "physics thread ����!" << std::endl;
		//	thread PhysicsThread(ProcessClients);
		//	PhysicsThread.detach();
		//}

	}
}