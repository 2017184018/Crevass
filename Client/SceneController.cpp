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
		g_pFramework->m_pNetwork->Send(CS_READY);			//송신할 때 내 캐릭터 뭔지도 전송
	}

	if (!Inactive) {
		//8 로비  13 펭귄  14 허스키  15 곰  16여우  17물개
		static bool one = true;
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (one) {
				if (AppContext->m_RItemsVec[210]->m_MaterialIndex == 8 || AppContext->m_RItemsVec[210]->m_MaterialIndex == 17) {
					AppContext->m_RItemsVec[210]->m_MaterialIndex = 13;
				}
				else {
					++AppContext->m_RItemsVec[210]->m_MaterialIndex;
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
				if (AppContext->m_RItemsVec[210]->m_MaterialIndex == 8 || AppContext->m_RItemsVec[210]->m_MaterialIndex == 13) {
					AppContext->m_RItemsVec[210]->m_MaterialIndex = 17;
				}
				else {
					--AppContext->m_RItemsVec[210]->m_MaterialIndex;
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

