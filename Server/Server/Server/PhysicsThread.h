#pragma once
#include "pch.h"
#include "Global.h"

bool IsRight[25];
UINT ShakeCnt[25];
bool IsDown[25];
bool IsShake[25];

void Update(vector<Player>& player)
{
	float speed = 1.0f; ;
	float crossspeed = sqrt(2) / 2;
	for (int i = 0; i < numOfCls; ++i)
	{
		if (player[i].GetKeyW() && player[i].GetKeyA()) {
			player[i].m_pos.z += crossspeed;
			player[i].m_pos.x -= crossspeed;
			player[i].dir = DIR_UP_LEFT;
		}
		else if (player[i].GetKeyW() && player[i].GetKeyD()) {
			player[i].m_pos.z += crossspeed;
			player[i].m_pos.x += crossspeed;
			player[i].dir = DIR_UP_RIGHT;
		}
		else if (player[i].GetKeyS() && player[i].GetKeyD()) {
			player[i].m_pos.z -= crossspeed;
			player[i].m_pos.x += crossspeed;
			player[i].dir = DIR_DOWN_RIGHT;

		}
		else if (player[i].GetKeyS() && player[i].GetKeyA()) {
			player[i].m_pos.z -= crossspeed;
			player[i].m_pos.x -= crossspeed;
			player[i].dir = DIR_DOWN_LEFT;

		}
		else {
			if (player[i].GetKeyW())
			{
				player[i].m_pos.z += speed;
				player[i].dir = DIR_UP;

			}
			if (player[i].GetKeyS())
			{
				player[i].m_pos.z -= speed;
				player[i].dir = DIR_DOWN;

			}
			if (player[i].GetKeyA())
			{
				player[i].m_pos.x -= speed;
				player[i].dir = DIR_LEFT;

			}
			if (player[i].GetKeyD())
			{
				player[i].m_pos.x += speed;
				player[i].dir = DIR_RIGHT;

			}
		}
	}

}

bool BlockCheck(int idx) {
	if (idx == 0 || idx == 2 || idx == 4 || idx == 10 || idx == 12 || idx == 14 || idx == 20 || idx == 22 || idx == 24)
		return false;
	return true;
}

void shake(Block* object, int index) {
	if (BlockCheck(index)) {		//destruct block
		if (IsRight[index]) {
			if (object->pos.x < 200 * (index / 5) + 5) {
				object->pos.x += 0.5f;
			}
			else
				IsRight[index] = false;
		}
		else {
			if (object->pos.x > 200 * (index / 5) - 5) {
				object->pos.x -= 0.5f;
			}
			else
				IsRight[index] = true;
		}
		if (IsRight[index] && (object->pos.x - 0.001f <= 200 * (index / 5) && object->pos.x + 0.001f >= 200 * (index / 5)))
			++ShakeCnt[index];
	}
	else {
		if (IsDown[index] /* && tmp2 != -1*/) {		//������ �� ������
			if (object->pos.y <= -170)
				IsDown[index] = false;
			else {
				object->pos.y -= (-object->pos.y / 140.0 - 4.0 / 35.0);
			}
		}
		else {
			if (object->pos.y <= -30/* && (tmp2 == -1 || m_Users[m_PlayerID]->bJump)*/) {		//������ �� ��� �ְų� �÷��̾ �������̸�
				object->pos.y += 0.2f;
			}
			else {
				IsDown[index] = true;
			}
			IsShake[index] = false;
		}
	}
}

void UpdateBlock(Block* object) {
	for (int i = 0; i < 25; ++i) {
		if (IsShake[i] || !IsDown[i]) {
			shake(&object[i], i);	//block
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++object[i].destuctioncnt;
		}
	}
}

void ProcessClients()
{
	g_player_lock.lock();
	Pro_Player players[3] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]} };
	Block blocks[25];
	for (int i = 0; i < 25; ++i) {
		blocks[i].id = i;
		blocks[i].destuctioncnt = 0;
		blocks[i].pos.x = i / 5 * 200;
		blocks[i].pos.y = -30.0;
		blocks[i].pos.z = i % 5 * 200;
		IsRight[i] = true;
		ShakeCnt[i] = 0;
		IsDown[i] = true;
		IsShake[i] = false;
	}
	//g_player_lock.unlock();

	std::queue <Message> phyMsgQueue;
	Message phyMsg;
	DirectX::XMFLOAT3 temp;

	for (int i = 0; i < numOfCls; ++i)
	{
		temp = { players[i].pos.x, players[i].pos.y,players->pos.z };
		phyPlayers.emplace_back(Player());
		//phyPlayers.emplace_back(Player());
		phyPlayers[i].SetPos(temp);
	}

	using FpFloatMilliseconds = duration<float, milliseconds::period>;
	auto prev_Time = chrono::high_resolution_clock::now();
	float elapsedTime{};
	float deltaT;

	//using frame = duration<int32_t, ratio<1, FPS>>;
	//using ms = duration<float, milli>;

	//time_point<steady_clock> fpsTimer(steady_clock::now());
	//frame FramePerSec{};
	while (true)
	{
		auto cur_Time = chrono::high_resolution_clock::now();
		elapsedTime += FpFloatMilliseconds(cur_Time - prev_Time).count();
		deltaT = FpFloatMilliseconds(cur_Time - prev_Time).count();
		prev_Time = cur_Time;
		//FramePerSec = duration_cast<frame>(steady_clock::now() - fpsTimer);

		if (/*FramePerSec.count() >= 1*/elapsedTime > 16)
		{
			static bool d = true;
			if (GetAsyncKeyState('1') & 0x8000) {		//n�� ���Ͽ� �浹�� �� IsShake[n], IsDown[n] �� �� true�� �����ϵ��� ���߿� �ٲٱ� �ʿ�
				if (d) {
					IsShake[5] = true;
					IsDown[5] = true;
					IsShake[10] = true;
					IsDown[10] = true;
					d = false;
				}
			}
			else {
				d = true;
			}

			g_MsgQueueLock.lock();
			phyMsgQueue = g_MsgQueue;
			g_MsgQueueLock.unlock();

			g_MsgQueueLock.lock();
			while (!g_MsgQueue.empty())
				g_MsgQueue.pop();
			g_MsgQueueLock.unlock();

			while (!phyMsgQueue.empty())
			{
				phyMsg = phyMsgQueue.front();
				phyMsgQueue.pop();

				if (phyMsg.type == TYPE_PLAYER)
				{
					switch (phyMsg.dir)
					{
					case DIR_UP:
						phyPlayers[phyMsg.id].SetKeyW(phyMsg.isPushed);
						break;
					case DIR_DOWN:
						phyPlayers[phyMsg.id].SetKeyS(phyMsg.isPushed);
						break;
					case DIR_LEFT:
						phyPlayers[phyMsg.id].SetKeyA(phyMsg.isPushed);
						break;
					case DIR_RIGHT:
						phyPlayers[phyMsg.id].SetKeyD(phyMsg.isPushed);
						break;
					case KEY_ATTACK:
						phyPlayers[phyMsg.id].is_attack = true;
						cout << "attack1 ===="<< static_cast<int>(phyMsg.id) << "==>" << phyPlayers[phyMsg.id].is_attack << endl;
						break;
					case KEY_JUMP:

						phyPlayers[phyMsg.id].is_jump = true;
						break;
					}
				}
				//else if (phyMsg.type == TYPE_ANIMATION)
				//{
				//	switch (phyMsg.AnimType)
				//	{
				///*	case ANIM_IDLE:
				//		if(phyPlayers[phyMsg.id].GetKeyA() || phyPlayers[phyMsg.id].GetKeyS()|| phyPlayers[phyMsg.id].GetKeyW()|| phyPlayers[phyMsg.id].GetKeyD())
				//		phyPlayers[phyMsg.id].anim = ANIM_MOVE;
				//		else
				//			phyPlayers[phyMsg.id].anim = ANIM_IDLE;
				//		break;

				//	case ANIM_MOVE:
				//		phyPlayers[phyMsg.id].anim = ANIM_MOVE;
				//		break;*/

				//	case ANIM_ATTACK:
				//		
				//		phyPlayers[phyMsg.id].anim = ANIM_ATTACK;
				//		phyPlayers[phyMsg.id].isAttack = true;
				//		
				//		break;

				//	case ANIM_JUMP:
				//		phyPlayers[phyMsg.id].anim = ANIM_JUMP;
				//		phyPlayers[phyMsg.id].isJump = true;
				//		break;

				//	}
				//}

				Update(phyPlayers);

				if (phyMsg.type == TYPE_PLAYER)
				{
					for (int i = 0; i < numOfCls; ++i)
					{
						players[i].id = i;
						players[i].pos.x = phyPlayers[i].m_pos.x;
						players[i].pos.y = phyPlayers[i].m_pos.y;
						players[i].pos.z = phyPlayers[i].m_pos.z;
						players[i].dir = phyPlayers[i].dir;
						cout <<"attack2 ==="<<i <<" ==> " << phyPlayers[i].is_attack << endl;
						players[i].anim = phyPlayers[i].GetAnimType();
					}
					SendPos(*players);
					SendAnim(*players);
					cout << static_cast<int>(players[0].dir) << endl;
				}
				//else if (phyMsg.type == TYPE_ANIMATION)
				//{
				//	for (int i = 0; i < numOfCls; ++i)
				//	{
				//		players[i].id = i;
				//	//	players[i].anim = phyPlayers[i].anim;
				//		players[i].anim = phyPlayers[i].GetAnimType();
				//	}
				//	SendAnim(*players);
				//}
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				if (phyPlayers[i].is_attack == true)
				{
					phyPlayers[i].AttackTimeCount += 1.0f;
					if (phyPlayers[i].AttackTimeCount > 60.0f)
					{
						phyPlayers[i].AttackTimeCount = 0.0f;
						phyPlayers[i].is_attack = false;
					/*	if (phyPlayers[i].GetKeyA() || phyPlayers[i].GetKeyW() || phyPlayers[i].GetKeyS() || phyPlayers[i].GetKeyD())
						{
							players[i].anim = ANIM_MOVE;
						}
						else {
							players[i].anim = ANIM_IDLE;
						}*/
						players[i].anim = phyPlayers[i].GetAnimType();
						SendAnim(*players);
					}
				}

				if (phyPlayers[i].is_jump == true)
				{
					phyPlayers[i].JumpTimeCount += 1.0f;
					if (phyPlayers[i].JumpTimeCount > 60.0f)
					{
						phyPlayers[i].JumpTimeCount = 0.0f;
						phyPlayers[i].is_jump = false;
					/*	if (phyPlayers[i].GetKeyA() || phyPlayers[i].GetKeyW() || phyPlayers[i].GetKeyS() || phyPlayers[i].GetKeyD())
						{
							players[i].anim = ANIM_MOVE;
						}
						else {
							players[i].anim = ANIM_IDLE;
						}*/
						players[i].anim = phyPlayers[i].GetAnimType();
						SendAnim(*players);
					}
				}


			}
		/*	for (int i = 0; i < numOfCls; ++i)
			{
				players[i].id = i;
				players[i].anim = phyPlayers[i].anim;
			}
			if (phyMsg.type == TYPE_ANIMATION)
			{
				players[phyMsg.id].anim = phyPlayers[phyMsg.id].GetAnimType();
				SendAnim(*players);
			}*/

			UpdateBlock(blocks);
			SendBlockPacket(*blocks);
			//fpsTimer = steady_clock::now();
			//cout << "LastFrame:" << duration_cast<ms>(FramePerSec).count() << "ms | FPS: " << FramePerSec.count() * FPS << endl;
			elapsedTime = 0;
		}
	}
	phyPlayers.clear();
}