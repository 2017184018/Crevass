#include "Network.h"
#include "GameInfo.h"
#include "PlayerInfo.h"
#include "CREVASS.h"
#include "SceneManager.h"
#include "Character.h"

#include "MainFramework.h"

#include "ApplicationContext.h"
#include "SoundManager.h"
namespace Core
{
	extern std::map<int, Character*> m_Users;
}

using namespace Core;

void Network::InitSocket()
{
	// ������ ���� ����
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) ErrorQuit("error wsa Init");
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ClientSocket == INVALID_SOCKET) ErrorQuit("socket()");
	ULONG on = 1;
	BOOL optval = TRUE;
	setsockopt(m_ClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	ioctlsocket(m_ClientSocket, FIONBIO, &on);	//nonblock
	

	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
}

void Network::Connect(/*std::string* ipAddr*/)
{
	// IP�Է��ϰ� Enter ��ư �������� -> Connect()
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
		if (0 == in_packet_size) in_packet_size = MAKEWORD(ptr[0], ptr[1]); // ó������ �޽��� ũ�⸦ ����
		if (io_byte + saved_packet_size >= in_packet_size) {   // ó������ �޽��� ũ�⺸�� ���� ���� ����Ʈ �� + �����ϰ� �ִ� ����Ʈ ���� ũ�� processPacket ȣ��, �Ѱ��� ����Ʈ ��ŭ ���� ������, ���� ����Ʈ �� ����
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);   // ���� �� ��ŭ�� �ӽ� ���� ���ۿ� ��Ƶα�.
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
			CharacterType[packet.id] = packet.Character_type;
		}

		break;
	}

	case SC_CHOOSE_HUSKY:
	{
		sc_packet_choose packet;
		memcpy(&packet, ptr, sizeof(packet));
		choosename[(int)packet.id] = 0;
		cout << (int)packet.id << endl;
		cout << (int)packet.type << endl;
		break;
	}
	case SC_CHOOSE_PENGUIN:
	{
		sc_packet_choose packet;
		memcpy(&packet, ptr, sizeof(packet));
		choosename[(int)packet.id] = 1;
		cout << (int)packet.id << endl;
		cout << (int)packet.type << endl;
		break;
	}

	case SC_CHOOSE_FOX:
	{
		sc_packet_choose packet;
		memcpy(&packet, ptr, sizeof(packet));
		choosename[(int)packet.id] = 2;
		cout << (int)packet.id << endl;
		cout << (int)packet.type << endl;
		break;
	}

	case SC_CHOOSE_POLARBEAR:
	{
		sc_packet_choose packet;
		memcpy(&packet, ptr, sizeof(packet));
		choosename[(int)packet.id] = 3;
		cout << (int)packet.id << endl;
		cout << (int)packet.type << endl;
		break;
	}	
	case SC_CHOOSE_SEAL:
	{
		sc_packet_choose packet;
		memcpy(&packet, ptr, sizeof(packet));
		choosename[(int)packet.id] = 4;
		cout << (int)packet.id << endl;
		cout << (int)packet.type << endl;
		break;
	}


	case SC_GAMESTART:
	{

		std::cout << "game start" << std::endl;
		sc_packet_game_start packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_pGameInfo->m_GameStart = true;
	
		for (int i = 0; i < TOTAL_PLAYER_NUM; ++i)
		{
			if (packet.players[i].id != -1)
			{
				if (m_pGameInfo->m_PlayersInfo[i] != nullptr)
				{
					if (packet.players[i].pos.x != NULL && packet.players[i].pos.y != NULL && packet.players->pos.z != NULL)
					{
						m_pGameInfo->m_PlayersInfo[static_cast<int>(packet.players[i].id)]->SetPosition(packet.players->pos);
						m_pGameInfo->m_PlayersInfo[static_cast<int>(packet.players[i].id)]->SetCharacterType(packet.players->Character_type);
					}
				}
			}
		}

		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			PlayerLifeCnt[i] = packet.players[i].lifecnt;
		}

		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			Player_Type[i] = packet.players[i].Character_type;
		}

		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			PlayerPos[i] = packet.players[i].pos;
		}
		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			PlayerAnim[i] = packet.players[i].anim;
		}
		for (int i = 0; i < 2; ++i)
		{
			iglooLocation[i] = packet.iglooLocation[i];
		}
		for (int i = 0; i < 4; ++i) {
			SnowmanLocation[i] = packet.SnowmanLocation[i];
		}
		for (int i = 0; i < 25; ++i) {
			BlockPos[i] = packet.blocks[i].pos;
			BlockDestructionCnt[i] = packet.blocks[i].destuctioncnt;
		}
		SceneManager::GetApp()->ChangeScene(SceneType::GamePlay);
		break;
	}
	case SC_POS:
	{
		sc_packet_pos packet;
		memcpy(&packet, ptr, sizeof(packet));
		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			PlayerPos[i] = packet.players[i].pos;
			dir[i] = packet.players[i].dir;
			PlayerHide[i] = packet.players[i].IsHide;
			PlayerSnowmanHide[i] = packet.players[i].SnowmanNum;
			PlayerSkillCool[i] = packet.players[i].IsSkillCool;
		}

		break;
	}
	case SC_BLOCK:
	{
		sc_packet_block packet;
		memcpy(&packet, ptr, sizeof(packet));
		for (int i = 0; i < 25; ++i) {
			BlockPos[i] = packet.blocks[i].pos;
			BlockDestructionCnt[i] = packet.blocks[i].destuctioncnt;
		}
		break;
	}
	case SC_CRASH:
	{
		sc_packet_crash packet;
		memcpy(&packet, ptr, sizeof(packet));
		cout << "crash cube num = " << packet.blocknum << endl;
		crash_block_index = packet.blocknum;
		XMFLOAT3 f3 = AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(packet.blocknum))->GetPosition();
		AppContext->DisplayParticle("crushparticle", "crushparticle" + std::to_string(packet.blocknum), XMFLOAT3(f3.x, f3.y +60.f, f3.z));
		break;
	}

	case SC_HITPLAYER:
	{
	
		sc_packet_hitplayer packet;
		memcpy(&packet, ptr, sizeof(packet));
		cout << "type num = " << packet.typenum << endl;
		switch (packet.typenum)
		{
		case CHARACTER_HUSKY:

			AppContext->DisplayParticle("starParticle", "husky_star_particle", packet.hit_pos);
			break;
		case CHARACTER_PENGUIN:
			AppContext->DisplayParticle("starParticle", "Penguin_star_particle", packet.hit_pos);
			break;
		case CHARACTER_POLARBEAR:
			AppContext->DisplayParticle("starParticle", "PolarBear_star_particle", packet.hit_pos);
			break;
		case CHARACTER_ARCTICFOX:
			AppContext->DisplayParticle("starParticle", "ArcticFox_star_particle", packet.hit_pos);
			break;
		case CHARACTER_SEAL:
			AppContext->DisplayParticle("starParticle", "Seal_star_particle", packet.hit_pos);

			break;
		default:
			break;
		}
		SoundManager::GetApp()->PlaySoundOnce(L"Attacked.mp3", SoundManager::CHANNEL_ID::PLAYER_ATTACKED, 3.0f);
		break;
	}

	case SC_ANIM:
	{
		sc_packet_anim packet;
		memcpy(&packet, ptr, sizeof(packet));
		for (int i = 0; i < m_pGameInfo->m_ClientsNum; ++i)
		{
			PlayerAnim[i] = static_cast<int>(packet.players[i].anim);
		}
		break;
	}

	case SC_FALL:
	{
		sc_packet_fall packet;
		memcpy(&packet, ptr, sizeof(packet));
		IsFall[packet.id] = true;
		break;
	}

	case SC_RESET:
	{
		sc_packet_reset packet;
		memcpy(&packet, ptr, sizeof(packet));
		isReset[packet.id] = true;
		PlayerLifeCnt[packet.id] = packet.life;
		AppContext->HiddenUI("player_" + std::to_string(packet.id) + "hp" + std::to_string(packet.life), "player_" + std::to_string(packet.id) + "hp" + std::to_string(packet.life));
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
	case SC_TIME:
	{
		sc_packet_time packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_timer = packet.time;// % 3600 / 60;
		
		break;
	}
	case SC_HAIL:
	{
		sc_packet_hail packet;
		memcpy(&packet, ptr, sizeof(packet));
		for (int i = 0; i < 5; ++i) {
			AppContext->FindObject<GameObject>("rock_1", "rock_1" + std::to_string(i + 15))->SetPosition(packet.pos[i]);
		}
		break;
	}
	case SC_FOX_SKILL:
	{
		sc_packet_foxskill packet;
		memcpy(&packet, ptr, sizeof(packet));
		FoxSkill = packet.foxskill;
		break;
	}
	case SC_HUSKY_SKILL:
	{
		sc_packet_huskyskill packet;
		memcpy(&packet, ptr, sizeof(packet));
		HuskySkill = packet.huskyskill;
		break;
	}
	case SC_PENGUIN_SKILL:
	{
		sc_packet_penguinskill packet;
		memcpy(&packet, ptr, sizeof(packet));
		PenguinSkill = packet.penguinskill;
		break;
	}
	case SC_SEAL_SKILL:
	{
		sc_packet_sealskill packet;
		memcpy(&packet, ptr, sizeof(packet));
		SealSkill = packet.sealskill;
		break;
	}
	case SC_GAMEOVER:
	{
		std::cout << "game over" << std::endl;
		sc_packet_gameover packet;
		memcpy(&packet, ptr, sizeof(packet));
		m_pGameInfo->m_GameStart = false;
		
		m_pGameInfo->m_WinnerID = (int)packet.id;
		cout << "Game Over, Winner is " << m_pGameInfo->m_WinnerID << endl;
		if (m_pGameInfo->m_WinnerID == m_pGameInfo->m_ClientID)
			MessageBox(nullptr, L"You Win", L"Game Over!", MB_OK);
		else if (m_pGameInfo->m_WinnerID != m_pGameInfo->m_ClientID)
			MessageBox(nullptr, L"You Lose", L"Game Over!", MB_OK);

		SceneManager::GetApp()->ChangeScene(SceneType::Lobby);

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

DirectX::XMFLOAT3 Network::GetPlayerPos(int num)const
{
	return PlayerPos[num];
}

int Network::GetiglooLocation(int num)const
{
	return iglooLocation[num];
}

int Network::GetSnowmanLocation(int num)const
{
	return SnowmanLocation[num];
}

int Network::GetPlayerDir(int num)const
{
	return static_cast<int>(dir[num]);
}

DirectX::XMFLOAT3 Network::GetBlockPos(int num)const
{
	return BlockPos[num];
}

DirectX::XMFLOAT3 Network::GetHailPos(int num) const
{
	return HailPos[num];
}

int Network::GetBlockDestructionCnt(int num)const {
	return BlockDestructionCnt[num];
}

int Network::GetCrashBlockIndex() const
{

	return crash_block_index;
}

int Network::GetPlayerAnim(int num)const
{
	return PlayerAnim[num];
}

char Network::GetCharacterType(int num)const {
	return CharacterType[num];
}

bool Network::GetCharacterFall(int num)const
{
	return IsFall[num];
}

void Network::SetCharacterFall(int num)
{
	IsFall[num] = false;
	cout << num << endl;

}

bool Network::GetCharacterReset(int num)const
{
	return isReset[num];
}

void Network::SetCharacterReset(int num)
{
	isReset[num] = false;
}

bool Network::GetPlayerHide(int num)const
{
	return PlayerHide[num];
}

int Network::GetPlayerSnowmanNum(int num)const
{
	return PlayerSnowmanHide[num];
}

int Network::Gettime() const
{

	return m_timer;
}
bool Network::GetFoxSkill() const
{
	return FoxSkill;
}

bool Network::GetHuskySkill() const
{
	return HuskySkill;
}

bool Network::GetPenguinSkill() const
{
	return PenguinSkill;
}

bool Network::GetSealSkill() const
{
	return SealSkill;
}

bool Network::GetPlayerSkillCool(int num)const
{
	return PlayerSkillCool[num];
}

int Network::GetPlayerLifeCnt(int num)const
{
	return PlayerLifeCnt[num];
}

void Network::SetPlayerLifeCnt()
{
	for (int i = 0; i < 5; i++)
		PlayerLifeCnt[i] = 5;
}

int Network::GetChooseCharacter(int num)const
{
	//for (int i = 0; i < 5; ++i) {
	//	cout << "c" <<i<<": " << choosename[i] << endl;
	//}
	return choosename[num];
}