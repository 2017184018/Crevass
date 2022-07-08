#pragma once

class IGameApp abstract
{
public:
	virtual void Startup(void) = 0;
	virtual void Cleanup(void) = 0;

	virtual bool IsDone(void);

	virtual void Update(const float deltaT) = 0;

	virtual void RenderScene(void) = 0;
};





