#include "pch.h"
#include "MeshReference.h"
#include "Character.h"
#include "Camera.h"
#include "CharacterParts.h"

void Character::SetState()
{
	switch (m_PlayerState)
	{
	case STATE_IDLE:
		switch (m_KeyState)
		{
		case STATE_FORWARD:
			m_PlayerState = STATE_IDLE_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.4f;
			m_BlendFrame = 1.f;
			break;

		case STATE_JUMP:
			m_PlayerState = STATE_IDLE_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.4f;
			m_BlendFrame = 1.0f;
			break;

		}
		break;

	case STATE_FORWARD:
		switch (m_KeyState)
		{
		case STATE_IDLE:
			m_PlayerState = STATE_FORWARD_TO_IDLE;
			m_MapAnimData["Idle"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		case STATE_JUMP:
			m_PlayerState = STATE_FORWARD_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.4f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;
	case STATE_JUMP:
		switch (m_KeyState)
		{
		case STATE_IDLE:
			m_PlayerState = STATE_JUMP_TO_IDLE;
			m_MapAnimData["Idle"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;
	case STATE_IDLE_TO_FORWARD:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_FORWARD;
		m_BlendFrame++;
		break;

	case STATE_FORWARD_TO_IDLE:
	case STATE_JUMP_TO_IDLE:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_IDLE;
		m_BlendFrame++;
		break;
	case STATE_FORWARD_TO_JUMP:
	case STATE_IDLE_TO_JUMP:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_JUMP;
		m_BlendFrame++;
		break;
	}
}
void Character::UpdateBoneTransforms()
{
	std::string strState = "Idle";

	switch (m_PlayerState)
	{
	case Character::STATE_IDLE:
		strState = "Idle";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time
		);
		break;
	case Character::STATE_FORWARD:
		strState = "Run";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time
		);
		break;
	case Character::STATE_JUMP:
		strState = "Jump";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time
		);
		break;
	case Character::STATE_IDLE_TO_FORWARD:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_IDLE_TO_JUMP:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FORWARD_TO_IDLE:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_JUMP_TO_IDLE:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FORWARD_TO_JUMP:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	}

	if (m_MapAnimData[strState]->m_Time > m_SkinnedModelInst->SkinnedInfo->GetClipEndTime(strState))
	{
		m_MapAnimData[strState]->m_Time = 0.f;
	}

}
Character::Character(RenderLayer layer, std::string type, std::string id) :
	GameObject(layer, type, id),
	m_PlayerState(PlayerState::STATE_IDLE),
	m_KeyState(PlayerState::STATE_IDLE),
	m_MyCamera(nullptr)
{
	m_Position = { 0,0,0 };
	m_Right = { 1,0,0 };
	m_Up = { 0,1,0 };
	m_Look = { 0,0,1 };

	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
	m_Roll = 0.0f;

	m_MapAnimData["Idle"] = std::make_unique<AnimData>("Idle", 0.f);
	m_MapAnimData["Run"] = std::make_unique<AnimData>("Run", 0.f);
	m_MapAnimData["Jump"] = std::make_unique<AnimData>("Jump", 0.f);

	m_MaxBlendFrames = 10.f;
}


Character::~Character()
{
}

void Character::Update(const float deltaT)
{
	//m_PlayerController->Update(deltaT);


	SetState();
	for (auto& p : m_MapAnimData)
	{
		p.second->m_Time += deltaT;
	}
	UpdateBoneTransforms();

	CameraType cType = m_MyCamera->GetCameraType();

	if (cType == CameraType::Third)
	{
		XMFLOAT3 target = GetPosition();
		target.y += 100;
		m_MyCamera->Update(target, deltaT);
		m_MyCamera->SetTarget(target);
	}
	
}

void Character::SetCamera(Camera* myCamera, CameraType cameraType)
{
	m_MyCamera = myCamera;

	SetCamera(cameraType);
}

void Character::SetCamera(CameraType cameraType)
{
	if (m_MyCamera == nullptr) return;
	m_MyCamera->SetCamera(cameraType, this);

	switch (cameraType)
	{
	case CameraType::First:
		// 현재 포지션벡터가 발끝에 있음 눈으로 거리이동해야함
		m_MyCamera->SetTimeLag(0.f);
		m_MyCamera->SetOffset(XMFLOAT3(0.0f, 180.f, 30.f));
		m_MyCamera->SetPosition(MathHelper::Add(GetPosition(), m_MyCamera->GetOffset()));
		break;
	case CameraType::Third:
		m_MyCamera->SetTimeLag(0.1f);
		m_MyCamera->SetOffset(XMFLOAT3(0.0f, 150.f, -300.f));
		m_MyCamera->SetPosition(MathHelper::Add(GetPosition(), m_MyCamera->GetOffset()));
		break;
	case CameraType::Free:
		break;
	}
}

void Character::SetController()
{
}

void Character::SetParts(CharacterParts* parts)
{
	m_Parts[parts->m_PartsName] = parts;
}

void Character::SetDir(float angle)
{
	int R = 360.f - m_CurrentAngle;

	GameObject::Rotate(0, R+ angle, 0);
	for (auto& p : m_Parts)
	{
		p.second->Rotate(0, R + angle, 0);
	}

	m_CurrentAngle = angle;
}




void Character::SetPosition(float posX, float posY, float posZ)
{
	GameObject::SetPosition(posX, posY, posZ);
	for (auto& p : m_Parts)
	{
		p.second->SetPosition(posX, posY, posZ);
	}
}

void Character::SetRight(const DirectX::XMFLOAT3& Right)
{
	GameObject::SetRight(Right);
	for (auto& p : m_Parts)
	{
		p.second->SetRight(Right);
	}
}

void Character::SetUp(const DirectX::XMFLOAT3& Up)
{
	GameObject::SetUp(Up);
	for (auto& p : m_Parts)
	{
		p.second->SetUp(Up);
	}
}

void Character::SetLook(const DirectX::XMFLOAT3& Look)
{
	GameObject::SetLook(Look);
	for (auto& p : m_Parts)
	{
		p.second->SetLook(Look);
	}
}

void Character::MoveStrafe(float distance)
{
	GameObject::MoveStrafe(distance);
	for (auto& p : m_Parts)
	{
		p.second->MoveStrafe(distance);
	}
}

void Character::MoveUp(float distance)
{
	GameObject::MoveUp(distance);
	for (auto& p : m_Parts)
	{
		p.second->MoveUp(distance);
	}
}

void Character::MoveForward(float distance)
{
	GameObject::MoveForward(distance);
	for (auto& p : m_Parts)
	{
		p.second->MoveForward(distance);
	}
}

void Character::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	GameObject::Move(dwDirection, fDistance, bUpdateVelocity);
	for (auto& p : m_Parts)
	{
		//p.second->Move(dwDirection, fDistance, bUpdateVelocity);
	}
}

void Character::Move(const XMFLOAT3& xmf3Shift, bool bVelocity)
{
	GameObject::Move(xmf3Shift, bVelocity);
	for (auto& p : m_Parts)
	{
		p.second->Move(xmf3Shift, bVelocity);
	}

	//m_MyCamera->Move(xmf3Shift);
}


void Character::Rotate(const DirectX::XMFLOAT3& axis, float angle)
{
	GameObject::Rotate(axis, angle);
	for (auto& p : m_Parts)
	{
		p.second->Rotate(axis, angle);
	}
}

void Character::Rotate(const DirectX::XMFLOAT4& quaternion)
{
	GameObject::Rotate(quaternion);
	for (auto& p : m_Parts)
	{
		p.second->Rotate(quaternion);
	}
}

void Character::Rotate(float pitch, float yaw, float roll)
{
	GameObject::Rotate(pitch, yaw, roll);
	for (auto& p : m_Parts)
	{
		p.second->Rotate(pitch, yaw, roll);
	}
}


void Character::Scale(float x, float y, float z)
{
	GameObject::Scale(x, y, z);
	for (auto& p : m_Parts)
	{
		p.second->Scale(x, y, z);
	}
}

