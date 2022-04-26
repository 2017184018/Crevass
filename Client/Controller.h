#pragma once

class Controller abstract
{
	// friend Network or Service
public:
	virtual void Update() = 0;

protected:
	virtual void HandleInput() = 0;

	virtual void OnKeyPressed() {};
	virtual void OnKeyReleased() {};
};

class PlayerController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};

// 각 씬별로 Controller 소유
class TitleController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};

class LobbyController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};

class GameroomController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};

class GameplayController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};

class GameresultController final : public Controller
{
public:
	virtual void Update() override;

private:
	virtual void HandleInput() override;
};