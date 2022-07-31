#pragma once
#include "pch.h"
#include "Global.h"
#include "Hail.h"
#include <string>
#include <random>

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,2 };
uniform_int_distribution<> uid2{ 0,1 };
uniform_int_distribution<> uid3{ 0,3 };

bool IsRight[25];
UINT ShakeCnt[25];
bool IsDown[25];
bool IsShake[25];
Block blocks[25];

void Update(vector<Player>& player, float elapsedTime)
{
	float elaps_time = elapsedTime / 16;

	for (int i = 0; i < numOfCls; ++i)
	{
		float saveX = 0;
		float saveZ = 0;
		if (player[i].is_hitted == false || player[i].IsFall==false) {
			if (player[i].GetKeyW() && player[i].GetKeyA()) {
				player[i].m_pos.z += player[i].GetCrossSpeed() * elaps_time;
				player[i].m_pos.x -= player[i].GetCrossSpeed() * elaps_time;
				player[i].dir = DIR_UP_LEFT;
				saveX = -player[i].GetCrossSpeed() * elaps_time;
				saveZ = player[i].GetCrossSpeed() * elaps_time;
			}
			else if (player[i].GetKeyW() && player[i].GetKeyD()) {
				player[i].m_pos.z += player[i].GetCrossSpeed() * elaps_time;
				player[i].m_pos.x += player[i].GetCrossSpeed() * elaps_time;
				player[i].dir = DIR_UP_RIGHT;
				saveX = player[i].GetCrossSpeed() * elaps_time;
				saveZ = player[i].GetCrossSpeed() * elaps_time;
			}
			else if (player[i].GetKeyS() && player[i].GetKeyD()) {
				player[i].m_pos.z -= player[i].GetCrossSpeed() * elaps_time;
				player[i].m_pos.x += player[i].GetCrossSpeed() * elaps_time;
				player[i].dir = DIR_DOWN_RIGHT;
				saveX = player[i].GetCrossSpeed() * elaps_time;
				saveZ = -player[i].GetCrossSpeed() * elaps_time;

			}
			else if (player[i].GetKeyS() && player[i].GetKeyA()) {
				player[i].m_pos.z -= player[i].GetCrossSpeed() * elaps_time;
				player[i].m_pos.x -= player[i].GetCrossSpeed() * elaps_time;
				player[i].dir = DIR_DOWN_LEFT;
				saveX = -player[i].GetCrossSpeed() * elaps_time;
				saveZ = -player[i].GetCrossSpeed() * elaps_time;
			}
			else {
				if (player[i].GetKeyW())
				{
					player[i].m_pos.z += player[i].GetSpeed() * elaps_time;
					player[i].dir = DIR_UP;
					saveZ = player[i].GetSpeed() * elaps_time;
				}
				if (player[i].GetKeyS())
				{
					player[i].m_pos.z -= player[i].GetSpeed() * elaps_time;
					player[i].dir = DIR_DOWN;
					saveZ = -player[i].GetSpeed() * elaps_time;
				}
				if (player[i].GetKeyA())
				{
					player[i].m_pos.x -= player[i].GetSpeed() * elaps_time;
					player[i].dir = DIR_LEFT;
					saveX = -player[i].GetSpeed() * elaps_time;
				}
				if (player[i].GetKeyD())
				{
					player[i].m_pos.x += player[i].GetSpeed() * elaps_time;
					player[i].dir = DIR_RIGHT;
					saveX = player[i].GetSpeed() * elaps_time;
				}
			}
		}

		else {
			//위
			if (static_cast<int>(player[i].hitted_dir) == 0) {
				player[i].m_pos.z += player[i].GetSpeed() * elaps_time;
				player[i].dir = DIR_DOWN;
				saveZ = player[i].GetSpeed() * elaps_time;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 2)
			{
				player[i].m_pos.x += player[i].GetSpeed() * elaps_time;
				player[i].dir = DIR_LEFT;
				saveZ = player[i].GetSpeed() * elaps_time;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 4)
			{
				player[i].m_pos.z -= player[i].GetSpeed() * elaps_time;
				player[i].dir = DIR_UP;
				saveZ = player[i].GetSpeed() * elaps_time;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 6)
			{
				player[i].m_pos.x -= player[i].GetSpeed() * elaps_time;
				player[i].dir = DIR_RIGHT;
				saveZ = player[i].GetSpeed() * elaps_time;
			}
		}


		for (int j = 0; j < numOfCls; ++j) {
			if (i != j && !(player[i].TypeName == "husky" && player[i].is_Skill)) {
				g_boundaries[player[i].TypeName]->Center.x += saveX;
				g_boundaries[player[i].TypeName]->Center.y += player[i].gravity;
				g_boundaries[player[i].TypeName]->Center.z += saveZ;
				if (g_boundaries[player[i].TypeName]->Intersects(*g_boundaries[player[j].TypeName])) {
					player[i].m_pos.x -= saveX;
					player[i].m_pos.y -= player[i].gravity;
					player[i].m_pos.z -= saveZ;
					g_boundaries[player[i].TypeName]->Center.x -= saveX;
					g_boundaries[player[i].TypeName]->Center.y -= player[i].gravity;
					g_boundaries[player[i].TypeName]->Center.z -= saveZ;
					if (player[i].GetPos().y < player[j].GetPos().y)
						player[j].gravity = 0;
					else
						player[i].gravity = 0;
				}
			}
		}


		for (int j = 0; j < 25; ++j) {
			if (player[i].m_pos.y < blocks[j].pos.y + 60) {
				g_boundaries[phyPlayers[i].TypeName]->Center = player[i].m_pos;

			//	g_boundaries[player[i].TypeName]->Center.x += saveX;
			//	g_boundaries[player[i].TypeName]->Center.z += saveZ;
				if (BlockCheck(j)) {
					if (g_boundaries["icecube" + std::to_string(j)]->Intersects(*g_boundaries[player[i].TypeName])) {
						player[i].m_pos.x -= saveX;
						player[i].m_pos.y -= 1.f;
						player[i].m_pos.z -= saveZ;
					//	g_boundaries[player[i].TypeName]->Center.x -= saveX;
					//	g_boundaries[player[i].TypeName]->Center.z -= saveZ;
					}
				}
				else {
					if (g_boundaries["snowcube" + std::to_string(j)]->Intersects(*g_boundaries[player[i].TypeName])) {
						player[i].m_pos.x -= saveX;
						//player[i].m_pos.y -= 1.f;
						player[i].m_pos.z -= saveZ;
					//	g_boundaries[player[i].TypeName]->Center.x -= saveX;
					//	g_boundaries[player[i].TypeName]->Center.z -= saveZ;
					}
				}
			}
		}
	}
}

void Hitted_Pos_Update(Player& player, int tyname_num, float anitime) {

	float saveX = 0;
	float saveZ = 0;
	//위
	if (static_cast<int>(player.hitted_dir) == 0) {
		player.m_pos.z += player.GetHittedSpeed() * HITTED_POWER;
		player.dir = DIR_DOWN;
		saveZ = player.GetHittedSpeed() * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 2)
	{
		player.m_pos.x += player.GetHittedSpeed() * HITTED_POWER;
		player.dir = DIR_LEFT;
		saveX = player.GetHittedSpeed() * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 4)
	{
		player.m_pos.z -= player.GetHittedSpeed() * HITTED_POWER;
		player.dir = DIR_UP;
		saveZ = -player.GetHittedSpeed() * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 6)
	{
		player.m_pos.x -= player.GetHittedSpeed() * HITTED_POWER;
		player.dir = DIR_RIGHT;
		saveX = -player.GetHittedSpeed() * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 1)
	{
		player.m_pos.x += player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.m_pos.z += player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.dir = DIR_DOWN_LEFT;
		saveX = player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		saveZ = player.GetHittedSpeed() * cos(45) * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 3)
	{
		player.m_pos.x += player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.m_pos.z -= player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.dir = DIR_UP_LEFT;
		saveX = player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		saveZ = -player.GetHittedSpeed() * cos(45) * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 5)
	{
		player.m_pos.x -= player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.m_pos.z -= player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.dir = DIR_UP_RIGHT;
		saveX = -player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		saveZ = -player.GetHittedSpeed() * cos(45) * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 7)
	{
		player.m_pos.x -= player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.m_pos.z += player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		player.dir = DIR_DOWN_RIGHT;
		saveX = -player.GetHittedSpeed() * cos(45) * HITTED_POWER;
		saveZ = player.GetHittedSpeed() * cos(45) * HITTED_POWER;
	}


	for (int j = 0; j < 25; ++j) {
		if (player.m_pos.y < blocks[j].pos.y + 60) {
			g_boundaries[player.TypeName]->Center.x += saveX;
			g_boundaries[player.TypeName]->Center.z += saveZ;
			if (BlockCheck(j)) {
				if (g_boundaries["icecube" + std::to_string(j)]->Intersects(*g_boundaries[player.TypeName])) {
					player.m_pos.x -= saveX;
					player.m_pos.z -= saveZ;
					g_boundaries[player.TypeName]->Center.x -= saveX;
					g_boundaries[player.TypeName]->Center.z -= saveZ;
				}
			}
			else {
				if (g_boundaries["snowcube" + std::to_string(j)]->Intersects(*g_boundaries[player.TypeName])) {
					player.m_pos.x -= saveX;
					player.m_pos.z -= saveZ;
					g_boundaries[player.TypeName]->Center.x -= saveX;
					g_boundaries[player.TypeName]->Center.z -= saveZ;
				}
			}
		}
	}

}

void shake(Block* object, int index) {
	if (BlockCheck(index)) {      //destruct block
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
		if (IsDown[index]) {
			bool check = false;
			for (int i = 0; i < numOfCls; ++i) {
				if (phyPlayers[i].CurrentSnowcube == index) {
					check = true;
					break;
				}
			}
			if (check) {
				if (object->pos.y <= -170)
					IsDown[index] = false;
				else {
					object->pos.y -= (-object->pos.y / 140.0 - 4.0 / 35.0);
				}
			}
		}
		else {
			if (object->pos.y <= -30) {
				bool check = false;
				for (int i = 0; i < numOfCls; ++i) {
					if (phyPlayers[i].CurrentSnowcube == index) {
						check = true;
						break;
					}
				}
				if (!check) {
					object->pos.y += 0.2f;
				}
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
			shake(&object[i], i);   //block
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++object[i].destuctioncnt;
			SendCrashPacket(i);
			if (object[i].destuctioncnt == 3) {
				g_boundaries["icecube" + std::to_string(i)]->Center = DirectX::XMFLOAT3(-0, -1000, 0);
				g_boundaries["icecube" + std::to_string(i)]->Extents = DirectX::XMFLOAT3(0, 0, 0);
				object[i].pos.y = -150;

			}
		}
		if (object[i].destuctioncnt == 0) {
			if (BlockCheck(i)) {
				if (object[i].pos.y < -30) {
					object[i].pos.y += 1;
				}
			}
		}
	}
}

void PolarbearSkill(int tmp) {
	if (BlockCheck(tmp) && blocks[tmp].destuctioncnt != 3) {
		IsShake[tmp] = true;
		IsDown[tmp] = true;
	}
}

void ProcessClients()
{
	Hail hails[5];
	DirectX::XMFLOAT3 OriginBlockExtents;
	int CalcTime = 0;
	g_player_lock.lock();
	int GameOverTimeCount = 0;
	bool GameOverCheck = false;
	bool BlockIn = false;
	
	bool HideInSnowman[4] = { false,false,false,false };	//n번째 눈사람에 누군가 숨어있는지
	bool dir_switch[5];
	bool BearSkill = true;
	Pro_Player players[5] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]},{g_initialPos[3]},{g_initialPos[4]} };

	for (int i = 0; i < 5; ++i) {
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
	OriginBlockExtents = g_boundaries["icecube0"]->Extents;
	auto OriginSnowmanExtens = g_boundaries["snowman0"]->Extents;
	auto OriginIglooExtens = g_boundaries["igloo0"]->Extents;

	g_player_lock.unlock();

	std::queue <Message> phyMsgQueue;
	Message phyMsg;
	DirectX::XMFLOAT3 temp;

	for (int i = 0; i < numOfCls; ++i)
	{
		temp = { players[i].pos.x, players[i].pos.y,players[i].pos.z };
		phyPlayers.emplace_back(Player());
		//phyPlayers.emplace_back(Player());
		phyPlayers[i].SetPos(temp);
		if (players[i].Character_type == CHARACTER_HUSKY) {
			phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 4.0f);
			phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 4.0f));
			phyPlayers[i].SetHittedSpeed(BASE_HITTEDSPEED + 0.2f * 4.0f);
			phyPlayers[i].TypeName = "husky";
		}
		else if (players[i].Character_type == CHARACTER_PENGUIN) {
			phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 2.0f);
			phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 2.0f));
			phyPlayers[i].SetHittedSpeed(BASE_HITTEDSPEED + 0.2f * 2.0f);
			phyPlayers[i].TypeName = "Penguin";
		}
		else if (players[i].Character_type == CHARACTER_ARCTICFOX) {
			phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 5.0f);
			phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 5.0f));
			phyPlayers[i].SetHittedSpeed(BASE_HITTEDSPEED + 0.2f * 5.0f);
			phyPlayers[i].TypeName = "ArcticFox";
		}
		else if (players[i].Character_type == CHARACTER_SEAL) {
			phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 3.0f);
			phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 3.0f));
			phyPlayers[i].SetHittedSpeed(BASE_HITTEDSPEED + 0.2f * 3.0f);
			phyPlayers[i].TypeName = "Seal";
		}
		else if (players[i].Character_type == CHARACTER_POLARBEAR) {
			phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 1.0f);
			phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 1.0f));
			phyPlayers[i].SetHittedSpeed(BASE_HITTEDSPEED + 0.2f * 1.0f);
			phyPlayers[i].TypeName = "PolarBear";
		}
		phyPlayers[i].Hit_BB.Radius = g_boundaries[phyPlayers[i].TypeName]->Extents.z / 2;
	}

	using FpFloatMilliseconds = duration<float, milliseconds::period>;
	auto prev_Time = chrono::high_resolution_clock::now();
	float elapsedTime{};
	float deltaT;
	while (true)
	{
		auto cur_Time = chrono::high_resolution_clock::now();
		elapsedTime += FpFloatMilliseconds(cur_Time - prev_Time).count();
		deltaT = FpFloatMilliseconds(cur_Time - prev_Time).count();
		prev_Time = cur_Time;

		//FramePerSec = duration_cast<frame>(steady_clock::now() - fpsTimer);

		if (elapsedTime > 1000.0f / 60.0f)
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
						if (!dir_switch) {
							temp.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = true;
						}
						break;
					case DIR_DOWN:

						phyPlayers[phyMsg.id].SetKeyS(phyMsg.isPushed);
						if (dir_switch) {
							temp.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = false;
						}
						break;
					case DIR_LEFT:
						phyPlayers[phyMsg.id].SetKeyA(phyMsg.isPushed);
						if (dir_switch) {
							temp.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = false;
						}
						break;
					case DIR_RIGHT:
						phyPlayers[phyMsg.id].SetKeyD(phyMsg.isPushed);
						if (!dir_switch) {
							temp.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z = g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x;
							g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = true;
						}
						break;
					case KEY_ATTACK:
						phyPlayers[phyMsg.id].is_attack = true;
						if (phyPlayers[phyMsg.id].is_jump == false) {
							phyPlayers[phyMsg.id].SetKeyA(false);
							phyPlayers[phyMsg.id].SetKeyW(false);
							phyPlayers[phyMsg.id].SetKeyS(false);
							phyPlayers[phyMsg.id].SetKeyD(false);
						}
						//hit box  위치 갱신 
						phyPlayers[phyMsg.id].Hit_BB.Center = phyPlayers[phyMsg.id].GetPos();
						if (phyPlayers[phyMsg.id].dir == 0) {
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 2)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 4)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 6)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 1)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 3)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z;

						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 5)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;

						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 7)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[phyPlayers[phyMsg.id].TypeName]->Extents.z ;

						}

						for (int i = 0; i < numOfCls; i++) {
							g_boundaries[phyPlayers[i].TypeName]->Center = players[i].pos;

							if (phyMsg.id != i) {
								//맞았다면
								if (phyPlayers[phyMsg.id].Hit_BB.Intersects(*g_boundaries[phyPlayers[i].TypeName])) {
									cout << "hit!" << endl;
									phyPlayers[i].is_hitted = true;
									phyPlayers[i].is_jump = true;
									//맞은 방향
									phyPlayers[i].hitted_dir = phyPlayers[phyMsg.id].dir;
									SendHitPalyer(players[phyMsg.id].Character_type, phyPlayers[phyMsg.id].Hit_BB.Center);

									phyPlayers[phyMsg.id].Hit_BB.Center = DirectX::XMFLOAT3(0, 0, 0);

								}
							}
						}
						break;
					case KEY_JUMP:
						if (phyPlayers[phyMsg.id].m_pos.y > -100 && phyPlayers[phyMsg.id].SnowmanNum == -1)
							phyPlayers[phyMsg.id].is_jump = true;
						phyPlayers[phyMsg.id].is_jumpanim = true;
						break;
					case KEY_INTERACT:
						phyPlayers[phyMsg.id].IsInteract = true;
						break;
					case KEY_SKILL:
						phyPlayers[phyMsg.id].is_Skill = true;
						phyPlayers[phyMsg.id].is_Skillanim = true;
						break;
					}
				}

				//Update(phyPlayers);

				//if (phyMsg.type == TYPE_PLAYER)
				//{
				//   for (int i = 0; i < numOfCls; ++i)
				//   {
				//      players[i].id = i;
				//      players[i].pos.x = phyPlayers[i].m_pos.x;
				//      players[i].pos.y = phyPlayers[i].m_pos.y;
				//      players[i].pos.z = phyPlayers[i].m_pos.z;
				//      players[i].dir = phyPlayers[i].dir;
				//      players[i].anim = phyPlayers[i].GetAnimType();

				//      g_boundaries[TypeName[i]]->Center = players[i].pos;
				//      //임시방편
				//      if (TypeName[i] == "Penguin") {
				//         g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 3;
				//      }
				//      else {
				//         g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 1.5;
				//      }

				//   }
				//   //   SendPos(*players);
				//   SendAnim(*players);
				//}
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				if (phyPlayers[i].is_attack == true)
				{
					phyPlayers[i].AttackTimeCount += elapsedTime;
					if (phyPlayers[i].AttackTimeCount > 700.0f)
					{
						phyPlayers[i].AttackTimeCount = 0.0f;
						phyPlayers[i].is_attack = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						//SendAnim(*players);   /
					}
				}

				if (phyPlayers[i].is_jumpanim == true)
				{
					phyPlayers[i].JumpTimeCount += elapsedTime;
					if (phyPlayers[i].JumpTimeCount > 700.0f)
					{
						phyPlayers[i].JumpTimeCount = 0.0f;
						phyPlayers[i].is_jumpanim = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						//SendAnim(*players);   /
					}
				}

				if (phyPlayers[i].is_Skillanim == true)
				{
					phyPlayers[i].SkillTimeCount += elapsedTime;
					if (phyPlayers[i].SkillTimeCount > 700.0f)
					{
						phyPlayers[i].SkillTimeCount = 0.0f;
						if (phyPlayers[i].TypeName != "Penguin")
							phyPlayers[i].is_Skillanim = false;
						players[i].anim = phyPlayers[i].GetAnimType();
					}
				}

				if (phyPlayers[i].is_hitted == true)
				{
					phyPlayers[i].hittedTimeCount += elapsedTime;
					if (phyPlayers[i].hittedTimeCount > 700.0f)
					{
						phyPlayers[i].hittedTimeCount = 0.0f;
						phyPlayers[i].is_hitted = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						//SendAnim(*players);   /
					}
					else {
						Hitted_Pos_Update(phyPlayers[i], i, phyPlayers[i].hittedTimeCount);
						players[i].id = i;
						players[i].pos.x = phyPlayers[i].m_pos.x;
						players[i].pos.y = phyPlayers[i].m_pos.y;
						players[i].pos.z = phyPlayers[i].m_pos.z;
						players[i].dir = phyPlayers[i].dir;
						//SendPos(*players);

					}
				}


			}
			for (int i = 0; i < numOfCls; ++i)
			{
				//아이스 , 스노우 아무것도 출동하지 않을때 
				if (phyPlayers[i].CurrentIcecube == -1 && phyPlayers[i].CurrentSnowcube == -1)
				{
					phyPlayers[i].gravity -= 0.98f / pow(elapsedTime, 2) * elapsedTime;
				}
			}

			for (int i = 0; i < numOfCls; ++i) {
				if (phyPlayers[i].CurrentIcecube == -1 && phyPlayers[i].CurrentSnowcube == -1) {
					phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
					players[i].pos.y = phyPlayers[i].m_pos.y;
				}
				else {
					if (phyPlayers[i].CurrentIcecube != -1) {
						if (phyPlayers[i].m_pos.y > 10 && phyPlayers[i].is_jump == false && blocks[phyPlayers[i].CurrentIcecube].destuctioncnt != 3 && phyPlayers[i].m_pos.y <= 30) {
							phyPlayers[i].m_pos.y = blocks[phyPlayers[i].CurrentIcecube].pos.y + 60;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
						else {
							phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
					}
					else {
						if (phyPlayers[i].m_pos.y - blocks[phyPlayers[i].CurrentSnowcube].pos.y >= 50 &&
							phyPlayers[i].is_jump == false && !phyPlayers[i].IsFall && phyPlayers[i].m_pos.y - blocks[phyPlayers[i].CurrentSnowcube].pos.y <= 70)
						{
							phyPlayers[i].m_pos.y = blocks[phyPlayers[i].CurrentSnowcube].pos.y + 60;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
						else
						{
							phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
					}
				}

				if (phyPlayers[i].m_pos.y < -100)
				{

					phyPlayers[i].IsFall = true;
					phyPlayers[i].m_keyW = false;
					phyPlayers[i].m_keyA = false;
					phyPlayers[i].m_keyS = false;
					phyPlayers[i].m_keyD = false;
					phyPlayers[i].is_jump = false;
					phyPlayers[i].m_pos.y = -100;
					players[i].pos.y = phyPlayers[i].m_pos.y;
					phyPlayers[i].gravity = 0.0f;
					SendFall(i);
					phyPlayers[i].is_reset = true;
				}
				
		\
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				if (phyPlayers[i].is_reset == true)
				{
					phyPlayers[i].ResetTimeCount += 1;
					if (phyPlayers[i].ResetTimeCount > 180.0f)
					{
						phyPlayers[i].IsFall = false;
						phyPlayers[i].is_reset = false;
						phyPlayers[i].ResetTimeCount = 0.0f;
						phyPlayers[i].is_jump = false;
						phyPlayers[i].is_jumpanim = false;
						phyPlayers[i].is_hitted = false;
						phyPlayers[i].is_attack = false;
						//	phyPlayers[i].is_Skill = false;
						phyPlayers[i].is_Skillanim = false;
						phyPlayers[i].m_keyW = false;
						phyPlayers[i].m_keyA = false;
						phyPlayers[i].m_keyS = false;
						phyPlayers[i].m_keyD = false;

						int BlockTmpX;
						int BlockTmpZ;
						bool resetoverlap = false;
						do {
							resetoverlap = false;
							BlockTmpX = uid(dre);
							BlockTmpZ = uid(dre);
							for (int j = 0; j < numOfCls; ++j) {
								if (i != j && BlockTmpX * 10 + BlockTmpZ * 2 == phyPlayers[j].ResetLocation) {
									resetoverlap = true;
								}
							}
						} while (blocks[BlockTmpX * 10 + BlockTmpZ * 2].pos.y < -50 || resetoverlap);

						phyPlayers[i].ResetLocation = BlockTmpX * 10 + BlockTmpZ * 2;
						phyPlayers[i].m_pos.x = BlockTmpX * 400;
						phyPlayers[i].m_pos.y = 100.0f;
						phyPlayers[i].m_pos.z = BlockTmpZ * 400;
						if (phyPlayers[i].lifecnt > 0)
							phyPlayers[i].lifecnt -= 1;
						if (phyPlayers[i].lifecnt == 0) {
							phyPlayers[i].IsDead = true;
						}
						g_boundaries[phyPlayers[i].TypeName]->Center = phyPlayers[i].m_pos;
				
						if (phyPlayers[i].lifecnt >= 0)
							SendReset(i, phyPlayers[i].lifecnt);

					}
				}

				if (phyPlayers[i].ResetLocation != -1)
				{
					phyPlayers[i].ResetLocationSaveCount += 1;
					if (phyPlayers[i].ResetLocationSaveCount > 60.0f) {
						phyPlayers[i].ResetLocation = -1;
						phyPlayers[i].ResetLocationSaveCount = 0.0f;
					}
				}

				if (phyPlayers[i].IsDead) {
					phyPlayers[i].m_pos.x = -1000;
					phyPlayers[i].m_pos.y = 30;
					phyPlayers[i].m_pos.z = -1000;
					players[i].pos.y = phyPlayers[i].m_pos.y;
					phyPlayers[i].gravity = 0.0f;
				}
			}
			Update(phyPlayers, elapsedTime);

			for (int i = 0; i < 2; ++i) {
				g_boundaries["igloo" + std::to_string(i)]->Extents.x = OriginIglooExtens.x * 2.0 / 10.0;
				g_boundaries["igloo" + std::to_string(i)]->Extents.y = OriginIglooExtens.y * 2.0 / 10.0 + 0.05;
				g_boundaries["igloo" + std::to_string(i)]->Extents.z = OriginIglooExtens.z * 2.0 / 10.0;
				g_boundaries["igloo" + std::to_string(i)]->Center.x = TempiglooLocation[i] / 3 * 400;
				g_boundaries["igloo" + std::to_string(i)]->Center.y = blocks[TempiglooLocation[i] / 3 * 10 + TempiglooLocation[i] % 3 * 2].pos.y + 60;
				g_boundaries["igloo" + std::to_string(i)]->Center.z = TempiglooLocation[i] % 3 * 400;
			}
			for (int i = 0; i < 4; ++i) {
				g_boundaries["snowman" + std::to_string(i)]->Extents.x = OriginSnowmanExtens.x * (0.6);
				g_boundaries["snowman" + std::to_string(i)]->Extents.y = OriginSnowmanExtens.y * (0.6);
				g_boundaries["snowman" + std::to_string(i)]->Extents.z = OriginSnowmanExtens.z * (0.6);
				g_boundaries["snowman" + std::to_string(i)]->Center.x = TempSnowmanLocation[i] / 5 * 200;
				g_boundaries["snowman" + std::to_string(i)]->Center.y = blocks[TempSnowmanLocation[i]].pos.y + 20;
				g_boundaries["snowman" + std::to_string(i)]->Center.z = TempSnowmanLocation[i] % 5 * 200;
			}
			for (int j = 0; j < numOfCls; ++j) {		//상호작용
				if (phyPlayers[j].IsInteract && !phyPlayers[j].is_Skill) {
					if (g_boundaries["igloo0"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						phyPlayers[j].m_pos = DirectX::XMFLOAT3(g_boundaries["igloo1"]->Center.x, g_boundaries["igloo1"]->Center.y + 10, g_boundaries["igloo1"]->Center.z);
					}
					else if (g_boundaries["igloo1"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						phyPlayers[j].m_pos = DirectX::XMFLOAT3(g_boundaries["igloo0"]->Center.x, g_boundaries["igloo0"]->Center.y + 10, g_boundaries["igloo0"]->Center.z);
					}
					else if (g_boundaries["snowman0"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						if (!HideInSnowman[0] || phyPlayers[j].SnowmanNum == 0) {
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = -1 - phyPlayers[j].SnowmanNum;
							HideInSnowman[0] = !HideInSnowman[0];
						}
					}
					else if (g_boundaries["snowman1"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						if (!HideInSnowman[1] || phyPlayers[j].SnowmanNum == 1) {
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = -phyPlayers[j].SnowmanNum;
							HideInSnowman[1] = !HideInSnowman[1];
						}
					}
					else if (g_boundaries["snowman2"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						if (!HideInSnowman[2] || phyPlayers[j].SnowmanNum == 2) {
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = 1 - phyPlayers[j].SnowmanNum;
							HideInSnowman[2] = !HideInSnowman[2];
						}
					}
					else if (g_boundaries["snowman3"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
						if (!HideInSnowman[3] || phyPlayers[j].SnowmanNum == 3) {
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = 2 - phyPlayers[j].SnowmanNum;
							HideInSnowman[3] = !HideInSnowman[3];
						}
					}
				}
				phyPlayers[j].IsInteract = false;
			}

			//블록 block
			UpdateBlock(blocks);
			for (int j = 0; j < numOfCls; ++j) { //블록 충돌체크
				for (int i = 0; i < 25; ++i) {
					if (BlockCheck(i)) {
						if (blocks[i].destuctioncnt != 3)
							g_boundaries["icecube" + std::to_string(i)]->Center = blocks[i].pos;
						if (phyPlayers[j].CurrentIcecube == -1 &&
							phyPlayers[j].m_pos.x - 10 <= blocks[i].pos.x + 54 && phyPlayers[j].m_pos.x + 10 >= blocks[i].pos.x - 54 &&
							phyPlayers[j].m_pos.z - 10 <= blocks[i].pos.z + 54 && phyPlayers[j].m_pos.z + 10 >= blocks[i].pos.z - 54 &&
							phyPlayers[j].m_pos.y <= blocks[i].pos.y + 60 && blocks[i].destuctioncnt != 3)
						{
							//	if (tmp1[j] == -1 && g_boundaries["icecube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]]) && phyPlayers[j].GetPos().y >= 30) {
							phyPlayers[j].CurrentIcecube = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
							phyPlayers[j].is_jumpanim = false;
							if (phyPlayers[j].CurrentBlockNum != i) {
								phyPlayers[j].CurrentBlockNum = i;
								phyPlayers[j].TimeWhileBlock = 0;
							}
							if (!BlockIn) {
								IsShake[i] = true;
								IsDown[i] = true;
								BlockIn = true;
							}
						}
						if (phyPlayers[j].CurrentIcecube != -1 &&
							!(phyPlayers[j].m_pos.x - 10 <= blocks[phyPlayers[j].CurrentIcecube].pos.x + 54 && phyPlayers[j].m_pos.x + 10 >= blocks[phyPlayers[j].CurrentIcecube].pos.x - 54 &&
								phyPlayers[j].m_pos.z - 10 <= blocks[phyPlayers[j].CurrentIcecube].pos.z + 54 && phyPlayers[j].m_pos.z + 10 >= blocks[phyPlayers[j].CurrentIcecube].pos.z - 54 &&
								phyPlayers[j].m_pos.y <= blocks[phyPlayers[j].CurrentIcecube].pos.y + 70))
						{
							//	if (tmp1[j] != -1 && !(g_boundaries["icecube" + std::to_string(tmp1[j])]->Intersects(*g_boundaries[TypeName[j]]))) {
							BlockIn = false;
							//if (!phyPlayers[j].is_jump)
							//   Gravity += 0.05;
							phyPlayers[j].CurrentIcecube = -1;
						}
					}
					else {
						g_boundaries["snowcube" + std::to_string(i)]->Center = blocks[i].pos;
						/*if (tmp2[j] == -1 && g_boundaries["snowcube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]]) && phyPlayers[j].GetPos().y >= blocks[i].pos.y + 60)*/
						if (phyPlayers[j].CurrentSnowcube == -1 &&
							phyPlayers[j].m_pos.x - 10 <= blocks[i].pos.x + 54 && phyPlayers[j].m_pos.x + 10 >= blocks[i].pos.x - 54 &&
							phyPlayers[j].m_pos.z - 10 <= blocks[i].pos.z + 54 && phyPlayers[j].m_pos.z + 10 >= blocks[i].pos.z - 54 &&
							phyPlayers[j].m_pos.y <= blocks[i].pos.y + 60)
						{
							phyPlayers[j].CurrentSnowcube = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
							phyPlayers[j].is_jumpanim = false;
							IsShake[i] = true;
							IsDown[i] = true;
							if (phyPlayers[j].CurrentBlockNum != i) {
								phyPlayers[j].CurrentBlockNum = i;
								phyPlayers[j].TimeWhileBlock = 0;
							}
						}
						/*if (tmp2[j] != -1 && !(g_boundaries["snowcube" + std::to_string(tmp2[j])]->Intersects(*g_boundaries[TypeName[j]])))*/
						if (phyPlayers[j].CurrentSnowcube != -1 &&
							!(phyPlayers[j].m_pos.x - 10 <= blocks[phyPlayers[j].CurrentSnowcube].pos.x + 54 && phyPlayers[j].m_pos.x + 10 >= blocks[phyPlayers[j].CurrentSnowcube].pos.x - 54 &&
								phyPlayers[j].m_pos.z - 10 <= blocks[phyPlayers[j].CurrentSnowcube].pos.z + 54 && phyPlayers[j].m_pos.z + 10 >= blocks[phyPlayers[j].CurrentSnowcube].pos.z - 54 &&
								phyPlayers[j].m_pos.y <= blocks[phyPlayers[j].CurrentSnowcube].pos.y + 70))
						{
							for (int k = 0; k < numOfCls; ++k) {
								if (j != k && phyPlayers[j].CurrentSnowcube != phyPlayers[k].CurrentSnowcube)
									IsDown[phyPlayers[j].CurrentSnowcube] = false;
							}
							phyPlayers[j].CurrentSnowcube = -1;
						}
					}
				}
			}

			for (int i = 0; i < numOfCls; ++i) {	//눈사람에 숨어을때 위치 동기화
				if (phyPlayers[i].SnowmanNum >= 0) {
					phyPlayers[i].m_pos.x = TempSnowmanLocation[phyPlayers[i].SnowmanNum] / 5 * 200;
					phyPlayers[i].m_pos.y = blocks[TempSnowmanLocation[phyPlayers[i].SnowmanNum]].pos.y + 60;
					phyPlayers[i].m_pos.z = TempSnowmanLocation[phyPlayers[i].SnowmanNum] % 5 * 200;
				}
				if (blocks[TempSnowmanLocation[phyPlayers[i].SnowmanNum]].destuctioncnt == 3 || (phyPlayers[i].SnowmanNum >= 0 && phyPlayers[i].is_hitted)) {
					phyPlayers[i].IsHide = false;
					HideInSnowman[phyPlayers[i].SnowmanNum] = false;
					phyPlayers[i].SnowmanNum = -1;
				}
			}

			//블록 재생성
			{		// 꼭지점
				if (blocks[9].destuctioncnt == 3 && blocks[3].destuctioncnt == 3) {
					int tmp = uid2(dre);
					if (tmp == 0) {
						blocks[3].destuctioncnt = 0;
						g_boundaries["icecube3"]->Center = blocks[3].pos;
						g_boundaries["icecube3"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[9].destuctioncnt = 0;
						g_boundaries["icecube9"]->Center = blocks[9].pos;
						g_boundaries["icecube9"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[1].destuctioncnt == 3 && blocks[5].destuctioncnt == 3) {
					int tmp = uid2(dre);
					if (tmp == 0) {
						blocks[1].destuctioncnt = 0;
						g_boundaries["icecube1"]->Center = blocks[1].pos;
						g_boundaries["icecube1"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[5].destuctioncnt = 0;
						g_boundaries["icecube5"]->Center = blocks[5].pos;
						g_boundaries["icecube5"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[19].destuctioncnt == 3 && blocks[23].destuctioncnt == 3) {
					int tmp = uid2(dre);
					if (tmp == 0) {
						blocks[23].destuctioncnt = 0;
						g_boundaries["icecube23"]->Center = blocks[23].pos;
						g_boundaries["icecube23"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[19].destuctioncnt = 0;
						g_boundaries["icecube19"]->Center = blocks[19].pos;
						g_boundaries["icecube19"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[21].destuctioncnt == 3 && blocks[15].destuctioncnt == 3) {
					int tmp = uid2(dre);
					if (tmp == 0) {
						blocks[21].destuctioncnt = 0;
						g_boundaries["icecube21"]->Center = blocks[21].pos;
						g_boundaries["icecube21"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[15].destuctioncnt = 0;
						g_boundaries["icecube15"]->Center = blocks[15].pos;
						g_boundaries["icecube15"]->Extents = OriginBlockExtents;
					}
				}
			}
			{		//변
				if (blocks[9].destuctioncnt == 3 && blocks[13].destuctioncnt == 3 && blocks[19].destuctioncnt == 3) {
					int tmp = uid(dre);
					if (tmp == 0) {
						blocks[13].destuctioncnt = 0;
						g_boundaries["icecube13"]->Center = blocks[13].pos;
						g_boundaries["icecube13"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[9].destuctioncnt = 0;
						g_boundaries["icecube9"]->Center = blocks[9].pos;
						g_boundaries["icecube9"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 2) {
						blocks[19].destuctioncnt = 0;
						g_boundaries["icecube19"]->Center = blocks[19].pos;
						g_boundaries["icecube19"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[1].destuctioncnt == 3 && blocks[7].destuctioncnt == 3 && blocks[3].destuctioncnt == 3) {
					int tmp = uid(dre);
					if (tmp == 0) {
						blocks[3].destuctioncnt = 0;
						g_boundaries["icecube3"]->Center = blocks[3].pos;
						g_boundaries["icecube3"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[1].destuctioncnt = 0;
						g_boundaries["icecube1"]->Center = blocks[1].pos;
						g_boundaries["icecube1"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 2) {
						blocks[7].destuctioncnt = 0;
						g_boundaries["icecube7"]->Center = blocks[7].pos;
						g_boundaries["icecube7"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[21].destuctioncnt == 3 && blocks[17].destuctioncnt == 3 && blocks[23].destuctioncnt == 3) {
					int tmp = uid(dre);
					if (tmp == 0) {
						blocks[23].destuctioncnt = 0;
						g_boundaries["icecube23"]->Center = blocks[23].pos;
						g_boundaries["icecube23"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[21].destuctioncnt = 0;
						g_boundaries["icecube21"]->Center = blocks[21].pos;
						g_boundaries["icecube21"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 2) {
						blocks[17].destuctioncnt = 0;
						g_boundaries["icecube17"]->Center = blocks[17].pos;
						g_boundaries["icecube17"]->Extents = OriginBlockExtents;
					}
				}
				if (blocks[5].destuctioncnt == 3 && blocks[11].destuctioncnt == 3 && blocks[15].destuctioncnt == 3) {
					int tmp = uid(dre);
					if (tmp == 0) {
						blocks[5].destuctioncnt = 0;
						g_boundaries["icecube5"]->Center = blocks[5].pos;
						g_boundaries["icecube5"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[11].destuctioncnt = 0;
						g_boundaries["icecube11"]->Center = blocks[11].pos;
						g_boundaries["icecube11"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 2) {
						blocks[15].destuctioncnt = 0;
						g_boundaries["icecube15"]->Center = blocks[15].pos;
						g_boundaries["icecube15"]->Extents = OriginBlockExtents;
					}
				}
				SendBlockPacket(*blocks);
			}
			{		//중앙
				if (blocks[7].destuctioncnt == 3 && blocks[11].destuctioncnt == 3
					&& blocks[13].destuctioncnt == 3 && blocks[17].destuctioncnt == 3) {
					int tmp = uid3(dre);
					if (tmp == 0) {
						blocks[11].destuctioncnt = 0;
						g_boundaries["icecube11"]->Center = blocks[11].pos;
						g_boundaries["icecube11"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 1) {
						blocks[7].destuctioncnt = 0;
						g_boundaries["icecube7"]->Center = blocks[7].pos;
						g_boundaries["icecube7"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 2) {
						blocks[13].destuctioncnt = 0;
						g_boundaries["icecube13"]->Center = blocks[13].pos;
						g_boundaries["icecube13"]->Extents = OriginBlockExtents;
					}
					else if (tmp == 3) {
						blocks[17].destuctioncnt = 0;
						g_boundaries["icecube17"]->Center = blocks[17].pos;
						g_boundaries["icecube17"]->Extents = OriginBlockExtents;
					}
				}
			}
			//1
			for (int i = 0; i < numOfCls; ++i)
			{
				//cout << phyPlayers[0].m_pos.y <<", "<< phyPlayers[0].gravity << endl;
				if (phyPlayers[i].is_jump == true && phyPlayers[i].is_hitted == true) {
					phyPlayers[i].m_pos.y += (HITTED_JUMP_POWER + phyPlayers[i].gravity);
				}
				else if (phyPlayers[i].is_jump == true)
				{
					phyPlayers[i].m_pos.y += (JUMP_POWER + phyPlayers[i].gravity);
				}
				{		//우박 hail
					if (!GameOverCheck) {
						if (phyPlayers[i].CurrentBlockNum != -1) {
							if (phyPlayers[i].TimeWhileBlock >= 180) {		//한 블록에 3초 이상
								float time = phyPlayers[i].TimeWhileBlock / 60.0 - 3;
								hails[i].SetPosCalc(phyPlayers[i].CurrentBlockNum / 5 * 200, 200, phyPlayers[i].CurrentBlockNum % 5 * 200, time);
								g_boundaries["hail" + std::to_string(i)]->Center = hails[i].GetPos();
								for (int j = 0; j < 5; ++j) {
									if (g_boundaries["hail" + std::to_string(j)]->Intersects(*g_boundaries[phyPlayers[i].TypeName]) && phyPlayers[i].SnowmanNum == -1) {
										phyPlayers[i].is_hitted = true;
										SendHitPalyer(players[i].Character_type, phyPlayers[i].GetPos());
										if (phyPlayers[i].TypeName == "husky") {
											phyPlayers[i].is_Skill = false;
											phyPlayers[i].is_Skillanim = false;
											phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 4.0f);
											phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 4.0f));
										}
										float SubX = hails[j].GetPos().x - phyPlayers[i].GetPos().x;
										float SubZ = hails[j].GetPos().z - phyPlayers[i].GetPos().z;
										if (SubX < -9) {
											if (SubZ < -9)
												phyPlayers[i].hitted_dir = 1;
											else if (SubZ >= -9 && SubZ < 9)
												phyPlayers[i].hitted_dir = 2;
											else if (SubZ >= 9)
												phyPlayers[i].hitted_dir = 3;
										}
										else 	if (SubX >= -9 && SubX <= 9) {
											if (SubZ < -9)
												phyPlayers[i].hitted_dir = 0;
											else if (SubZ >= -9 && SubZ < 9)
												phyPlayers[i].hitted_dir = 0;
											else if (SubZ >= 9)
												phyPlayers[i].hitted_dir = 4;
										}
										else 	if (SubX > 9) {
											if (SubZ < -9)
												phyPlayers[i].hitted_dir = 7;
											else if (SubZ >= -9 && SubZ < 9)
												phyPlayers[i].hitted_dir = 6;
											else if (SubZ >= 9)
												phyPlayers[i].hitted_dir = 5;
										}
									}
								}
								if (hails[i].GetPos().y <= -90) {
									hails[i].SetPos(-1000, -1000, -1000);
								}
							}
							else if (phyPlayers[i].TimeWhileBlock == 0) {
								hails[i].SetPos(-1000, -1000, -1000);
							}
							phyPlayers[i].TimeWhileBlock += 1;
						}
					}
				}
			}

			SendHail(*hails);
			//   Update(phyPlayers)
			{	//skill
				for (int i = 0; i < numOfCls; ++i) {
					if (phyPlayers[i].is_Skill && phyPlayers[i].SnowmanNum == -1 && !phyPlayers[i].IsSkillEnd) {
						if (phyPlayers[i].TypeName == "Penguin") {
							phyPlayers[i].IsSkillCool = true;
							if (phyPlayers[i].m_pos.y <= 200) {
								phyPlayers[i].gravity = 0.0f;
								phyPlayers[i].m_pos.y += 2.0f;
								phyPlayers[i].is_jump = true;
							}
							SendPenguinSkill(true);
							if (phyPlayers[i].SkillTime >= 180) {
								phyPlayers[i].is_Skill = false;
								phyPlayers[i].is_Skillanim = false;
								phyPlayers[i].SkillTime = 0;
								phyPlayers[i].IsSkillCool = true;
								SendPenguinSkill(false);
								phyPlayers[i].IsSkillEnd = true;
							}
						}
						else if (phyPlayers[i].TypeName == "husky") {
							phyPlayers[i].IsSkillCool = true;
							static float HittedIdx = -1;
							if (phyPlayers[i].m_pos.y < 40) {
								phyPlayers[i].m_pos.y += 10.0f;
							}
							if (phyPlayers[i].SkillTime >= 10 && phyPlayers[i].SkillTime <= 80) {
								switch (static_cast<int>(phyPlayers[i].dir)) {
								case 0:
									phyPlayers[i].SetKeyW(true);
									phyPlayers[i].SetSpeed(5.0f * BASE_SPEED);
									break;
								case 1:
									phyPlayers[i].SetKeyW(true);
									phyPlayers[i].SetKeyD(true);
									phyPlayers[i].SetCrossSpeed(cos(45) * 7.5f);
									break;
								case 2:
									phyPlayers[i].SetKeyD(true);
									phyPlayers[i].SetSpeed(5.0f * BASE_SPEED);
									break;
								case 3:
									phyPlayers[i].SetKeyD(true);
									phyPlayers[i].SetKeyS(true);
									phyPlayers[i].SetCrossSpeed(cos(45) * 7.5f);
									break;
								case 4:
									phyPlayers[i].SetKeyS(true);
									phyPlayers[i].SetSpeed(5.0f * BASE_SPEED);
									break;
								case 5:
									phyPlayers[i].SetKeyS(true);
									phyPlayers[i].SetKeyA(true);
									phyPlayers[i].SetCrossSpeed(cos(45) * 7.5f);
									break;
								case 6:
									phyPlayers[i].SetKeyA(true);
									phyPlayers[i].SetSpeed(5.0f * BASE_SPEED);
									break;
								case 7:
									phyPlayers[i].SetKeyW(true);
									phyPlayers[i].SetKeyA(true);
									phyPlayers[i].SetCrossSpeed(cos(45) * 7.5f);
									break;
								}

								for (int j = 0; j < numOfCls; ++j) {
									if (i != j && g_boundaries["husky"]->Intersects(*g_boundaries[phyPlayers[j].TypeName])) {
										phyPlayers[j].hitted_dir = phyPlayers[i].dir;
										phyPlayers[j].is_hitted = true;
										phyPlayers[j].SetHittedSpeed(3.0f * 1.5f);
										HittedIdx = j;
									}
								}
								SendHuskySkill(true);
							}
							else if (phyPlayers[i].SkillTime > 80) {
								phyPlayers[i].is_Skill = false;
								phyPlayers[i].SkillTime = 0;
								phyPlayers[i].SetKeyW(false);
								phyPlayers[i].SetKeyA(false);
								phyPlayers[i].SetKeyS(false);
								phyPlayers[i].SetKeyD(false);
								phyPlayers[i].SetSpeed(BASE_SPEED + 0.2f * 4.0f);
								phyPlayers[i].SetCrossSpeed(cos(45) * (BASE_SPEED + 0.2f * 4.0f));
								phyPlayers[i].IsSkillEnd = true;
								if (HittedIdx != -1)
									phyPlayers[HittedIdx].SetHittedSpeed(1.0f * 1.5f);
								HittedIdx = -1;
								SendHuskySkill(false);
							}
							else {
								if (phyPlayers[i].CurrentSnowcube != -1) {
									if (20 < phyPlayers[i].GetPos().y && phyPlayers[i].GetPos().y < 30) {
										phyPlayers[i].m_pos.y = 50;
									}
								}
							}
						}
						else if (phyPlayers[i].TypeName == "ArcticFox") {
							phyPlayers[i].IsSkillCool = true;
							SendFoxSkill(true);
						}
						else if (phyPlayers[i].TypeName == "Seal") {
							phyPlayers[i].IsSkillCool = true;
							phyPlayers[i].is_hitted = false;
							SendSealSkill(true);
						}
						else if (phyPlayers[i].TypeName == "PolarBear") {
							phyPlayers[i].IsSkillCool = true;
							if (phyPlayers[i].CurrentIcecube != -1 || phyPlayers[i].CurrentSnowcube != -1) {
								int TempBlockIdx = max(phyPlayers[i].CurrentIcecube, phyPlayers[i].CurrentSnowcube);
								PolarbearSkill(TempBlockIdx);
								if (TempBlockIdx % 5 != 0) {
									PolarbearSkill(TempBlockIdx - 1);
									if (TempBlockIdx / 5 != 0) {
										PolarbearSkill(TempBlockIdx - 6);
									}
								}
								if (TempBlockIdx % 5 != 4) {
									PolarbearSkill(TempBlockIdx + 1);
									if (TempBlockIdx / 5 != 4) {
										PolarbearSkill(TempBlockIdx + 6);
									}
								}
								if (TempBlockIdx / 5 != 0) {
									PolarbearSkill(TempBlockIdx - 5);
									if (TempBlockIdx % 5 != 4) {
										PolarbearSkill(TempBlockIdx - 4);
									}
								}
								if (TempBlockIdx / 5 != 4) {
									PolarbearSkill(TempBlockIdx + 5);
									if (TempBlockIdx % 5 != 0) {
										PolarbearSkill(TempBlockIdx + 4);
									}
								}
							}
							phyPlayers[i].is_Skill = false;
							phyPlayers[i].SkillTime = 0;
							phyPlayers[i].IsSkillEnd = true;
							SendBearSkill(BearSkill);
							BearSkill = !BearSkill;
						}
						phyPlayers[i].SkillTime += 1;
						if (phyPlayers[i].SkillTime >= 300) {
							phyPlayers[i].is_Skill = false;
							phyPlayers[i].is_Skillanim = false;
							phyPlayers[i].SkillTime = 0;
							phyPlayers[i].IsSkillCool = true;
							SendFoxSkill(false);
							SendSealSkill(false);
							phyPlayers[i].IsSkillEnd = true;
						}
					}
					else {
						phyPlayers[i].is_Skill = false;
						//	phyPlayers[i].is_Skillanim = false;
					}
					if (phyPlayers[i].IsSkillCool) {
						phyPlayers[i].SkillCoolTime += 1;
						if (phyPlayers[i].SkillCoolTime >= 600) {
							phyPlayers[i].IsSkillCool = false;
							phyPlayers[i].SkillCoolTime = 0;
							phyPlayers[i].IsSkillEnd = false;
						}
					}
				}
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				players[i].anim = phyPlayers[i].GetAnimType();
				players[i].pos = phyPlayers[i].m_pos;
				players[i].dir = phyPlayers[i].dir;
				players[i].IsHide = phyPlayers[i].IsHide;
				players[i].SnowmanNum = phyPlayers[i].SnowmanNum;
				players[i].IsSkillCool = phyPlayers[i].IsSkillCool;

				g_boundaries[phyPlayers[i].TypeName]->Center = players[i].pos;
				/*		if (TypeName[i] == "Penguin") {
							g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 3;
						}
						else {
							g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 1.5;
						}*/

			}
			SendPos(*players);
			SendAnim(*players);


			//fpsTimer = steady_clock::now();
			//cout << "LastFrame:" << duration_cast<ms>(FramePerSec).count() << "ms | FPS: " << FramePerSec.count() * FPS << endl;
			elapsedTime = 0;
			CalcTime += 1;

			if (CalcTime % 60 == 0)
			{
				SendTime(CalcTime);
			}

			/*	for (int i = 0; i < 5; ++i) {
					cout <<i<<": " << who_lose[i] << endl;
				}*/

			for (int i = 0; i < numOfCls; ++i)
			{
				if (phyPlayers[i].IsDead == true)
				{
					who_lose[lose_count] = i;
					lose_count += 1;
				}
			}

			if ((numOfCls - 1) == lose_count)
			{
				for (int i = 0; i < numOfCls; ++i)
				{
					for (int j = 0; j < lose_count; ++j)
					{
						if (i == who_lose[j])
						{
							GameOverCheck = false;
							break;
						}
					}

					if (GameOverCheck == true)
					{
						GameOverTimeCount += 1;
						if (GameOverTimeCount > 300 /** numOfCls*/)
						{
							SendGameOverPacket(i);
							cout << "Winner is " << i << endl;
							phyPlayers = *new std::vector <Player>;
							//	phyPlayers.clear();
							//	g_boundaries.clear();


							while (!g_MsgQueue.empty())
								g_MsgQueue.pop();

							for (int i = 0; i < numOfCls; ++i)
							{
								g_playerReadyInfo[i].ready = 0;
								g_initialPos[i].Character_type = CHARACTER_NONE;
								who_lose[i] = 0;
							}

							g_isPlaying = false;
							lose_count = 0;
							phyPlayers = *new std::vector <Player>;
							//delete[] blocks;

							return;
						}
						else if (GameOverTimeCount > 270 /** numOfCls*/) {
							phyPlayers[i].is_Skillanim = false;
						}
						else {
							phyPlayers[i].is_Skillanim = true;
							phyPlayers[i].IsHide = false;
							phyPlayers[i].SetPos(DirectX::XMFLOAT3(400, 30, 400));
							phyPlayers[i].gravity = 0.0f;
							phyPlayers[i].SnowmanNum = -1;
							IsShake[12] = false;
							IsDown[12] = false;
						}
					}

					GameOverCheck = true;
				}
			}
			lose_count = 0;
		}
	}
}