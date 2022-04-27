#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "LobbyScene.h"
#include "GameplayScene.h"
#include "GameresultScene.h"

SceneManager::SceneManager() :
	m_CurScene(0)
{
}

SceneManager::~SceneManager()
{
	for (auto& s : m_Scenes) {
		s->Exit();
		SAFE_DELETE_PTR(s);
	}
	m_CurScene = -1;
}

void SceneManager::InitializeScenes()
{
	CreateScene<LobbyScene>(SceneType::Lobby);
	CreateScene<GameplayScene>(SceneType::GamePlay);
	CreateScene<GameresultScene>(SceneType::GameResult);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	if (m_CurScene == -1)
		cout << "NullScene" << endl;

	m_Scenes[m_CurScene]->Exit();

	m_CurScene = static_cast<int>(sceneType);
	m_Scenes[m_CurScene]->Enter();
}


void SceneManager::EnterScene(SceneType sceneType)
{
	m_CurScene = static_cast<UINT>(sceneType);
	m_Scenes[m_CurScene]->Enter();

}

void SceneManager::ExitScene()
{
	m_Scenes[m_CurScene]->Exit();
}

void SceneManager::ChangeScene()
{
	if (m_CurScene == -1)
		cout << "NullScene" << endl;

	m_Scenes[m_CurScene]->Exit();

	m_CurScene += 1;

	if (m_CurScene >= static_cast<int>(SceneType::Count))
		m_CurScene = 1;

	m_Scenes[m_CurScene]->Enter();
}


void SceneManager::UpdateScene(const float& deltaT)
{
	m_Scenes[m_CurScene]->Update(deltaT);
}

void SceneManager::RenderScene()
{
	m_Scenes[m_CurScene]->Render();
}

Scene* SceneManager::GetCurScene() const
{
	return m_Scenes[m_CurScene];
}
