#include "pch.h"
#include "Character.h"
#include "Camera.h"
#include "CharacterParts.h"
Character::Character(RenderLayer layer, std::string type, std::string id) :
	GameObject(layer, type, id),
	m_MyCamera(nullptr)
{
	m_Position = { 0,0,0 };
	m_Right = { 1,0,0 };
	m_Up = { 0,1,0 };
	m_Look = { 0,0,1 };

	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
	m_Roll = 0.0f;
}

Character::~Character()
{
}

void Character::Update(const float deltaT)
{
	//m_PlayerController->Update(deltaT);

	CameraType cType = m_MyCamera->GetCameraType();

	if (cType == CameraType::Third)
	{
		XMFLOAT3 target = GetPosition();
		target.y += 200;
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
		//y랑z가 바뀜
		//m_MyCamera->SetOffset(XMFLOAT3(0.0f, 300.f, -400.f));
		m_MyCamera->SetOffset(XMFLOAT3(0.0f, 0.f, -400.f));
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

void Character::SetPosition(float posX, float posY, float posZ)
{
	GameObject::SetPosition(posX, posY, posZ);
	for (auto& p : m_Parts)
	{
		p.second->SetPosition(posX, posY, posZ);
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

