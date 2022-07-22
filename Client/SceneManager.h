#pragma once
#include "Singleton.h"

class Scene;
class SceneManager : public TemplateSingleton<SceneManager>
{
private:
	template<class NewScene>
	void CreateScene(SceneType sceneType)
	{
		Scene* scene = new NewScene;

		
		scene->Initialize();
		m_Scenes[static_cast<int>(sceneType)] = scene;
	}

public:
	explicit SceneManager();
	virtual ~SceneManager();

	void InitializeScenes();

public:

	void	ChangeScene();
	void	ChangeScene(SceneType sceneType);

	void	EnterScene(SceneType sceneType);
	void	ExitScene();

	void	UpdateScene(const float& deltaT);
	void	RenderScene();
	void	RenderUI();
	void	WriteShadow();

	Scene* GetCurScene() const;

private:
	std::array<Scene*, static_cast<int>(SceneType::Count)> m_Scenes;
	UINT m_CurScene;
};

