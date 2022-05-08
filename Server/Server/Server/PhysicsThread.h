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
string TypeName[3];
bool IsFall[3] = { false,false,false };

void Update(vector<Player>& player)
{
	float speed = 1.0f; ;
	float crossspeed = sqrt(2) / 2;
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
	}
}

void Hitted_Pos_Update(Player& player,int tyname_num) {
	float speed = 1.0f; ;
	float crossspeed = sqrt(2) / 2;
		float saveX = 0;
		float saveZ = 0;
			//위
			if (static_cast<int>(player.hitted_dir) == 0) {
				player.m_pos.z += speed;
				player.dir = DIR_DOWN;
				saveZ = speed;
			}
			else if (static_cast<int>(player.hitted_dir) == 2)
			{
				player.m_pos.x += speed;
				player.dir = DIR_LEFT;
				saveZ = speed;
			}
			else if (static_cast<int>(player.hitted_dir) == 4)
			{
				player.m_pos.z -= speed;
				player.dir = DIR_UP;
				saveZ = speed;
			}
			else if (static_cast<int>(player.hitted_dir) == 6)
			{
				player.m_pos.x -= speed;
				player.dir = DIR_RIGHT;
				saveZ = speed;
			}
		
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
		phyPlayers[i].Hit_BB.Radius = g_boundaries[TypeName[i]]->Extents.z/2;

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



			//float  speed = 1.0f; 
			//speed += Gravity;			//점프 하면 중력 0.1로 최고점에서 닿은 순간부터 중력에 0.05씩 더하기



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

						for (int i = 0; i < numOfCls; i++) {
							if (phyMsg.id != i) {
								if (phyPlayers[phyMsg.id].Hit_BB.Intersects(*g_boundaries[TypeName[i]])) {
									cout << "hit!" << endl;
									phyPlayers[i].is_hitted = true;
									//처맞은 방향
									phyPlayers[i].hitted_dir = phyPlayers[phyMsg.id].dir;
									phyPlayers[phyMsg.id].Hit_BB.Center = DirectX::XMFLOAT3(0, 0, 0);
								}
							}
						}
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
						cout << static_cast<int>(players[i].dir) << endl;
						players[i].anim = phyPlayers[i].GetAnimType();

						g_boundaries[TypeName[i]]->Center = players[i].pos;

					}

					SendPos(*players);
					SendAnim(*players);
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
						//phyPlayers[i].is_jump = false;
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

				if (phyPlayers[i].is_hitted == true)
				{
					phyPlayers[i].hittedTimeCount += 1.0f;
					if (phyPlayers[i].hittedTimeCount > 60.0f)
					{
						phyPlayers[i].hittedTimeCount = 0.0f;
						phyPlayers[i].is_hitted = false;
						players[i].anim = phyPlayers[i].GetAnimType();
						SendAnim(*players);
					}
					else {
						cout << "호출" << endl;
						Hitted_Pos_Update(phyPlayers[i], i);
						players[i].id = i;
						players[i].pos.x = phyPlayers[i].m_pos.x;
						players[i].pos.y = phyPlayers[i].m_pos.y;
						players[i].pos.z = phyPlayers[i].m_pos.z;
						players[i].dir = phyPlayers[i].dir;
						SendPos(*players);
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
			for (int i = 0; i < numOfCls; ++i)
			{
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
						if (phyPlayers[i].m_pos.y > 10 && phyPlayers[i].is_jump == false && blocks[tmp1[i]].destuctioncnt != 3) {
							phyPlayers[i].m_pos.y = blocks[tmp1[i]].pos.y + 60;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
						else {
							phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
					}
					else {
						if (phyPlayers[i].m_pos.y += phyPlayers[i].gravity - blocks[tmp2[i]].pos.y >= 50 &&
							phyPlayers[i].is_jump == false && !IsFall[i]) {
							phyPlayers[i].m_pos.y = blocks[tmp2[i]].pos.y + 60;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
						else {
							phyPlayers[i].m_pos.y += phyPlayers[i].gravity;
							players[i].pos.y = phyPlayers[i].m_pos.y;
						}
					}
				}
				if (phyPlayers[i].m_pos.y < -100) {
					IsFall[i] = true;
					phyPlayers[i].m_pos.y = -100;
					players[i].pos.y = phyPlayers[i].m_pos.y;
					//fall 애니메이션
					players[i].anim = ANIM_FALL;
					SendAnim(*players);
				}
			}
			SendPos(*players);

			UpdateBlock(blocks);
			for (int j = 0; j < numOfCls; ++j) { //블록 충돌체크
				for (int i = 0; i < 25; ++i) {
					if (BlockCheck(i)) {
						g_boundaries["icecube" + std::to_string(i)]->Center = blocks[i].pos;
						if (tmp1[j] == -1 && g_boundaries["icecube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp1[j] = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
							if (!BlockIn) {
								IsShake[i] = true;
								IsDown[i] = true;
								BlockIn = true;
							}
						}
						if (tmp1[j] != -1 && !(g_boundaries["icecube" + std::to_string(tmp1[j])]->Intersects(*g_boundaries[TypeName[j]]))) {
							BlockIn = false;
							//if (!phyPlayers[j].is_jump)
							//	Gravity += 0.05;
							tmp1[j] = -1;
						}
					}
					else {
						if (tmp2[j] == -1 && g_boundaries["snowcube" + std::to_string(i)]->Intersects(*g_boundaries[TypeName[j]])) {
							tmp2[j] = i;
							phyPlayers[j].gravity = 0.0f;
							phyPlayers[j].is_jump = false;
							IsShake[i] = true;
							IsDown[i] = true;
						}
						if (tmp2[j] != -1 && !(g_boundaries["snowcube" + std::to_string(tmp2[j])]->Intersects(*g_boundaries[TypeName[j]]))) {  // or 물에 떨어지면
							IsDown[tmp2[j]] = false;
							//if (!phyPlayers[j].is_jump)
							//	Gravity += 0.05;
							tmp2[j] = -1;
						}
					}
				}
			}
			SendBlockPacket(*blocks);

			//for (int i = 0; i < numOfCls; ++i)
			//{
			//	if (phyPlayers[i].is_jump == true)
			//	{
			//		phyPlayers[i].m_pos.y += (JUMP_POWER + phyPlayers[i].gravity);
			//	}
			//}
			Update(phyPlayers);

			for (int i = 0; i < numOfCls; ++i)
			{
				players[i].pos = phyPlayers[i].m_pos;
			}
			SendPos(*players);

			//for (int i = 0; i < numOfCls; ++i)
			//{
			//}
			//fpsTimer = steady_clock::now();
			//cout << "LastFrame:" << duration_cast<ms>(FramePerSec).count() << "ms | FPS: " << FramePerSec.count() * FPS << endl;
			elapsedTime = 0;
		}
	}
	phyPlayers.clear();
}