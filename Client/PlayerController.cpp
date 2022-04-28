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

static bool SpacePush = false;

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


		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			m_Owner->Move(DIR_FORWARD, speed, true);
			if (!m_Owner->bJump) {
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			}
			m_Owner->SetDir(0);
		}


		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			m_Owner->Move(DIR_LEFT, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(270);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			m_Owner->Move(DIR_BACKWARD, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(180);
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			m_Owner->Move(DIR_RIGHT, speed, true);
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(90);
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			if (m_Owner->bJump == false) {
				if (!SpacePush) {
					m_Owner->bJump = true;
					m_Owner->is_Inair = true;
					m_Owner->m_KeyState = Character::PlayerState::STATE_JUMP;
					SpacePush = true;
				}
			}
		}
		else {
			SpacePush = false;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (!m_Owner->bJump) {

				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			}
			m_Owner->SetDir(45);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;

			m_Owner->SetDir(135);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
			m_Owner->SetDir(225);
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_UP) & 0x8000) {
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;

			m_Owner->SetDir(315);
		}

		if (InputHandler::IsKeyUp(VK_UP))
		{
			if (!m_Owner->bJump) {

				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
			}
		}
		if (InputHandler::IsKeyUp(VK_LEFT))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
		if (InputHandler::IsKeyUp(VK_DOWN))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
		if (InputHandler::IsKeyUp(VK_RIGHT))
		{
			if (!m_Owner->bJump)
				m_Owner->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
		break;
	case CameraType::Free:
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			m_Owner->m_MyCamera->Walk(speed);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			m_Owner->m_MyCamera->Walk(-speed);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			m_Owner->m_MyCamera->Strafe(-speed);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
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
