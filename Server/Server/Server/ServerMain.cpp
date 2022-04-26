#include "pch.h"
#include "Global.h"
#include "RecvThread.h"

void ErrQuit(const char* msg);
void ErrDisplay(const char* msg);

int main()
{
	wcout.imbue(locale("korean")); // �ѱ���� ���� 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) ErrQuit("socket() - ��� ����");

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_family = AF_INET;
	int retval = ::bind(listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) ErrQuit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) ErrQuit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	ZeroMemory(&clientAddr, addrlen);

	int user_id{ -1 };

	while (true)
	{
		client_sock = accept(listen_sock, (SOCKADDR*)&clientAddr, &addrlen);
		if (client_sock == INVALID_SOCKET) ErrDisplay("accept()");
			
		// 3���� �̹� ������ ������ ������, 4��°�� ������ ��û�ϴ� Ŭ���̾�Ʈ���� ������ ������� ����. 
		if (numOfCls == MAXPLAYER || g_isPlaying == true) {
			closesocket(client_sock);
			continue;
		}

		for (int i = 0; i < MAXPLAYER; ++i) 
		{
			g_connectedClsLock.lock();
			if (false == g_connectedCls[i].is_connected) {
				g_connectedCls[i].is_connected = true;
				g_connectedClsLock.unlock();
				user_id = i;
				numOfCls++;
				cout << "==============================" << endl;
				cout << "ID " << i << " is connected" << endl;
				cout << "==============================" << endl;

				break;
			}
			g_connectedClsLock.unlock();
		}

		g_socketLock.lock();
		g_clients[user_id] = client_sock;
		g_socketLock.unlock();

		g_playerReadyInfoLock.lock();
		g_playerReadyInfo[user_id].id = user_id;
		g_playerReadyInfo[user_id].ready = 0;		// �ϴ��� false��
		g_playerReadyInfoLock.unlock();

		// ������ Ŭ���̾�Ʈ�� id �˷��ֱ�
		SendLoginOkPacket(user_id);


		//������ �������� �����Ȳ
		//������ Ŭ�󿡰� ���� �����Ȳ
		for (int i = 0; i < MAXPLAYER; ++i)	// �������̵� ������� ������ ���� ����
		{
			g_connectedClsLock.lock();
			if (true == g_connectedCls[i].is_connected)
			{
				g_connectedClsLock.unlock();
				g_playerReadyInfoLock.lock();
				SendReadyPacket(user_id, i, g_playerReadyInfo[i].ready);	//mutex �ʿ�
				g_playerReadyInfoLock.unlock();
				SendReadyPacket(i, user_id, 0);	//0���� ���� mutex�Ⱦ�����
				g_connectedClsLock.lock();
			}
			g_connectedClsLock.unlock();
		}
		// RecvThread ����
		thread RecvThread(Receiver, user_id);
		RecvThread.detach();

	}


}