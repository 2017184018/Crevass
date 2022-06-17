#pragma once
#include "pch.h"
#include "Global.h"
#include "Hail.h"
#include <string>
#include <random>

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,2 };

bool IsRight[25];
UINT ShakeCnt[25];
bool IsDown[25];
bool IsShake[25];
int tmp1[3] = { -1,-1,-1 };
bool BlockIn = false;
float Gravity = 0.1;
int tmp2[3] = { -1,-1,-1 };
string TypeName[3];
bool IsFall[3] = { false,false,false };
Block blocks[25];
DirectX::XMFLOAT3 temp(0.f, 0.f, 0.f);
bool dir_switch[3];
bool IsInteract[3] = { false,false,false };		//상호작용 키 눌렀는지
bool IsInteracting[3] = { false,false,false };	//상호작용 중인지
bool HideInSnowman[4] = { false,false,false,false };	//n번째 눈사람에 누군가 숨어있는지
Hail hails[5];

int CalcTime = 0;

void Update(vector<Player>& player)
{
	float speed = 1.0f * 1.5f;
	//float crossspeed = sqrt(2) / 2;
	float crossspeed = cos(45) * 1.5f;
	for (int i = 0; i < numOfCls; ++i)
	{
		float saveX = 0;
		float saveZ = 0;
		if (player[i].is_hitted == false) {
			if (player[i].GetKeyW() && player[i].GetKeyA()) {
				player[i].m_pos.z += crossspeed;
				player[i].m_pos.x -= crossspeed;
				player[i].dir = DIR_UP_LEFT;
				saveX = -crossspeed;
				saveZ = crossspeed;
			}
			else if (player[i].GetKeyW() && player[i].GetKeyD()) {
				player[i].m_pos.z += crossspeed;
				player[i].m_pos.x += crossspeed;
				player[i].dir = DIR_UP_RIGHT;
				saveX = crossspeed;
				saveZ = crossspeed;
			}
			else if (player[i].GetKeyS() && player[i].GetKeyD()) {
				player[i].m_pos.z -= crossspeed;
				player[i].m_pos.x += crossspeed;
				player[i].dir = DIR_DOWN_RIGHT;
				saveX = crossspeed;
				saveZ = -crossspeed;

			}
			else if (player[i].GetKeyS() && player[i].GetKeyA()) {
				player[i].m_pos.z -= crossspeed;
				player[i].m_pos.x -= crossspeed;
				player[i].dir = DIR_DOWN_LEFT;
				saveX = -crossspeed;
				saveZ = -crossspeed;
			}
			else {
				if (player[i].GetKeyW())
				{
					player[i].m_pos.z += speed;
					player[i].dir = DIR_UP;
					saveZ = speed;
				}
				if (player[i].GetKeyS())
				{
					player[i].m_pos.z -= speed;
					player[i].dir = DIR_DOWN;
					saveZ = -speed;
				}
				if (player[i].GetKeyA())
				{
					player[i].m_pos.x -= speed;
					player[i].dir = DIR_LEFT;
					saveX = -speed;
				}
				if (player[i].GetKeyD())
				{
					player[i].m_pos.x += speed;
					player[i].dir = DIR_RIGHT;
					saveX = speed;
				}
			}
		}

		else {
			//위

			if (static_cast<int>(player[i].hitted_dir) == 0) {
				player[i].m_pos.z += speed;
				player[i].dir = DIR_DOWN;
				saveZ = speed;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 2)
			{
				player[i].m_pos.x += speed;
				player[i].dir = DIR_LEFT;
				saveZ = speed;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 4)
			{
				player[i].m_pos.z -= speed;
				player[i].dir = DIR_UP;
				saveZ = speed;
			}
			else if (static_cast<int>(player[i].hitted_dir) == 6)
			{
				player[i].m_pos.x -= speed;
				player[i].dir = DIR_RIGHT;
				saveZ = speed;
			}
		}
		for (int j = 0; j < numOfCls; ++j) {
			if (i != j) {
				g_boundaries[TypeName[i]]->Center.x += saveX;
				g_boundaries[TypeName[i]]->Center.z += saveZ;
				if (g_boundaries[TypeName[i]]->Intersects(*g_boundaries[TypeName[j]])) {
					player[i].m_pos.x -= saveX;
					player[i].m_pos.z -= saveZ;
					g_boundaries[TypeName[i]]->Center.x -= saveX;
					g_boundaries[TypeName[i]]->Center.z -= saveZ;

				}
			}
		}

		for (int j = 0; j < 25; ++j) {
			if (phyPlayers[i].m_pos.y < blocks[j].pos.y + 60) {
				g_boundaries[TypeName[i]]->Center.x += saveX;
				g_boundaries[TypeName[i]]->Center.z += saveZ;
				if (BlockCheck(j)) {
					if (g_boundaries["icecube" + std::to_string(j)]->Intersects(*g_boundaries[TypeName[i]])) {
						player[i].m_pos.x -= saveX;
						player[i].m_pos.y -= 1.f;
						player[i].m_pos.z -= saveZ;
						g_boundaries[TypeName[i]]->Center.x -= saveX;
						g_boundaries[TypeName[i]]->Center.z -= saveZ;
					}
				}
				else {
					if (g_boundaries["snowcube" + std::to_string(j)]->Intersects(*g_boundaries[TypeName[i]])) {
						player[i].m_pos.x -= saveX;
						//player[i].m_pos.y -= 1.f;
						player[i].m_pos.z -= saveZ;
						g_boundaries[TypeName[i]]->Center.x -= saveX;
						g_boundaries[TypeName[i]]->Center.z -= saveZ;
					}
				}
			}
		}
	}
}

void Hitted_Pos_Update(Player& player, int tyname_num, float anitime) {


	float speed = 1.0f * 1.5f;
	float crossspeed = cos(45) * 1.5f;
	float saveX = 0;
	float saveZ = 0;
	//위
	if (static_cast<int>(player.hitted_dir) == 0) {
		player.m_pos.z += speed * HITTED_POWER;
		player.dir = DIR_DOWN;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 2)
	{
		player.m_pos.x += speed * HITTED_POWER;
		player.dir = DIR_LEFT;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 4)
	{
		player.m_pos.z -= speed * HITTED_POWER;
		player.dir = DIR_UP;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 6)
	{
		player.m_pos.x -= speed * HITTED_POWER;
		player.dir = DIR_RIGHT;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 1)
	{
		player.m_pos.x += crossspeed * HITTED_POWER;
		player.m_pos.z += crossspeed * HITTED_POWER;
		player.dir = DIR_DOWN_LEFT;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 3)
	{
		player.m_pos.x += crossspeed * HITTED_POWER;
		player.m_pos.z -= crossspeed * HITTED_POWER;
		player.dir = DIR_UP_LEFT;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 5)
	{
		player.m_pos.x -= crossspeed * HITTED_POWER;
		player.m_pos.z -= crossspeed * HITTED_POWER;
		player.dir = DIR_UP_RIGHT;
		saveZ = speed * HITTED_POWER;
	}
	else if (static_cast<int>(player.hitted_dir) == 7)
	{
		player.m_pos.x -= crossspeed * HITTED_POWER;
		player.m_pos.z += crossspeed * HITTED_POWER;
		player.dir = DIR_DOWN_RIGHT;
		saveZ = speed * HITTED_POWER;
	}

	//if (anitime < 30) {
	//   player.m_pos.y += speed;
	//}
	//else {
	//   player.m_pos.y -= speed;
	//}

	for (int j = 0; j < numOfCls; ++j) {
		if (tyname_num != j) {
			g_boundaries[TypeName[tyname_num]]->Center.x += saveX;
			g_boundaries[TypeName[tyname_num]]->Center.z += saveZ;
			if (g_boundaries[TypeName[tyname_num]]->Intersects(*g_boundaries[TypeName[j]])) {

				player.m_pos.x -= saveX;
				player.m_pos.z -= saveZ;
				g_boundaries[TypeName[tyname_num]]->Center.x -= saveX;
				g_boundaries[TypeName[tyname_num]]->Center.z -= saveZ;
			}
		}
	}
	for (int j = 0; j < 25; ++j) {
		if (player.m_pos.y < blocks[j].pos.y + 60) {
			g_boundaries[TypeName[tyname_num]]->Center.x += saveX;
			g_boundaries[TypeName[tyname_num]]->Center.z += saveZ;
			if (BlockCheck(j)) {
				if (g_boundaries["icecube" + std::to_string(j)]->Intersects(*g_boundaries[TypeName[tyname_num]])) {
					player.m_pos.x -= saveX;
					player.m_pos.z -= saveZ;
					g_boundaries[TypeName[tyname_num]]->Center.x -= saveX;
					g_boundaries[TypeName[tyname_num]]->Center.z -= saveZ;
				}
			}
			else {
				if (g_boundaries["snowcube" + std::to_string(j)]->Intersects(*g_boundaries[TypeName[tyname_num]])) {
					player.m_pos.x -= saveX;
					player.m_pos.z -= saveZ;
					g_boundaries[TypeName[tyname_num]]->Center.x -= saveX;
					g_boundaries[TypeName[tyname_num]]->Center.z -= saveZ;
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
			shake(&object[i], i);   //block
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++object[i].destuctioncnt;
			if (object[i].destuctioncnt == 3) {
				g_boundaries["icecube" + std::to_string(i)]->Center = DirectX::XMFLOAT3(-0, -1000, 0);
				g_boundaries["icecube" + std::to_string(i)]->Extents = DirectX::XMFLOAT3(0, 0, 0);
			}
		}
	}
}

void ProcessClients()
{
	g_player_lock.lock();
	Pro_Player players[3] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]} };

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

	auto OriginSnowmanExtens = g_boundaries["snowman0"]->Extents;
	auto OriginIglooExtens = g_boundaries["igloo0"]->Extents;

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
		phyPlayers[i].Hit_BB.Radius = g_boundaries[TypeName[i]]->Extents.z / 2;

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

		if (/*FramePerSec.count() >= 1*/elapsedTime > 1000.0f / 60.0f)
		{
			g_MsgQueueLock.lock();
			phyMsgQueue = g_MsgQueue;
			g_MsgQueueLock.unlock();

			g_MsgQueueLock.lock();
			while (!g_MsgQueue.empty())
				g_MsgQueue.pop();
			g_MsgQueueLock.unlock();



			//float  speed = 1.0f; 
			//speed += Gravity;         //점프 하면 중력 0.1로 최고점에서 닿은 순간부터 중력에 0.05씩 더하기



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
							temp.z = g_boundaries[TypeName[phyMsg.id]]->Extents.z;
							g_boundaries[TypeName[phyMsg.id]]->Extents.z = g_boundaries[TypeName[phyMsg.id]]->Extents.x;
							g_boundaries[TypeName[phyMsg.id]]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = true;
						}
						break;
					case DIR_DOWN:

						phyPlayers[phyMsg.id].SetKeyS(phyMsg.isPushed);
						if (dir_switch) {
							temp.z = g_boundaries[TypeName[phyMsg.id]]->Extents.z;
							g_boundaries[TypeName[phyMsg.id]]->Extents.z = g_boundaries[TypeName[phyMsg.id]]->Extents.x;
							g_boundaries[TypeName[phyMsg.id]]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = false;
						}
						break;
					case DIR_LEFT:
						phyPlayers[phyMsg.id].SetKeyA(phyMsg.isPushed);
						if (dir_switch) {
							temp.z = g_boundaries[TypeName[phyMsg.id]]->Extents.z;
							g_boundaries[TypeName[phyMsg.id]]->Extents.z = g_boundaries[TypeName[phyMsg.id]]->Extents.x;
							g_boundaries[TypeName[phyMsg.id]]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = false;
						}
						break;
					case DIR_RIGHT:
						phyPlayers[phyMsg.id].SetKeyD(phyMsg.isPushed);
						if (!dir_switch) {
							temp.z = g_boundaries[TypeName[phyMsg.id]]->Extents.z;
							g_boundaries[TypeName[phyMsg.id]]->Extents.z = g_boundaries[TypeName[phyMsg.id]]->Extents.x;
							g_boundaries[TypeName[phyMsg.id]]->Extents.x = temp.z;
							dir_switch[phyMsg.id] = true;
						}
						break;
					case KEY_ATTACK:
						phyPlayers[phyMsg.id].is_attack = true;
						//hit box  위치 갱신 
						phyPlayers[phyMsg.id].Hit_BB.Center = phyPlayers[phyMsg.id].GetPos();
						if (phyPlayers[phyMsg.id].dir == 0) {
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[TypeName[phyMsg.id]]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 2)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[TypeName[phyMsg.id]]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 4)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[TypeName[phyMsg.id]]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 6)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[TypeName[phyMsg.id]]->Extents.z;
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 1)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);
						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 3)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x += g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);

						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 5)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);
							phyPlayers[phyMsg.id].Hit_BB.Center.z -= g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);

						}
						else if (static_cast<int>(phyPlayers[phyMsg.id].dir) == 7)
						{
							phyPlayers[phyMsg.id].Hit_BB.Center.x -= g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);
							phyPlayers[phyMsg.id].Hit_BB.Center.z += g_boundaries[TypeName[phyMsg.id]]->Extents.z * cos(45);

						}

						for (int i = 0; i < numOfCls; i++) {
							if (phyMsg.id != i) {
								if (phyPlayers[phyMsg.id].Hit_BB.Intersects(*g_boundaries[TypeName[i]])) {
									cout << "hit!" << endl;
									phyPlayers[i].is_hitted = true;
									phyPlayers[i].is_jump = true;
									//처맞은 방향
									phyPlayers[i].hitted_dir = phyPlayers[phyMsg.id].dir;
									phyPlayers[phyMsg.id].Hit_BB.Center = DirectX::XMFLOAT3(0, 0, 0);
								}
							}
						}
						break;
					case KEY_JUMP:
						if (phyPlayers[phyMsg.id].m_pos.y > -100 && phyPlayers[phyMsg.id].SnowmanNum == -1)
							phyPlayers[phyMsg.id].is_jump = true;
						break;
					case KEY_INTERACT:
						IsInteract[phyMsg.id] = true;
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
					phyPlayers[i].AttackTimeCount += 1.0f;
					if (phyPlayers[i].AttackTimeCount > 60.0f)
					{
						phyPlayers[i].AttackTimeCount = 0.0f;
						phyPlayers[i].is_attack = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						//SendAnim(*players);   /
					}
				}

				if (phyPlayers[i].is_jump == true)
				{
					phyPlayers[i].JumpTimeCount += 1.0f;
					if (phyPlayers[i].JumpTimeCount > 60.0f)
					{
						phyPlayers[i].JumpTimeCount = 0.0f;
						//phyPlayers[i].is_jump = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						//SendAnim(*players);   /
					}
				}

				if (phyPlayers[i].is_hitted == true)
				{
					phyPlayers[i].hittedTimeCount += 1.0f;
					if (phyPlayers[i].hittedTimeCount > 60.0f)
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
			/*   for (int i = 0; i < numOfCls; ++i)
			   {
				  players[i].id = i;
				  players[i].anim = phyPlayers[i].anim;
			   }
			   if (phyMsg.type == TYPE_ANIMATION)
			   {
				  players[phyMsg.id].anim = phyPlayers[phyMsg.id].GetAnimType();
				  SendAnim(*players);
			   }*/
			for (int i = 0; i < numOfCls; ++i)
			{
				//아이스 , 스노우 아무것도 출동하지 않을때 
				if (tmp1[i] == -1 && tmp2[i] == -1)
				{
					phyPlayers[i].gravity -= 0.02f;
				}
			}

			for (int i = 0; i < numOfCls; ++i) {
				if (tmp1[i] == -1 && tmp2[i] == -1) {
					phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
					players[i].pos.y = phyPlayers[i].m_pos.y;
				}
				else {
					if (tmp1[i] != -1) {
						if (phyPlayers[i].m_pos.y > 10 && phyPlayers[i].is_jump == false && blocks[tmp1[i]].destuctioncnt != 3 && phyPlayers[i].m_pos.y <= 30) {   //점프 이상하면 마지막 조건 지우기
							phyPlayers[i].m_pos.y = blocks[tmp1[i]].pos.y + 60;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
						else {
							phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
					}
					else {
						if (phyPlayers[i].m_pos.y - blocks[tmp2[i]].pos.y >= 50 &&
							phyPlayers[i].is_jump == false && !IsFall[i] && phyPlayers[i].m_pos.y - blocks[tmp2[i]].pos.y <= 70)
						{//점프 이상하면 마지막 조건 지우기
							phyPlayers[i].m_pos.y = blocks[tmp2[i]].pos.y + 60;
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
					IsFall[i] = true;
					phyPlayers[i].m_pos.y = -100;
					players[i].pos.y = phyPlayers[i].m_pos.y;
					SendFall(i);
					phyPlayers[i].is_reset = true;
				}
				g_boundaries[TypeName[i]]->Center = players[i].pos;
				if (TypeName[i] == "Penguin") {
					g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 3;
				}
				else {
					g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 1.5;
				}
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				if (phyPlayers[i].is_reset == true)
				{
					phyPlayers[i].ResetTimeCount += 1;
					if (phyPlayers[i].ResetTimeCount > 180.0f)
					{
						IsFall[i] = false;
						phyPlayers[i].is_reset = false;
						phyPlayers[i].ResetTimeCount = 0.0f;
						phyPlayers[i].is_jump = false;
						phyPlayers[i].is_hitted = false;
						phyPlayers[i].is_attack = false;
						int BlockTmpX;
						int BlockTmpZ;
						do {
							BlockTmpX = uid(dre);
							BlockTmpZ = uid(dre);
						} while (blocks[BlockTmpX * 10 + BlockTmpZ * 2].pos.y < -50);
						phyPlayers[i].m_pos.x = BlockTmpX * 400;
						phyPlayers[i].m_pos.y = 100.0f;
						phyPlayers[i].m_pos.z = BlockTmpZ * 400;

						//phyPlayers[i].anim = ANIM_IDLE;
						g_boundaries[TypeName[i]]->Center = phyPlayers[i].m_pos;
						if (TypeName[i] == "Penguin") {
							g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 3;
						}
						else {
							g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 1.5;
						}
						SendReset(i);
					}
				}
			}
			Update(phyPlayers);

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
				if (IsInteract[j]) {
					if (g_boundaries["igloo0"]->Intersects(*g_boundaries[TypeName[j]])) {
						IsInteracting[j] = true;
						phyPlayers[j].m_pos = g_boundaries["igloo1"]->Center;
						IsInteracting[j] = false;
					}
					else if (g_boundaries["igloo1"]->Intersects(*g_boundaries[TypeName[j]])) {
						IsInteracting[j] = true;
						phyPlayers[j].m_pos = g_boundaries["igloo0"]->Center;
						IsInteracting[j] = false;
					}
					else if (g_boundaries["snowman0"]->Intersects(*g_boundaries[TypeName[j]])) {
						if (!HideInSnowman[0] || phyPlayers[j].SnowmanNum == 0) {
							IsInteracting[j] = !IsInteracting[j];
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = -1 - phyPlayers[j].SnowmanNum;
							HideInSnowman[0] = !HideInSnowman[0];
						}
					}
					else if (g_boundaries["snowman1"]->Intersects(*g_boundaries[TypeName[j]])) {
						if (!HideInSnowman[1] || phyPlayers[j].SnowmanNum == 1) {
							IsInteracting[j] = !IsInteracting[j];
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = -phyPlayers[j].SnowmanNum;
							HideInSnowman[1] = !HideInSnowman[1];
						}
					}
					else if (g_boundaries["snowman2"]->Intersects(*g_boundaries[TypeName[j]])) {
						if (!HideInSnowman[2] || phyPlayers[j].SnowmanNum == 2) {
							IsInteracting[j] = !IsInteracting[j];
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = 1 - phyPlayers[j].SnowmanNum;
							HideInSnowman[2] = !HideInSnowman[2];
						}
					}
					else if (g_boundaries["snowman3"]->Intersects(*g_boundaries[TypeName[j]])) {
						if (!HideInSnowman[3] || phyPlayers[j].SnowmanNum == 3) {
							IsInteracting[j] = !IsInteracting[j];
							phyPlayers[j].IsHide = !phyPlayers[j].IsHide;
							phyPlayers[j].SnowmanNum = 2 - phyPlayers[j].SnowmanNum;
							HideInSnowman[3] = !HideInSnowman[3];
						}
					}
					IsInteract[j] = false;
				}
			}

			UpdateBlock(blocks);
			for (int j = 0; j < numOfCls; ++j) { //블록 충돌체크
				for (int i = 0; i < 25; ++i) {
					if (BlockCheck(i)) {
						if (blocks[i].destuctioncnt != 3)
							g_boundaries["icecube" + std::to_string(i)]->Center = blocks[i].pos;
						if (tmp1[j] == -1 && g_boundaries["icecube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp1[j] = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
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
						if (tmp1[j] != -1 && !(g_boundaries["icecube" + std::to_string(tmp1[j])]->Intersects(*g_boundaries[TypeName[j]]))) {
							BlockIn = false;
							//if (!phyPlayers[j].is_jump)
							//   Gravity += 0.05;
							tmp1[j] = -1;
						}
					}
					else {
						g_boundaries["snowcube" + std::to_string(i)]->Center = blocks[i].pos;
						if (tmp2[j] == -1 && g_boundaries["snowcube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp2[j] = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
							IsShake[i] = true;
							IsDown[i] = true;
							if (phyPlayers[j].CurrentBlockNum != i) {
								phyPlayers[j].CurrentBlockNum = i;
								phyPlayers[j].TimeWhileBlock = 0;
							}
						}
						if (tmp2[j] != -1 && !(g_boundaries["snowcube" + std::to_string(tmp2[j])]->Intersects(*g_boundaries[TypeName[j]]))) {  // or 물에 떨어지면
							IsDown[tmp2[j]] = false;

							cout << j << "check" << endl;
							//if (!phyPlayers[j].is_jump)
							//   Gravity += 0.05;
							tmp2[j] = -1;
						}
					}
				}
			}
			SendBlockPacket(*blocks);

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
				if (phyPlayers[i].CurrentBlockNum != -1) {
					if (phyPlayers[i].TimeWhileBlock >= 180) {		//한 블록에 3초 이상
						float time = phyPlayers[i].TimeWhileBlock / 60.0 - 3;
						hails[i].SetPos(
							phyPlayers[i].CurrentBlockNum / 5 * 200 + hails[i].GetVelo().x * time + 0.5 * hails[i].GetAccel().x * time * time,
							200 + hails[i].GetVelo().y * time + 0.5 * hails[i].GetAccel().y * time * time,
							phyPlayers[i].CurrentBlockNum % 5 * 200 + hails[i].GetVelo().z * time + 0.5 * hails[i].GetAccel().z * time * time
						);
						g_boundaries["hail" + std::to_string(i)]->Center = hails[i].GetPos();
						for (int j = 0; j < 5; ++j) {
							if (g_boundaries["hail" + std::to_string(j)]->Intersects(*g_boundaries[TypeName[i]])) {
								phyPlayers[i].is_hitted = true;
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
					}
					else if (phyPlayers[i].TimeWhileBlock == 0) {
						hails[i].SetPos(-1000, -1000, -1000);
					}
					phyPlayers[i].TimeWhileBlock += 1;
				}
			}
			SendHail(*hails);
			//   Update(phyPlayers);

			for (int i = 0; i < numOfCls; ++i) {	//눈사람에 숨어을때 위치 동기화
				if (phyPlayers[i].SnowmanNum >= 0) {
					phyPlayers[i].m_pos.x = TempSnowmanLocation[phyPlayers[i].SnowmanNum] / 5 * 200;
					phyPlayers[i].m_pos.y = blocks[TempSnowmanLocation[phyPlayers[i].SnowmanNum]].pos.y + 60;
					phyPlayers[i].m_pos.z = TempSnowmanLocation[phyPlayers[i].SnowmanNum] % 5 * 200;
				}
				if (blocks[TempSnowmanLocation[phyPlayers[i].SnowmanNum]].destuctioncnt == 3) {
					IsInteracting[i] = false;
					phyPlayers[i].IsHide = false;
					HideInSnowman[phyPlayers[i].SnowmanNum] = false;
					phyPlayers[i].SnowmanNum = -1;
				}
			}

			for (int i = 0; i < numOfCls; ++i)
			{
				players[i].anim = phyPlayers[i].GetAnimType();
				players[i].pos = phyPlayers[i].m_pos;
				players[i].dir = phyPlayers[i].dir;
				players[i].IsHide = phyPlayers[i].IsHide;
				players[i].SnowmanNum = phyPlayers[i].SnowmanNum;

				g_boundaries[TypeName[i]]->Center = players[i].pos;
				if (TypeName[i] == "Penguin") {
					g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 3;
				}
				else {
					g_boundaries[TypeName[i]]->Center.y += g_boundaries[TypeName[i]]->Extents.y / 1.5;
				}

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
		}
	}
	phyPlayers.clear();
}