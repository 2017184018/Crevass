#include "pch.h"
#include "GameObject.h"
#include "MeshReference.h"
#include "ApplicationContext.h"
#include "Particle.h"
static unsigned int s_currentIndex = 0;

GameObject::GameObject(std::string type, std::string id) :
	m_Type(type),
	m_ID(id),
	m_Index(s_currentIndex++),
	m_SkinnedModelInst(nullptr)
{
	m_World = MathHelper::Identity4x4();
	m_TexTransform = MathHelper::Identity4x4();

}

GameObject::~GameObject()
{
}

void GameObject::Rotate(const DirectX::XMFLOAT3& axis, float angle)
{
	XMMATRIX RotMat = XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(angle));
	XMMATRIX World = RotMat * XMLoadFloat4x4(&m_World);
	XMStoreFloat4x4(&m_World, World);

}

void GameObject::Rotate(const DirectX::XMFLOAT4& quaternion)
{
	XMMATRIX RotMat = XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion));
	XMMATRIX World = RotMat * XMLoadFloat4x4(&m_World);
	XMStoreFloat4x4(&m_World, World);
}

void GameObject::Rotate(float pitch, float yaw, float roll)
{
	XMMATRIX R = XMMatrixRotationRollPitchYaw(
		MathHelper::XMConvertToRadians(pitch),
		MathHelper::XMConvertToRadians(yaw),
		MathHelper::XMConvertToRadians(roll));
	XMStoreFloat4x4(&m_World, R * XMLoadFloat4x4(&m_World));
}

void GameObject::SetParticle(std::string particleName, std::string instID)
{
	Particle* particle = AppContext->FindObject<Particle>(particleName, instID);
	if (!particle) return;

	m_Particles[particleName] = particle;
}

void GameObject::BlockParticle()
{
	XMFLOAT3 offSet = { 0,50,0 };
	AppContext->DisplayParticle("crushparticle", "crushparticle" + std::to_string(11), MathHelper::Add(GetPosition(), offSet));
}

void GameObject::InitializeTransform()
{
	m_World = MathHelper::Identity4x4();
	m_TexTransform = MathHelper::Identity4x4();
}

void GameObject::Update(const float deltaT)
{
	//for (auto& p : m_Particles)
	//{
	//p.second->Update(deltaT);
	//}
}

void GameObject::UpdateParticleTime(const float deltaT)
{
	for (auto& p : m_Particles)
	{
		p.second->Update(deltaT);
	}
}

void GameObject::Scale(float x, float y, float z)
{
	m_World._11 *= x;
	m_World._22 *= y;
	m_World._33 *= z;
}

void GameObject::SetPosition(float posX, float posY, float posZ)
{
	m_World._41 = posX;
	m_World._42 = posY;
	m_World._43 = posZ;

}

void GameObject::SetPosition(DirectX::XMFLOAT3 xmPos)
{
	m_World._41 = xmPos.x;
	m_World._42 = xmPos.y;
	m_World._43 = xmPos.z;
}

void GameObject::SetRight(const DirectX::XMFLOAT3& Right)
{
	m_World._11 = Right.x;
	m_World._12 = Right.y;
	m_World._13 = Right.z;
}

void GameObject::SetUp(const DirectX::XMFLOAT3& Up)
{
	m_World._21 = Up.x;
	m_World._22 = Up.y;
	m_World._23 = Up.z;
}

void GameObject::SetLook(const DirectX::XMFLOAT3& Look)
{
	m_World._31 = Look.x;
	m_World._32 = Look.y;
	m_World._33 = Look.z;
}

XMFLOAT3 GameObject::GetPosition() const
{
	return XMFLOAT3(m_World._41, m_World._42, m_World._43);
}

XMFLOAT3 GameObject::GetLook() const
{
	XMFLOAT3 look = { m_World._31, m_World._32, m_World._33 };
	return MathHelper::Normalize(look);
}

XMFLOAT3 GameObject::GetUp() const
{
	XMFLOAT3 up = { m_World._21, m_World._22, m_World._23 };
	return MathHelper::Normalize(up);
}

XMFLOAT3 GameObject::GetRight() const
{
	XMFLOAT3 right = { m_World._11, m_World._12, m_World._13 };
	return MathHelper::Normalize(right);
}

void GameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = MathHelper::Add(xmf3Position, xmf3Right, fDistance);
	SetPosition(xmf3Position);
}

void GameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = MathHelper::Add(xmf3Position, xmf3Up, fDistance);
	SetPosition(xmf3Position);
}

void GameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = MathHelper::Add(xmf3Position, xmf3Look, fDistance);
	SetPosition(xmf3Position);
}

void GameObject::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	/*XMFLOAT3 look = GetLook();
	XMFLOAT3 right = GetRight();
	XMFLOAT3 up = GetUp();*/

	XMFLOAT3 look = { 0,0,1 };
	XMFLOAT3 right = { 1,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) xmf3Shift = MathHelper::Add(xmf3Shift, look, fDistance);
		if (dwDirection & DIR_BACKWARD) xmf3Shift = MathHelper::Add(xmf3Shift, look, -fDistance);
		if (dwDirection & DIR_RIGHT) xmf3Shift = MathHelper::Add(xmf3Shift, right, fDistance);
		if (dwDirection & DIR_LEFT) xmf3Shift = MathHelper::Add(xmf3Shift, right, -fDistance);
		if (dwDirection & DIR_UP) xmf3Shift = MathHelper::Add(xmf3Shift, up, fDistance);
		if (dwDirection & DIR_DOWN) xmf3Shift = MathHelper::Add(xmf3Shift, up, -fDistance);

		Move(xmf3Shift, bUpdateVelocity);
	}
}

void GameObject::Move(const XMFLOAT3& xmf3Shift, bool bVelocity)
{
	XMFLOAT3 pos = MathHelper::Add(GetPosition(), xmf3Shift);
	SetPosition(pos);
}

