#include "pch.h"
#include "GameresultScene.h"

void GameresultScene::Initialize()
{
	m_SceneController = new GameresultController(this);
}

void GameresultScene::OnResize()
{
}

bool GameresultScene::Enter()
{
	cout << "Gameresult Scene" << endl;

	return false;
}

void GameresultScene::Exit()
{
	cout << "exitGameresultScene===========================================" << endl << endl;
}

void GameresultScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update(fDeltaTime);
}

void GameresultScene::Render()
{
}

void GameresultScene::RenderUI()
{
}
