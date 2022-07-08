#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
public:
	virtual void Initialize() override;

public:
	virtual bool Enter() override;
	virtual void Exit() override;

	virtual void Update(const float& fDeltaTime) override;
	virtual void Render() override;
};

