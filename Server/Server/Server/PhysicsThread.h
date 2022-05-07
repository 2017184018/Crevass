#pragma once
#include "pch.h"
#include "Global.h"
#include <string>

bool IsRight[25];
UINT ShakeCnt[25];
bool IsDown[25];
bool IsShake[25];
int tmp1[3] = { -1,-1,-1 };
bool BlockIn = false;
float Gravity = 0.1;
int tmp2[3] = { -1,-1,-1 };

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
		if (IsDown[index] && (tmp2[0] != -1 || tmp2[1] != -1 || tmp2[2] != -1)) {
			if (object->pos.y <= -170)
				IsDown[index] = false;
			else {
				object->pos.y -= (-object->pos.y / 140.0 - 4.0 / 35.0);
			}
		}
		else {
			if (object->pos.y <= -30 && (tmp2[0] == -1 || tmp2[1] == -1 || tmp2[2] == -1)) {
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
	string TypeName[3];

	for (int i = 0; i < 3; ++i) {
		if (players[i].Character_type == CHARACTER_HUSKY) {
			g_boundaries["husky"]->Center = players[i].pos;
			TypeName[i] = "husky";
		}
		else if (players[i].Character_type == CHARACTER_PENGUIN) {
			g_boundaries["Penguin"]->Center = players[i].pos;
			TypeName[i] = "Penguin";
		}
		else if (players[i].Character_type == CHARACTER_ARCTICFOX) {
			g_boundaries["ArcticFox"]->Center = players[i].pos;
			TypeName[i] = "ArcticFox";
		}
		else if (players[i].Character_type == CHARACTER_SEAL) {
			g_boundaries["Seal"]->Center = players[i].pos;
			TypeName[i] = "Seal";
		}
		else if (players[i].Character_type == CHARACTER_POLARBEAR) {
			g_boundaries["PolarBear"]->Center = players[i].pos;
			TypeName[i] = "PolarBear";
		}
	}

	Block blocks[25];
	DirectX::XMFLOAT3 tmp[25];

	for (int i = 0; i < 25; ++i) {
		blocks[i].id = i;
		tmp[i] = DirectX::XMFLOAT3(i / 5 * 200, -30.0, i % 5 * 200);
		if (BlockCheck(i)) {
			g_boundaries["icecube" + std::to_string(i)]->Center = blocks[i].pos = tmp[i];
		}
		else {
			g_boundaries["snowcube" + std::to_string(i)]->Center = blocks[i].pos = tmp[i];
		}

		blocks[i].destuctioncnt = 0;

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
					}
				}
				else if (phyMsg.type == TYPE_ANIMATION)
				{
					switch (phyMsg.AnimType)
					{
					case ANIM_IDLE:
						phyPlayers[phyMsg.id].anim = ANIM_IDLE;
						break;

					case ANIM_MOVE:
						phyPlayers[phyMsg.id].anim = ANIM_MOVE;
						break;

					case ANIM_ATTACK:
						phyPlayers[phyMsg.id].anim = ANIM_ATTACK;
						players[phyMsg.id].isAttack = true;
						break;

					case ANIM_JUMP:
						phyPlayers[phyMsg.id].anim = ANIM_JUMP;
						players[phyMsg.id].isJump = true;
						break;

					}
				}
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

						if (players[i].Character_type == CHARACTER_HUSKY) {
							g_boundaries["husky"]->Center = players[i].pos;
						}
						else if (players[i].Character_type == CHARACTER_PENGUIN) {
							g_boundaries["Penguin"]->Center = players[i].pos;
						}
						else if (players[i].Character_type == CHARACTER_ARCTICFOX) {
							g_boundaries["ArcticFox"]->Center = players[i].pos;
						}
						else if (players[i].Character_type == CHARACTER_SEAL) {
							g_boundaries["Seal"]->Center = players[i].pos;
						}
						else if (players[i].Character_type == CHARACTER_POLARBEAR) {
							g_boundaries["PolarBear"]->Center = players[i].pos;
						}
					}
					SendPos(*players);
					cout << static_cast<int>(players[0].dir) << endl;
				}
				else if (phyMsg.type == TYPE_ANIMATION)
				{
					for (int i = 0; i < numOfCls; ++i)
					{
						players[i].id = i;
						players[i].anim = phyPlayers[i].anim;
					}
					SendAnim(*players);
				}
			}
			for (int i = 0; i < numOfCls; ++i)
			{
				if (players[i].isAttack == true)
				{
					players[i].AttackTimeCount += 1.0f;
					if (players[i].AttackTimeCount > 60.0f)
					{
						players[i].AttackTimeCount = 0.0f;
						players[i].isAttack = false;
						players[i].anim = ANIM_IDLE;
						SendAnim(*players);
					}
				}

				if (players[i].isJump == true)
				{
					players[i].JumpTimeCount += 1.0f;
					if (players[i].JumpTimeCount > 60.0f)
					{
						players[i].JumpTimeCount = 0.0f;
						players[i].isJump = false;
						players[i].anim = ANIM_IDLE;
						SendAnim(*players);
					}
				}
			}

			UpdateBlock(blocks);
			for (int j = 0; j < numOfCls; ++j) {
				for (int i = 0; i < 25; ++i) {
					if (BlockCheck(i)) {
						g_boundaries["icecube" + std::to_string(i)]->Center = blocks[i].pos;
						if (tmp1[j] == -1 && g_boundaries["icecube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp1[j] = i;
							if (!BlockIn) {
								IsShake[i] = true;
								IsDown[i] = true;
								BlockIn = true;
							}
						}
						if (tmp1[j] != -1 && !(g_boundaries["icecube" + std::to_string(tmp1[j])]->Intersects(*g_boundaries[TypeName[j]]))) {
							BlockIn = false;
							if (!players[j].isJump)
								Gravity += 0.05;
							tmp1[j] = -1;
						}
					}
					else {
				//		g_boundaries["snowcube" + std::to_string(i)]->Center = blocks[i].pos;
						if (tmp2[j] == -1 && g_boundaries["snowcube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp2[j] = i;
							IsShake[i] = true;
							IsDown[i] = true;
						}
						if (tmp2[j] != -1 && !(g_boundaries["snowcube" + std::to_string(tmp2[j])]->Intersects(*g_boundaries[TypeName[j]]))) {  // or 물에 떨어지면
							IsDown[tmp2[j]] = false;
							if (!players[j].isJump)
								Gravity += 0.05;
							tmp2[j] = -1;
						}
					}
				}
			}
			SendBlockPacket(*blocks);
			//fpsTimer = steady_clock::now();
			//cout << "LastFrame:" << duration_cast<ms>(FramePerSec).count() << "ms | FPS: " << FramePerSec.count() * FPS << endl;
			elapsedTime = 0;
		}
	}
	phyPlayers.clear();
}