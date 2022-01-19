#pragma once
class Scene abstract
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual bool	InitializeScene() = 0;
	virtual void	UpdateScene(const float& fTimeDelta) = 0;
	virtual void	ReleaseScene() = 0;
};

