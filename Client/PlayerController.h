#pragma once
#include "Controller.h"

// Command 명령패턴 추가
class Character;
class PlayerController final : public Controller
{
public:
	explicit PlayerController(Character* player);
	virtual ~PlayerController() = default;

public:
	virtual void Update(const float deltaT) override;
	void Fall();
	void SetIsFall();

private:
	virtual void HandleInput(const float deltaT) override;
	virtual void MouseCallback() override;

	virtual void OnKeyPressed() override;
	virtual void OnKeyReleased() override;

private:
	Character* m_Owner;
	int tmp = -1;		//화면 내려갈 때 키 눌림 해제
	
};