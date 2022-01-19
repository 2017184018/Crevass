#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager() : m_pCurScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	m_pCurScene->ReleaseScene();
	SAFE_DELETE_PTR(m_pCurScene);
}

bool SceneManager::ChangeScene(Scene* pNewScene)
{
	// 인자로 들어온 pNewScene 객체를 현재 씬으로 할당합니다.
	if (m_pCurScene == nullptr)
		m_pCurScene = pNewScene;

	else
	{
		SAFE_DELETE_PTR(m_pCurScene);
		m_pCurScene = pNewScene;
	}

	return true;
}

void SceneManager::UpdateScene(const float& fTimeDelta)
{
	if (m_pCurScene != nullptr)
		m_pCurScene->UpdateScene(fTimeDelta);
}

Scene* SceneManager::GetCurScene() const
{
	return m_pCurScene;
}
