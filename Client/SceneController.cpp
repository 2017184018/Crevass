#include "pch.h"
#include "SceneController.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "ApplicationContext.h"

#include "CREVASS.h"

#include "MainFramework.h"
#include "Network.h"


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

void LobbyController::HandleInput(const float deltaT)
{
	if (InputHandler::IsKeyUp(VK_F1))
	{
		SceneManager::GetApp()->ChangeScene();
	}
	if (InputHandler::IsKeyUp('B'))
	{
		switch (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex) {
		case 8:
		case 13:
			g_pFramework->m_pNetwork->Send(CS_READY_PENGUIN);	
			break;
		case 14:
			g_pFramework->m_pNetwork->Send(CS_READY_HUSKY);
			break;
		case 15:
			g_pFramework->m_pNetwork->Send(CS_READY_POLARBEAR);
			break;
		case 16:
			g_pFramework->m_pNetwork->Send(CS_READY_FOX);
			break;
		case 17:
			g_pFramework->m_pNetwork->Send(CS_READY_SEAL);
			break;
		}
	}

	if (!Inactive) {
		//8 ·Îºñ  13 Æë±Ï  14 Çã½ºÅ°  15 °õ  16¿©¿ì  17¹°°³
		static bool one = true;
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			cout << AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex << endl;
			if (one) {
				if (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 8 || AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 17) {
					AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex = 13;
					
				}
				else {
					++AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex;
				}
				one = false;
			}
		}
		else {
			one = true;
		}

		static bool one2 = true;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			if (one2) {
				if (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 8 || AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 13) {
					AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex = 17;
				}
				else {
					--AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex;
				}
				one2 = false;
			}
		}
		else {
			one2 = true;
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
	if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
		SceneManager::GetApp()->ChangeScene();
	}

	//if (InputHandler::IsOverlap(VK_UP))
	//{
	//	CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction.x += 0.01f;
	//	cout << CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction.x << endl;
	//}
	//if (InputHandler::IsOverlap(VK_DOWN))
	//{
	//	CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction.x -= 0.01f;
	//	cout << CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction.x << endl;
	//}

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

