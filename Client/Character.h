#pragma once
#include "GameObject.h"

class CharacterParts;
class Character : public GameObject
{
	friend class CharacterParts;

public:
	explicit Character(RenderLayer layer, std::string type, std::string id);
	virtual ~Character();

	void SetParts(CharacterParts* parts);

	virtual void SetPosition(float posX, float posY, float posZ);

	virtual void Rotate(const DirectX::XMFLOAT3& axis, float angle);
	virtual void Rotate(const DirectX::XMFLOAT4& quaternion);
	virtual void Rotate(float pitch, float yaw, float roll);

	virtual void Scale(float x, float y, float z) override;

public:
	std::map<std::string, CharacterParts*> m_Parts;
};