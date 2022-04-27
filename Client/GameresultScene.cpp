#include "pch.h"
#include "GameresultScene.h"

void GameresultScene::Initialize()
{
	m_SceneController = new GameresultController(this);
}

bool GameresultScene::Enter()
{
	cout << "Gameresult Scene" << endl;

	return false;
}

void GameresultScene::Exit()
{
}

void GameresultScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update(fDeltaTime);
}

void GameresultScene::Render()
{
}
