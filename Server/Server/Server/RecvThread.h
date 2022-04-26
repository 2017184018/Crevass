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

	g_socketLock.lock();
	SOCKET sock = g_clients[id];
	g_socketLock.unlock();

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

			g_socketLock.lock();
			g_clients.erase(id);
			g_socketLock.unlock();

			for (auto& cl : g_clients)
				SendRemovePlayerPacket(cl.first, id);

			g_connectedClsLock.lock();
			g_connectedCls[id].is_connected = false;
			g_connectedClsLock.unlock();
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
		case CS_READY:
		{
			g_playerReadyInfoLock.lock();
			char ready = g_playerReadyInfo[id].ready = ((int)g_playerReadyInfo[id].ready + 1) % 2;
			g_playerReadyInfoLock.unlock();

			for (auto& cl : g_clients)
				SendReadyPacket(cl.first, id, ready);

			if (CheckGameStart())
			{
				SendGameStartPacket();
				g_isPlaying = true;

				// Physics Thread ����
				std::cout << "physics thread ����!" << std::endl;
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