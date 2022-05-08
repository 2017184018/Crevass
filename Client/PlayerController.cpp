#include "pch.h"
#include "PlayerController.h"
#include "InputHandler.h"
#include "Camera.h"

//#include "ApplicationContext.h"
//#include "GameObject.h"
#include "Character.h"
#include "SkinnedModelInstance.h"
#include "CommandCenter.h"
#include "MoveCommand.h"

#include "MainFramework.h"
#include "Network.h"
#include "CREVASS.h"

using namespace Core;

PlayerController::PlayerController(Character* player) :
	m_Owner(player)
{
	CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Idle), m_Owner);
}

void PlayerController::Update(const float deltaT)
{
	if (!Inactive) {
		MouseCallback();
		HandleInput(deltaT);

		OnKeyPressed();
		OnKeyReleased();
	}
	else {
		if (tmp == 0)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Forward));
		}
		if (tmp == 1)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Forward));
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::RightStrafe));
		}
		if (tmp == 2)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::RightStrafe));
		}
		if (tmp == 3)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Backward));
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::RightStrafe));
		}
		if (tmp == 4)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Backward));
		}
		if (tmp == 5)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Backward));
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::LeftStrafe));
		}
		if (tmp == 6)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::LeftStrafe));
		}
		if (tmp == 7)
		{
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Forward));
			//CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::LeftStrafe));
		}
		if (tmp == 8) {}
		tmp = -1;
	}
}

static bool SpacePush = false;
static int key_switch_up = 0;
static int key_switch_down = 0;
static int key_switch_left = 0;
static int key_switch_right = 0;
void PlayerController::HandleInput(const float deltaT)
{

	//���ݵ��� �������̰�
	if (CommandCenter::GetApp()->m_StartAttackAnim || CommandCenter::GetApp()->m_StartFallAnim || m_Owner->is_StartFallAnim) return;

	float speed = 300 * deltaT;
	XMVECTOR direction = {};

	DWORD dir = 0;

	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:
		//����

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_DOWN);
			key_switch_up = 0;

			tmp = 0;
		}
		else if(key_switch_up ==0){
			key_switch_up = 1;
			g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_UP);
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			g_pFramework->m_pNetwork->Send(CS_PLAYER_LEFT_DOWN);
			key_switch_left = 0;
			tmp = 6;
		}
				else  if (key_switch_left == 0) {
			key_switch_left = 1;
			g_pFramework->m_pNetwork->Send(CS_PLAYER_LEFT_UP);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			g_pFramework->m_pNetwork->Send(CS_PLAYER_DOWN_DOWN);
			key_switch_down = 0;
			tmp = 4;
		}
		else  if (key_switch_down == 0) {
			key_switch_down = 1;
			g_pFramework->m_pNetwork->Send(CS_PLAYER_DOWN_UP);
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			g_pFramework->m_pNetwork->Send(CS_PLAYER_RIGHT_DOWN);
			key_switch_right = 0;
			tmp = 2;
		}
		else if (key_switch_right == 0){
			key_switch_right = 1;
			g_pFramework->m_pNetwork->Send(CS_PLAYER_RIGHT_UP);
		}

		//
		//		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		//			/*if (m_Owner->m_KeyState != Character::PlayerState::STATE_ATTACK) {
		//				if (m_Owner->bJump == false) {
		//					if (!SpacePush) {
		//						m_Owner->bJump = true;
		//						m_Owner->is_Inair = true;
		//						m_Owner->m_KeyState = Character::PlayerState::STATE_JUMP;
		//						SpacePush = true;
		//					}
		//				}
		//				
		//
		//			}*/
		//			//CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		//#ifdef DEBUG_CLIENT
		//			m_Owner->Move(DIR_FORWARD, speed, true);
		//#elif DEBUG_SERVER
		//#endif
		//			m_Owner->SetDir(0);
		//			g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_DOWN);
		//			tmp = 0;
		//		}
		//
		//
		//		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		//			/*	if (m_Owner->m_KeyState != Character::PlayerState::STATE_ATTACK) {
		//					m_Owner->Move(DIR_LEFT, speed, true);
		//					if (!m_Owner->bJump)
		//						m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
		//					m_Owner->SetDir(270);
		//				}*/
		//				//CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		//			
		//#ifdef DEBUG_CLIENT
		//			m_Owner->Move(DIR_LEFT, speed, true);
		//#elif DEBUG_SERVER
		//#endif
		//			m_Owner->SetDir(270);
		//			g_pFramework->m_pNetwork->Send(CS_PLAYER_LEFT_DOWN);
		//			tmp = 6;
		//		}
		//
		//		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		//			/*if (m_Owner->m_KeyState != Character::PlayerState::STATE_ATTACK) {
		//				m_Owner->Move(DIR_BACKWARD, speed, true);
		//				if (!m_Owner->bJump)
		//					m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
		//				m_Owner->SetDir(180);
		//			}*/
		//			//CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		//			
		//#ifdef DEBUG_CLIENT
		//			m_Owner->Move(DIR_BACKWARD, speed, true);
		//#elif DEBUG_SERVER
		//#endif
		//			m_Owner->SetDir(180);
		//			g_pFramework->m_pNetwork->Send(CS_PLAYER_DOWN_DOWN);
		//			tmp = 4;
		//		}
		//
		//		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		//			/*if (m_Owner->m_KeyState != Character::PlayerState::STATE_ATTACK) {
		//				m_Owner->Move(DIR_RIGHT, speed, true);
		//				if (!m_Owner->bJump)
		//					m_Owner->m_KeyState = Character::PlayerState::STATE_FORWARD;
		//				m_Owner->SetDir(90);
		//			}*/
		//			//CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		//		
		//#ifdef DEBUG_CLIENT
		//			m_Owner->Move(DIR_RIGHT, speed, true);
		//#elif DEBUG_SERVER
		//#endif
		//
		//			m_Owner->SetDir(90);
		//			g_pFramework->m_pNetwork->Send(CS_PLAYER_RIGHT_DOWN);
		//			tmp = 2;
		//		}

		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 )&& (GetAsyncKeyState(VK_UP) & 0x8000)) {
			tmp = 1;
		}

		if ((GetAsyncKeyState(VK_DOWN) & 0x8000 )&& (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
			tmp = 3;
		}

		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && (GetAsyncKeyState(VK_LEFT) & 0x8000)) {
			tmp = 5;
		}

		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8000)) {
			tmp = 7;
		}

		break;
	case CameraType::Free:
	{
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			m_Owner->m_MyCamera->Walk(speed);
			tmp = 0;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			m_Owner->m_MyCamera->Walk(-speed);
			tmp = 4;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			m_Owner->m_MyCamera->Strafe(-speed);
			tmp = 6;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			m_Owner->m_MyCamera->Strafe(speed);
			tmp = 2;
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
	if (!m_Owner) return;
	if (!m_Owner->m_MyCamera) return;
	if (CommandCenter::GetApp()->m_StartJumpAnim || CommandCenter::GetApp()->m_StartAttackAnim || CommandCenter::GetApp()->m_StartFallAnim|| m_Owner->is_StartFallAnim) return;

	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:

		if (InputHandler::IsKeyDown('A')) {
			//HIT박스 위치 업데이트 
			if (m_Owner->m_CurrentAngle == 0) {
				
				m_Owner->m_HitBox.Center = m_Owner->GetPosition();
				m_Owner->m_HitBox.Center.z += m_Owner->m_HitBox.Extents.z;
				//AppContext->FindObject<GameObject>("huskyBB", "huskyBB0")->SetPosition(m_Owner->m_HitBox.Center);

			}
			else if(m_Owner->m_CurrentAngle == 90)
			{
				m_Owner->m_HitBox.Center = m_Owner->GetPosition();
				m_Owner->m_HitBox.Center.x += m_Owner->m_HitBox.Extents.z;
				//AppContext->FindObject<GameObject>("huskyBB", "huskyBB0")->SetPosition(m_Owner->m_HitBox.Center);
			}
			else if (m_Owner->m_CurrentAngle == 180)
			{
				m_Owner->m_HitBox.Center = m_Owner->GetPosition();
				m_Owner->m_HitBox.Center.z -= m_Owner->m_HitBox.Extents.z;
				//AppContext->FindObject<GameObject>("huskyBB", "huskyBB0")->SetPosition(m_Owner->m_HitBox.Center);
			}
			else if (m_Owner->m_CurrentAngle == 270)
			{
				m_Owner->m_HitBox.Center = m_Owner->GetPosition();
				m_Owner->m_HitBox.Center.x -= m_Owner->m_HitBox.Extents.z;
				//AppContext->FindObject<GameObject>("huskyBB", "huskyBB0")->SetPosition(m_Owner->m_HitBox.Center);
			}
			CommandCenter::GetApp()->m_StartAttackAnim = true;
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Attack), m_Owner);
			g_pFramework->m_pNetwork->Send(CS_PLAYER_ATTACK);
			
		}

		if (InputHandler::IsKeyDown('S')) {

			m_Owner->is_fall = !m_Owner->is_fall;
			CommandCenter::GetApp()->m_StartFallAnim = true;
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Fall), m_Owner);
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_MOVE);
		}

		if (InputHandler::IsKeyDown(VK_UP)) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
			//g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_DOWN);
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_MOVE);
			tmp = 0;
		}

		if (InputHandler::IsKeyDown(VK_LEFT)) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::LeftStrafe), m_Owner);
			//g_pFramework->m_pNetwork->Send(CS_PLAYER_MOVE);
			tmp = 6;
		}

		if (InputHandler::IsKeyDown(VK_DOWN)) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Backward), m_Owner);
			//g_pFramework->m_pNetwork->Send(CS_PLAYER_MOVE);
			tmp = 4;
		}

		if (InputHandler::IsKeyDown(VK_RIGHT)) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::RightStrafe), m_Owner);
			//g_pFramework->m_pNetwork->Send(CS_PLAYER_MOVE);
			tmp = 2;
		}

		if (InputHandler::IsKeyDown(VK_SPACE)) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Jump), m_Owner);
			CommandCenter::GetApp()->m_StartJumpAnim = true;
			g_pFramework->m_pNetwork->Send(CS_PLAYER_JUMP);

			m_Owner->is_Inair = true;
			m_Owner->bJump = true;
			tmp = 8;
		}
		else {
			//SpacePush = false;
		}

		/*if (GetAsyncKeyState(VK_UP) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && GetAsyncKeyState(VK_LEFT) & 0x8000) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_UP) & 0x8000) {
			CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Forward), m_Owner);
		}*/

		break;

	}
}

void PlayerController::OnKeyReleased()
{
	if (!m_Owner) return;
	if (!m_Owner->m_MyCamera) return;
	if (CommandCenter::GetApp()->m_StartJumpAnim || CommandCenter::GetApp()->m_StartAttackAnim || CommandCenter::GetApp()->m_StartFallAnim || m_Owner->is_StartFallAnim) return;

	switch (m_Owner->m_MyCamera->GetCameraType())
	{
	case CameraType::First:
	case CameraType::Third:
		if (InputHandler::IsKeyUp(VK_UP))
		{
			CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Forward));
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_UP);

		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_IDLE);

			tmp = -1;
		}
		if (InputHandler::IsKeyUp(VK_LEFT))
		{
			CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::LeftStrafe));
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_LEFT_UP);

		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_IDLE);

			tmp = -1;
		}
		if (InputHandler::IsKeyUp(VK_DOWN))
		{
			CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Backward));
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_DOWN_UP);

		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_IDLE);

			tmp = -1;
		}
		if (InputHandler::IsKeyUp(VK_RIGHT))
		{
			CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::RightStrafe));
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_RIGHT_UP);
			//g_pFramework->m_pNetwork->Send(CS_PLAYER_IDLE);
			tmp = -1;
		}
		if (InputHandler::IsKeyUp(VK_SPACE)) {
			tmp = -1;
		}
		if (InputHandler::IsKeyUp('A')) {
			CommandCenter::GetApp()->PopCommand(static_cast<int>(MoveState::Attack));
		}
		if (InputHandler::IsKeyUp('S')) {
		}
		break;
	}
}

void PlayerController::Fall() {
	m_Owner->is_fall = true;
	CommandCenter::GetApp()->m_StartFallAnim = true;
	CommandCenter::GetApp()->PushCommand<MoveCommand>(static_cast<int>(MoveState::Fall), m_Owner);
}