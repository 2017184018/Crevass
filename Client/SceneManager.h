#pragma once
#include "Singleton.h"

class Scene;
class SceneManager : public TemplateSingleton<SceneManager>
{
public:
	explicit SceneManager();
	virtual ~SceneManager();

public:
	bool	ChangeScene(Scene* pNewScene);
	void	UpdateScene(const float& fTimeDelta);
	Scene*	GetCurScene() const;

private:
	Scene* m_pCurScene;
};

