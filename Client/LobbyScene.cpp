#include "pch.h"
#include "LobbyScene.h"

void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
}

bool LobbyScene::Enter()
{
	cout << "Lobby Scene" << endl;
	return false;
}

void LobbyScene::Exit()
{
}

void LobbyScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update(fDeltaTime);
}

void LobbyScene::Render()
{
}
