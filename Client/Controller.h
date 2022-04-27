#pragma once

class Controller abstract
{
	// friend Network or Service
public:
	virtual void Update(const float deltaT) = 0;

protected:
	virtual void HandleInput(const float deltaT) = 0;
	virtual void MouseCallback() = 0;

	virtual void OnKeyPressed() {};
	virtual void OnKeyReleased() {};

};