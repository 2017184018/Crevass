#include "pch.h"
#include "SceneController.h"
#include "InputHandler.h"
#include "SceneManager.h"

#include "CREVASS.h"

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
	// F1: Auto / F2: Manual
	//if (InputHandler::IsKeyUp(VK_F1))
	//{
	//	Service::GetApp()->Notify(EVENT_LOBBY_AUTOMATCH_REQUEST);
	//}
	//if (InputHandler::IsKeyUp(VK_F2))
	//{
	//	Service::GetApp()->Notify(EVENT_LOBBY_MANUALMATCH_REQUEST);
	//}

	//// F3: 방만들기 / F4: 방참여
	//if (InputHandler::IsKeyUp(VK_F3))
	//{
	//	Service::GetApp()->Notify(EVENT_LOBBY_MANUALMATCH_ROOM_MAKE);
	//}
	//if (InputHandler::IsKeyUp(VK_F4))
	//{
	//	int tempEnterRoomNumber = 0;
	//	cout << "Input Room-Number: ";
	//	cin >> tempEnterRoomNumber;
	//	Service::GetApp()->Notify(EVENT_LOBBY_MANUALMATCH_ROOM_JOIN, 1, tempEnterRoomNumber);
	//}


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

