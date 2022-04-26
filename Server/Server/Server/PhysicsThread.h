#pragma once
#include "pch.h"
#include "Global.h"

void Update(vector<Player>& player)
{
	for (int i = 0; i < numOfCls; ++i)
	{
		if(player[i].GetKeyW())
		{
			player[i].m_pos.y += 1.0f;
			printf("%f\n", player[i].m_pos.y);
		}
		if (player[i].GetKeyS())
		{
			player[i].m_pos.y -= 1.0f;
		}
		if (player[i].GetKeyA())
		{
			player[i].m_pos.x -= 1.0f;
		}
		if (player[i].GetKeyD())
		{
			player[i].m_pos.x += 1.0f;
		}

	}
}

void ProcessClients()
{
	mutex player_lock;
	player_lock.lock();
	Pro_Player players[3] = { {g_initialPos[0]},{g_initialPos[1]},{g_initialPos[2]} };
	player_lock.unlock();


	std::cout << "make Physics thread!" << std::endl;
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

	while (true)
	{
		auto cur_Time = chrono::high_resolution_clock::now();
		elapsedTime += FpFloatMilliseconds(cur_Time - prev_Time).count();
		prev_Time = cur_Time;

		if (elapsedTime > 17)
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
				Update(phyPlayers);

				printf("%f\n", phyPlayers[0].m_pos.y);

				
				for (int i = 0; i < numOfCls; ++i)
				{
					players[i].id = i;
					players[i].pos.x = phyPlayers[i].m_pos.x;
					players[i].pos.y = phyPlayers[i].m_pos.y;
					players[i].pos.z = phyPlayers[i].m_pos.z;
				}


				SendPos(*players);
			}

		}
	}
	
}