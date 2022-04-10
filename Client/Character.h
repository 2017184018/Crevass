#pragma once
#include "GameObject.h"

class Camera;
class CharacterParts;
class Character : public GameObject
{
	friend class CharacterParts;

public:
	explicit Character(RenderLayer layer, std::string type, std::string id);
	virtual ~Character();

	void Update(const float deltaT);

	void SetCamera(Camera* myCamera, CameraType cameraType);
	void SetCamera(CameraType cameraType);
	void SetController();
	void SetParts(CharacterParts* parts);

	virtual void SetPosition(float posX, float posY, float posZ);

	virtual void MoveStrafe(float fDistance = 1.0f) override;
	virtual void MoveUp(float fDistance = 1.0f) override;
	virtual void MoveForward(float fDistance = 1.0f) override;

	virtual void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);

	virtual void Rotate(const DirectX::XMFLOAT3& axis, float angle);
	virtual void Rotate(const DirectX::XMFLOAT4& quaternion);
	virtual void Rotate(float pitch, float yaw, float roll);

	virtual void Scale(float x, float y, float z) override;

public:
	Camera* m_MyCamera;
	std::map<std::string, CharacterParts*> m_Parts;

private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;

	float m_Pitch;
	float m_Yaw;
	float m_Roll;
};