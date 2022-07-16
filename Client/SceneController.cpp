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
	static bool up = true;
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (up) {
			if (CREVASS::GetApp()->currchar == 4) {
				CREVASS::GetApp()->currchar = 0;
			}
			else {
				++CREVASS::GetApp()->currchar;
			}
			up = false;
		}
		//	CREVASS::GetApp()->m_Camera->Walk(50);
	}
	else {
		up = true;
	}
	static bool down = true;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (down) {
			if (CREVASS::GetApp()->currchar == 0) {
				CREVASS::GetApp()->currchar = 4;
			}
			else {
				--CREVASS::GetApp()->currchar;
			}
			down = false;
		}
		//	CREVASS::GetApp()->m_Camera->Walk(-50);
	}
	else {
		down = true;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		CREVASS::GetApp()->m_Camera->Strafe(-50);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		CREVASS::GetApp()->m_Camera->Strafe(50);
	}
	//const std::map<std::string, UINT>& info = AppContext->m_RItemsMap["lobby"]->GetinstanceKeymap();
	//UINT SNum = info.begin()->second;
	//if (InputHandler::IsKeyUp(VK_F1))
	//{
	//	SceneManager::GetApp()->ChangeScene();
	//}
	//if (InputHandler::IsKeyUp('B'))
	//{

	//	switch (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex) {
	//	case 8:
	//	case 13:
	//		g_pFramework->m_pNetwork->Send(CS_READY_PENGUIN);
	//		break;
	//	case 14:
	//		g_pFramework->m_pNetwork->Send(CS_READY_HUSKY);
	//		break;
	//	case 15:
	//		g_pFramework->m_pNetwork->Send(CS_READY_POLARBEAR);
	//		break;
	//	case 16:
	//		g_pFramework->m_pNetwork->Send(CS_READY_FOX);
	//		break;
	//	case 17:
	//		g_pFramework->m_pNetwork->Send(CS_READY_SEAL);
	//		break;
	//	}
	//}

	//if (!Inactive) {
	//	//8 로비  13 펭귄  14 허스키  15 곰  16여우  17물개
	//	static bool one = true;
	//	if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//		cout << AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex << endl;
	//		if (one) {

	//			if (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 8 || AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 17) {
	//				AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex = 13;
	//				
	//			}
	//			else {
	//				++AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex;
	//			}
	//			one = false;
	//		}
	//	}
	//	else {
	//		one = true;
	//	}

	//	static bool one2 = true;
	//	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//		if (one2) {

	//			if (AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 8 || AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex == 13) {
	//				AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex = 17;
	//			}
	//			else {
	//				--AppContext->FindObject<GameObject>("lobby", "lobby0")->m_MaterialIndex;
	//			}
	//			one2 = false;
	//		}
	//	}
	//	else {
	//		one2 = true;
	//	}
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

	// 파티클 루프 테스트
	if (InputHandler::IsKeyUp('P')) {
		//AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(11))->BlockParticle();
		//AppContext->DisplayParticle("testParticle", "testParticle" + std::to_string(11), XMFLOAT3(0, 0, 0));
		AppContext->HiddenUI("ui_p", "ui_p");
		cout << "find == " << AppContext->FindObject<GameObject>("player_" + std::to_string(1) + "hp" + std::to_string(1), "player_" + std::to_string(1) + "hp" + std::to_string(1))->m_IsVisible << endl;
	}
	if (InputHandler::IsKeyUp('O')) {
		AppContext->HiddenParticle("testParticle", "testParticle" + std::to_string(11));
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

