#include "pch.h"
#include "GameObject.h"

static unsigned int s_currentIndex = 0;

GameObject::GameObject(RenderLayer layer, std::string type, std::string id):
	m_Layer(layer),
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

