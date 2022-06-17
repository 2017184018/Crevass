#include "pch.h"
#include "Global.h"
#include "RecvThread.h"
#include <string>

void ErrQuit(const char* msg);
void ErrDisplay(const char* msg);

void LoadBBs(std::string filepath) {
	std::ifstream in(filepath);

	while (!in.eof()) {
		DirectX::BoundingBox* BB = new DirectX::BoundingBox;
		string meshname;
		in >> meshname >> BB->Extents.x >> BB->Extents.y >> BB->Extents.z;

		float tmpx = BB->Extents.x;
		float tmpy = BB->Extents.y;
		float tmpz = BB->Extents.z;
		if (meshname == "icecube" || meshname == "snowcube") {
			for (int i = 0; i < 25; ++i) {
				g_boundaries[meshname + std::to_string(i)] = BB;
				BB = new DirectX::BoundingBox;
				BB->Extents.x = tmpx;
				BB->Extents.y = tmpy;
				BB->Extents.z = tmpz;
			}
		}
		else if (meshname == "igloo") {
			for (int i = 0; i < 2; ++i) {
				g_boundaries[meshname + std::to_string(i)] = BB;
				BB = new DirectX::BoundingBox;
				BB->Extents.x = tmpx;
				BB->Extents.y = tmpy;
				BB->Extents.z = tmpz;
			}
		}
		else if (meshname == "snowman") {
			for (int i = 0; i < 4; ++i) {
				g_boundaries[meshname + std::to_string(i)] = BB;
				BB = new DirectX::BoundingBox;
				BB->Extents.x = tmpx;
				BB->Extents.y = tmpy;
				BB->Extents.z = tmpz;
			}
		}
		else if (meshname == "rock_1") {
			for (int i = 0; i < 5; ++i) {
				g_boundaries["hail" + std::to_string(i)] = BB;
				BB = new DirectX::BoundingBox;
				BB->Extents.x = tmpx * 3;
				BB->Extents.y = tmpy * 3;
				BB->Extents.z = tmpz * 3;
			}
		}
		else {
			g_boundaries[meshname] = BB;
			//cout << meshname <<": " << g_boundaries[meshname]->Extents.x << ", " << g_boundaries[meshname]->Extents.y << ", "
			//	<< g_boundaries[meshname]->Extents.z << endl;
		}
	}
}

int main()
{
	LoadBBs("BB.txt");
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
			g_ConnectedClsLock.lock();
			if (false == g_connectedCls[i].is_connected) {
				g_connectedCls[i].is_connected = true;
				g_ConnectedClsLock.unlock();
				user_id = i;
				numOfCls++;
				cout << "==============================" << endl;
				cout << "ID " << i << " is connected" << endl;
				cout << "==============================" << endl;

				break;
			}
			g_ConnectedClsLock.unlock();
		}

		g_SocketLock.lock();
		g_clients[user_id] = client_sock;
		g_SocketLock.unlock();

		g_PlayerReadyInfoLock.lock();
		g_playerReadyInfo[user_id].id = user_id;
		g_playerReadyInfo[user_id].ready = 0;		// �ϴ��� false��
		g_PlayerReadyInfoLock.unlock();

		// ������ Ŭ���̾�Ʈ�� id �˷��ֱ�
		SendLoginOkPacket(user_id);


		//������ �������� �����Ȳ
		//������ Ŭ�󿡰� ���� �����Ȳ
		for (int i = 0; i < MAXPLAYER; ++i)	// �������̵� ������� ������ ���� ����
		{
			g_ConnectedClsLock.lock();
			if (true == g_connectedCls[i].is_connected)
			{
				g_ConnectedClsLock.unlock();
				g_PlayerReadyInfoLock.lock();
				SendReadyPacket(user_id, i, g_playerReadyInfo[i].ready);	//mutex �ʿ�
				g_PlayerReadyInfoLock.unlock();
				SendReadyPacket(i, user_id, 0);	//0���� ���� mutex�Ⱦ�����
				g_ConnectedClsLock.lock();
			}
			g_ConnectedClsLock.unlock();
		}
		// RecvThread ����
		thread RecvThread(Receiver, user_id);
		RecvThread.detach();

	}


}