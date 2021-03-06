#include "pch.h"
#include "MoveCommand.h"
#include "Character.h"

MoveCommand::MoveCommand(Character* owner) :
	ICommand(owner)
{
}

void MoveCommand::Idle()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
}

void MoveCommand::Forward()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_FORWARD);
}

void MoveCommand::Backward()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_FORWARD);
}

void MoveCommand::LeftStrafe()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_FORWARD);
}

void MoveCommand::RightStrafe()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_FORWARD);
}

void MoveCommand::Attack()
{
	// Animation
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_ATTACK);
	// 공격을 한순간부터 쿨타임 시간시작
}

void MoveCommand::Jump()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_JUMP);
}

void MoveCommand::Fall()
{
	m_Owner->SetAnimationKeyState(Character::PlayerState::STATE_FALL);
}
