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
		g_pFramework->m_pNetwork->Send(CS_READY);
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

