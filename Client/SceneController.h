#pragma once
#include "Controller.h"

class LobbyScene;
class GameplayScene;
class GameresultScene;

class LobbyController final : public Controller
{
public:
	explicit LobbyController(LobbyScene* myScene);
	virtual ~LobbyController() = default;

public:
	virtual void Update(const float deltaT) override;

private:
	virtual void HandleInput(const float deltaT) override;
	virtual void MouseCallback() override;

private:
	LobbyScene* m_MyScene;
public:
	BOOL is_ready = false;
};


class GameplayController final : public Controller
{
public:
	explicit GameplayController(GameplayScene* myScene);
	virtual ~GameplayController() = default;

public:
	virtual void Update(const float deltaT) override;

private:
	virtual void HandleInput(const float deltaT) override;
	virtual void MouseCallback() override {};

private:
	GameplayScene* m_MyScene;
};

class GameresultController final : public Controller
{
public:
	explicit GameresultController(GameresultScene* myScene);
	virtual ~GameresultController() = default;

public:
	virtual void Update(const float deltaT) override;

private:
	virtual void HandleInput(const float deltaT) override;
	virtual void MouseCallback() override;

private:
	GameresultScene* m_MyScene;
};