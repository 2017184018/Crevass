#pragma once
#include "GameObject.h"
#include "PlayerController.h"

class Camera;
class CharacterParts;

class Character : public GameObject
{
	friend class PlayerController;
	friend class CharacterParts;

public:
	enum PlayerState
	{
		STATE_IDLE,
		STATE_IDLE_TO_FORWARD,
		STATE_IDLE_TO_JUMP,
		STATE_IDLE_TO_ATTACK,
		STATE_IDLE_TO_FALL,
		STATE_IDLE_TO_SKILL,

		STATE_FORWARD,
		STATE_FORWARD_TO_IDLE,
		STATE_FORWARD_TO_JUMP,
		STATE_FORWARD_TO_ATTACK,
		STATE_FORWARD_TO_FALL,
		STATE_FORWARD_TO_SKILL,

		STATE_JUMP,
		STATE_JUMP_TO_IDLE,
		STATE_JUMP_TO_FORWARD,
		STATE_JUMP_TO_ATTACK,
		STATE_JUMP_TO_FALL,
		STATE_JUMP_TO_SKILL,

		STATE_ATTACK,
		STATE_ATTACK_TO_JUMP,
		STATE_ATTACK_TO_IDLE,
		STATE_ATTACK_TO_FORWARD,
		STATE_ATTACK_TO_FALL,
		STATE_ATTACK_TO_SKILL,

		STATE_FALL,
		STATE_FALL_TO_IDLE,
		STATE_FALL_TO_FORWARD,
		STATE_FALL_TO_JUMP,
		STATE_FALL_TO_ATTACK,
		STATE_FALL_TO_SKILL,

		STATE_SKILL,
		STATE_SKILL_TO_IDLE,
		STATE_SKILL_TO_FORWARD,
		STATE_SKILL_TO_JUMP,
		STATE_SKILL_TO_ATTACK,
		STATE_SKILL_TO_FALL,
	};

	PlayerState m_PlayerState;
	PlayerState m_KeyState;
	void SetState(const float deltaT);
	void UpdateBoneTransforms();

public:
	class AnimData
	{
	public:
		explicit AnimData(std::string name, float time) : m_Name(name), m_Time(time) {};
		std::string m_Name;
		float m_Time;
	};

	std::map<std::string, std::unique_ptr<AnimData>> m_MapAnimData;

	float m_BlendFrame;
	float m_MaxBlendFrames;

public:
	explicit Character(std::string type, std::string id);
	virtual ~Character();

	void Update(const float deltaT);

	void SetCamera(Camera* myCamera, CameraType cameraType);
	void SetCamera(CameraType cameraType);
	void SetController();
	void SetParts(CharacterParts* parts);
	void SetDir(float angle);

	bool ReleaseTransform();

	virtual void SetPosition(float posX, float posY, float posZ);
	virtual void SetPosition(DirectX::XMFLOAT3 xmPos);

	virtual void SetRight(const DirectX::XMFLOAT3& Right);
	virtual void SetUp(const DirectX::XMFLOAT3& Up);
	virtual void SetLook(const DirectX::XMFLOAT3& Look);

	virtual void MoveStrafe(float fDistance = 1.0f) override;
	virtual void MoveUp(float fDistance = 1.0f) override;
	virtual void MoveForward(float fDistance = 1.0f) override;

	virtual void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);

	virtual void Rotate(const DirectX::XMFLOAT3& axis, float angle);
	virtual void Rotate(const DirectX::XMFLOAT4& quaternion);
	virtual void Rotate(float pitch, float yaw, float roll);



	virtual void Scale(float x, float y, float z) override;
	void pushed_back(float up, float back, float speed, float hit_angle);
	void SetAnimationKeyState(PlayerState keyState);

	void SetHide(bool hide);
	bool GetHide()const;

	void SetSnowmanNum(int num);
	int GetSnowmanNum()const;
public:
	Camera* m_MyCamera;
	std::unique_ptr<PlayerController> m_PlayerController;
	std::map<std::string, CharacterParts*> m_Parts;
	BOOL bJump = false;
	BOOL is_Inair = false;
	BOOL is_fall = false;
	BOOL is_StartFallAnim = false;
	float m_CurrentAngle = 0.0f;

	BoundingBox m_HitBox;
private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Right;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Look;

	float m_Pitch;
	float m_Yaw;
	float m_Roll;

	bool IsHide;
	int SnowmanNum;
	bool IsSkill = false;
};