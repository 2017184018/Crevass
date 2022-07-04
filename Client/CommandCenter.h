#pragma once
#include "ICommand.h"
#include "Singleton.h"
#include <list>

class Character;
class CommandCenter : public TemplateSingleton<CommandCenter>
{
public:
	void Release();

public:
	template <class TCommand>
	void PushCommand(int cEvent, Character* owner)
	{
		// Command 생성
		ICommand* iCmd = new TCommand(owner);
		iCmd->SetState(cEvent);

		m_FSM.push_back(iCmd);
	}
	void PopCommand(int cEvent)
	{
		// Command 삭제
		if (m_FSM.size() < 2) return;

		m_FSM.remove_if([&](ICommand* cmd) {
			if (cmd->GetState() == cEvent)
				return true;
			return false;
			});
	}

	void Order(float deltaT)
	{
//		cout << m_FSM.size() << endl;
		if (m_FSM.empty()) return;

		if (m_StartAttackAnim)
		{
			m_AttackDeltaT += 60.f / (1.f / deltaT);

			if (m_AttackAnimTime < m_AttackDeltaT)
			{
				m_AttackDeltaT = 0;
				m_StartAttackAnim = false;
				m_AttackCoolTimeSwitch = true;
				PopCommand(static_cast<int>(MoveState::Attack));
			}
		}

		else if (m_StartJumpAnim)
		{
			m_JumpDeltaT += 60.f / (1.f / deltaT);

			if (m_JumpAnimTime < m_JumpDeltaT)
			{
				m_JumpDeltaT = 0;
				m_StartJumpAnim = false;
				PopCommand(static_cast<int>(MoveState::Jump));
			}
		}

		else if (m_StartFallAnim)
		{
			m_FallDeltaT += 60.f / (1.f / deltaT);

			if (m_FallAnimTime < m_FallDeltaT)
			{
				m_FallDeltaT = 0;
				m_StartFallAnim = false;
				PopCommand(static_cast<int>(MoveState::Fall));
			}
		}

		else if (m_StartSkillAnim)
		{
			m_SkillDeltaT += 60.f / (1.f / deltaT);

			if (m_SkillAnimTime < m_SkillDeltaT)
			{
				if (!IsLoop) {
					m_SkillDeltaT = 0;
					m_StartSkillAnim = false;
					PopCommand(static_cast<int>(MoveState::Skill));
					IsSkillEnd = true;
				}
			}
		}

		m_FSM.back()->Execute();
	}

private:
	// 중간 삽입 삭제가 많이 일어나 list를 사용함
	// 하지만 순회를 해야하기 때문에 vector도 고민됨.
	std::list<ICommand*> m_FSM;

public:
	bool  m_StartAttackAnim = false;
	bool  m_AttackCoolTimeSwitch = false;
	float m_AttackAnimTime = 35.f;
	float m_AttackDeltaT = 0;

	bool	m_StartJumpAnim = false;
	float	m_JumpAnimTime = 45.f;
	float	m_JumpDeltaT = 0;

	bool	m_StartFallAnim = false;
	float	m_FallAnimTime = 76.f;
	float	m_FallDeltaT = 0;

	bool	m_StartSkillAnim = false;
	float	m_SkillAnimTime = 80.f;
	float	m_SkillDeltaT = 0;

	bool IsSkillEnd = false;
	bool IsLoop = false;
};

