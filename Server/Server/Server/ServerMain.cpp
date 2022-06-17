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
	wcout.imbue(locale("korean")); // 한국어로 세팅 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) ErrQuit("socket() - 대기 소켓");

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

		// 3인이 이미 서버에 접속해 있으면, 4번째로 접속을 요청하는 클라이언트부터 접속을 허용하지 않음. 
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
		g_playerReadyInfo[user_id].ready = 0;		// 일단은 false로
		g_PlayerReadyInfoLock.unlock();

		// 접속한 클라이언트에 id 알려주기
		SendLoginOkPacket(user_id);


		//나에게 나머지의 레디상황
		//나머지 클라에게 나의 레디상황
		for (int i = 0; i < MAXPLAYER; ++i)	// 유저아이디가 순서대로 증가할 때만 가능
		{
			g_ConnectedClsLock.lock();
			if (true == g_connectedCls[i].is_connected)
			{
				g_ConnectedClsLock.unlock();
				g_PlayerReadyInfoLock.lock();
				SendReadyPacket(user_id, i, g_playerReadyInfo[i].ready);	//mutex 필요
				g_PlayerReadyInfoLock.unlock();
				SendReadyPacket(i, user_id, 0);	//0넣은 이유 mutex안쓰려고
				g_ConnectedClsLock.lock();
			}
			g_ConnectedClsLock.unlock();
		}
		// RecvThread 생성
		thread RecvThread(Receiver, user_id);
		RecvThread.detach();

	}


}