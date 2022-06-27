#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
public:
	virtual void Initialize() override;
	virtual void OnResize() override;
public:
	virtual bool Enter() override;
	virtual void Exit() override;

	virtual void Update(const float& fDeltaTime) override;
	virtual void Render() override;
	virtual void RenderUI() override;
};

