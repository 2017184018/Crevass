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
	// ���ڷ� ���� pNewScene ��ü�� ���� ������ �Ҵ��մϴ�.
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
