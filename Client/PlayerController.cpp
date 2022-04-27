#include "pch.h"
#include "PlayerController.h"
#include "InputHandler.h"
#include "Camera.h"

#include "Character.h"
#include "SkinnedModelInstance.h"

PlayerController::PlayerController(Character* player) :
	m_Owner(player)
{
}

void PlayerController::Update(const float deltaT)
{
	MouseCallback();
	HandleInput(deltaT);

	OnKeyPressed();
	OnKeyReleased();
}

void PlayerController::HandleInput(const float deltaT)
{
	float speed = 100 * deltaT;
	XMVECTOR direction = {};

	DWORD dir = 0;

	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:
		//╬Нец
		//if (GetAsyncKeyState('G') & 0x8000) {
		//	//m_Owner->m_SkinnedModelInsts["husky"]->ClipName = "Peck";
		//}


		if (GetAsyncKeyState('W') & 0x8000) {
			m_Owner->Move(DIR_FORWARD, speed, true);
			if (!m_Owner->bJump) {
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			}
			m_Owner->SetDir(0);
		}


		if (GetAsyncKeyState('A') & 0x8000) {
			m_Owner->Move(DIR_LEFT, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(270);
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			m_Owner->Move(DIR_BACKWARD, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(180);
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			m_Owner->Move(DIR_RIGHT, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(90);
		}

		if (GetAsyncKeyState('F') & 0x8000 && m_Owner->bJump == false) {
			m_Owner->bJump = true;
			//B = true;
			m_Owner->m_KeyState = Character::PlayerState::STATE_JUMP;

		}

		if (GetAsyncKeyState('W') & 0x8000 && GetAsyncKeyState('D') & 0x8000) {
			if (!m_Owner->bJump) {

				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			}
			m_Owner->SetDir(45);
		}

		if (GetAsyncKeyState('S') & 0x8000 && GetAsyncKeyState('D') & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;

			m_Owner->SetDir(135);
		}

		if (GetAsyncKeyState('S') & 0x8000 && GetAsyncKeyState('A') & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(225);
		}

		if (GetAsyncKeyState('A') & 0x8000 && GetAsyncKeyState('W') & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;

			m_Owner->SetDir(315);
		}

		if (InputHandler::IsKeyUp('W'))
		{
			if (!m_Owner->bJump) {

				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
			}
		}
		if (InputHandler::IsKeyUp('A'))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
		if (InputHandler::IsKeyUp('S'))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
		if (InputHandler::IsKeyUp('D'))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
	break;
	case CameraType::Free:
	{
		if (GetAsyncKeyState('W') & 0x8000) {
			m_Owner->m_MyCamera->Walk(speed);
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			m_Owner->m_MyCamera->Walk(-speed);
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			m_Owner->m_MyCamera->Strafe(-speed);
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			m_Owner->m_MyCamera->Strafe(speed);
		}
		break;
}
	default:
		break;
	}


	if (GetAsyncKeyState(VK_F1) & 0x8000) {
		m_Owner->SetCamera(CameraType::First);
	}
	if (GetAsyncKeyState(VK_F2) & 0x8000) {
		m_Owner->SetCamera(CameraType::Third);
	}
	if (GetAsyncKeyState(VK_F3) & 0x8000) {
		m_Owner->SetCamera(CameraType::Free);
	}

}

void PlayerController::MouseCallback()
{
	
}

void PlayerController::OnKeyPressed()
{
	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:

		break;
	}
}

void PlayerController::OnKeyReleased()
{
	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:
		break;
	}
}
