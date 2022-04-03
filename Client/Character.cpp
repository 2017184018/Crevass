#include "pch.h"
#include "Character.h"
#include "CharacterParts.h"
Character::Character(RenderLayer layer, std::string type, std::string id) :
	GameObject(layer, type, id)
{
}

Character::~Character()
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
