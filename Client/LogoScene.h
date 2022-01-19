#pragma once
#include "Scene.h"

class LogoScene final : public Scene
{
public:
	explicit LogoScene();
	virtual ~LogoScene();

public:
	virtual bool InitializeScene() override;
	virtual void UpdateScene(const float& fTimeDelta) override;
	virtual void ReleaseScene() override;

public:
	void BuildRenderItems();
	void BuildSkyCube();
};

