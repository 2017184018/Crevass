#include "pch.h"
#include "SceneController.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "ApplicationContext.h"
#include "Character.h"
#include "CREVASS.h"

#include "MainFramework.h"
#include "Network.h"
#include "SoundManager.h"


using namespace Core;

LobbyController::LobbyController(LobbyScene* myScene) :
	m_MyScene(myScene)
{
}

void LobbyController::Update(const float deltaT)
{
	MouseCallback();
	HandleInput(deltaT);
}

void LobbyController::Initready()
{
	is_ready = false;
}

void LobbyController::HandleInput(const float deltaT)
{
	if (!Inactive) {
		static bool up = true;
		if (GetAsyncKeyState(VK_UP) & 0x8000 && !is_ready) {
			if (up) {
				SoundManager::GetApp()->PlaySoundOnce(L"Button.mp3", SoundManager::CHANNEL_ID::SELECT_BUTTON, 2.0f);
				if (CREVASS::GetApp()->currchar == 4) {
					CREVASS::GetApp()->currchar = 0;
				}
				else {
					++CREVASS::GetApp()->currchar;
				}
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
						if (CREVASS::GetApp()->currchar == 0) {
							++CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
						if (CREVASS::GetApp()->currchar == 1) {
							++CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
						if (CREVASS::GetApp()->currchar == 2) {
							++CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_POLARBEAR) {
						if (CREVASS::GetApp()->currchar == 3) {
							++CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
						if (CREVASS::GetApp()->currchar == 4) {
							CREVASS::GetApp()->currchar = 0;
							i = -1;
							continue;
						}
					}
				}

				up = false;

				switch (CREVASS::GetApp()->currchar)
				{
				case 0:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_HUSKY);
					break;

				case 1:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_PENGUIN);
					break;

				case 2:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_FOX);
					break;

				case 3:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_POLARBEAR);
					break;

				case 4:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_SEAL);
					break;
				}
			}
		}
		else {
			up = true;
		}
		//	CREVASS::GetApp()->m_Camera->Walk(50);

		static bool down = true;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && !is_ready) {
			if (down) {
				SoundManager::GetApp()->PlaySoundOnce(L"Button.mp3", SoundManager::CHANNEL_ID::SELECT_BUTTON, 2.0f);
				if (CREVASS::GetApp()->currchar == 0) {
					CREVASS::GetApp()->currchar = 4;
				}
				else {
					--CREVASS::GetApp()->currchar;
				}
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
						if (CREVASS::GetApp()->currchar == 0) {
							CREVASS::GetApp()->currchar = 4;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
						if (CREVASS::GetApp()->currchar == 1) {
							--CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
						if (CREVASS::GetApp()->currchar == 2) {
							--CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_POLARBEAR) {
						if (CREVASS::GetApp()->currchar == 3) {
							--CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
						if (CREVASS::GetApp()->currchar == 4) {
							--CREVASS::GetApp()->currchar;
							i = -1;
							continue;
						}
					}
				}
				down = false;

				switch (CREVASS::GetApp()->currchar)
				{
				case 0:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_HUSKY);
					break;

				case 1:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_PENGUIN);
					break;

				case 2:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_FOX);
					break;

				case 3:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_POLARBEAR);
					break;

				case 4:
					g_pFramework->m_pNetwork->Send(CS_CHOOSE_SEAL);
					break;

				}
			}
			//	CREVASS::GetApp()->m_Camera->Walk(-50);
		}
		else {
			down = true;
		}
		//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		//	CREVASS::GetApp()->m_Camera->Strafe(-50);
		//}
		//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		//	CREVASS::GetApp()->m_Camera->Strafe(50);
		//}

		if (InputHandler::IsKeyUp('B'))
		{
			switch (CREVASS::GetApp()->currchar) {
			case 0: {
				bool IsCharacterSeleted = false;
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {

					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
						IsCharacterSeleted = true;
						SoundManager::GetApp()->PlaySoundOnce(L"Overlap.mp3", SoundManager::CHANNEL_ID::PLAYER_OVERlAP, 2.5f);
						break;
					}

				}
				if (!IsCharacterSeleted && !is_ready) {
					SoundManager::GetApp()->PlaySoundOnce(L"Select.mp3", SoundManager::CHANNEL_ID::SELECT_OK, 2.0f);
					g_pFramework->m_pNetwork->Send(CS_READY_HUSKY);
					is_ready = true;
				}
				else {
					is_ready = false;
				}

				break;
			}
			case 1: {
				bool IsCharacterSeleted = false;
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
						IsCharacterSeleted = true;
						SoundManager::GetApp()->PlaySoundOnce(L"Overlap.mp3", SoundManager::CHANNEL_ID::PLAYER_OVERlAP, 2.5f);
						break;
					}
				}
				if (!IsCharacterSeleted) {
					SoundManager::GetApp()->PlaySoundOnce(L"Select.mp3", SoundManager::CHANNEL_ID::SELECT_OK, 2.0f);
					g_pFramework->m_pNetwork->Send(CS_READY_PENGUIN);
					is_ready = true;
				}
				else {
					is_ready = false;
				}
				break;
			}
			case 2: {
				bool IsCharacterSeleted = false;
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
						IsCharacterSeleted = true;
						SoundManager::GetApp()->PlaySoundOnce(L"Overlap.mp3", SoundManager::CHANNEL_ID::PLAYER_OVERlAP, 2.5f);
						break;
					}
				}
				if (!IsCharacterSeleted) {
					SoundManager::GetApp()->PlaySoundOnce(L"Select.mp3", SoundManager::CHANNEL_ID::SELECT_OK, 2.0f);
					g_pFramework->m_pNetwork->Send(CS_READY_FOX);
					is_ready = true;
				}
				else {
					is_ready = false;
				}
				break;
			}
			case 3: {
				bool IsCharacterSeleted = false;
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_POLARBEAR) {
						IsCharacterSeleted = true;
						SoundManager::GetApp()->PlaySoundOnce(L"Overlap.mp3", SoundManager::CHANNEL_ID::PLAYER_OVERlAP, 2.5f);
						break;
					}
				}
				if (!IsCharacterSeleted) {
					SoundManager::GetApp()->PlaySoundOnce(L"Select.mp3", SoundManager::CHANNEL_ID::SELECT_OK, 2.0f);
					g_pFramework->m_pNetwork->Send(CS_READY_POLARBEAR);
					is_ready = true;
				}
				else {
					is_ready = false;
				}
				break;
			}
			case 4: {
				bool IsCharacterSeleted = false;
				for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
					if (Core::g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
						IsCharacterSeleted = true;
						SoundManager::GetApp()->PlaySoundOnce(L"Overlap.mp3", SoundManager::CHANNEL_ID::PLAYER_OVERlAP, 2.5f);
						break;
					}
				}
				if (!IsCharacterSeleted) {
					SoundManager::GetApp()->PlaySoundOnce(L"Select.mp3", SoundManager::CHANNEL_ID::SELECT_OK, 2.0f);
					g_pFramework->m_pNetwork->Send(CS_READY_SEAL);
					is_ready = true;
				}
				else {
					is_ready = false;
				}
				break;
			}
			}
		}
	}
}

void LobbyController::MouseCallback()
{

}

GameplayController::GameplayController(GameplayScene* myScene) :
	m_MyScene(myScene)
{
}

void GameplayController::Update(const float deltaT)
{
	MouseCallback();
	HandleInput(deltaT);
}

void GameplayController::HandleInput(const float deltaT)
{

}


GameresultController::GameresultController(GameresultScene* myScene) :
	m_MyScene(myScene)
{
}

void GameresultController::Update(const float deltaT)
{
	MouseCallback();
	HandleInput(deltaT);
}

void GameresultController::HandleInput(const float deltaT)
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene(SceneType::Lobby);
	}

}

void GameresultController::MouseCallback()
{

}

