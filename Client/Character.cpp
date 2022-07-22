#include "pch.h"
#include "MeshReference.h"
#include "Character.h"
#include "Camera.h"
#include "CharacterParts.h"

void Character::SetState(const float deltaT)
{
	switch (m_PlayerState)
	{
	case STATE_IDLE:
		switch (m_KeyState)
		{
		case STATE_FORWARD:
			m_PlayerState = STATE_IDLE_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.0f;
			m_BlendFrame = 1.f;
			break;

		case STATE_JUMP:
			m_PlayerState = STATE_IDLE_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.2f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_ATTACK:
			m_PlayerState = STATE_IDLE_TO_ATTACK;
			m_MapAnimData["Attack"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_FALL:
			m_PlayerState = STATE_IDLE_TO_FALL;
			m_MapAnimData["Fall"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_SKILL:
			m_PlayerState = STATE_IDLE_TO_SKILL;
			m_MapAnimData["Skill"]->m_Time = 0.0f;
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
			m_MapAnimData["Jump"]->m_Time = 0.1f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_ATTACK:
			m_PlayerState = STATE_FORWARD_TO_ATTACK;
			m_MapAnimData["Attack"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_FALL:
			m_PlayerState = STATE_FORWARD_TO_FALL;
			m_MapAnimData["Fall"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_SKILL:
			m_PlayerState = STATE_FORWARD_TO_SKILL;
			m_MapAnimData["Skill"]->m_Time = 0.0f;
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
		case STATE_FORWARD:
			m_PlayerState = STATE_JUMP_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_ATTACK:
			m_PlayerState = STATE_JUMP_TO_ATTACK;
			m_MapAnimData["Attack"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_FALL:
			m_PlayerState = STATE_JUMP_TO_FALL;
			m_MapAnimData["Fall"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_SKILL:
			m_PlayerState = STATE_JUMP_TO_SKILL;
			m_MapAnimData["Skill"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;
	case STATE_ATTACK:
		switch (m_KeyState)
		{
		case STATE_IDLE:
			m_PlayerState = STATE_ATTACK_TO_IDLE;
			m_MapAnimData["Idle"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		case STATE_FORWARD:
			m_PlayerState = STATE_ATTACK_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_JUMP:
			m_PlayerState = STATE_ATTACK_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_FALL:
			m_PlayerState = STATE_ATTACK_TO_FALL;
			m_MapAnimData["Fall"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_SKILL:
			m_PlayerState = STATE_ATTACK_TO_SKILL;
			m_MapAnimData["Skill"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;

	case STATE_FALL:
		switch (m_KeyState)
		{
		case STATE_IDLE:
			m_PlayerState = STATE_FALL_TO_IDLE;
			m_MapAnimData["Idle"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		case STATE_FORWARD:
			m_PlayerState = STATE_FALL_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_JUMP:
			m_PlayerState = STATE_FALL_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_ATTACK:
			m_PlayerState = STATE_FALL_TO_ATTACK;
			m_MapAnimData["Attack"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_SKILL:
			m_PlayerState = STATE_FALL_TO_SKILL;
			m_MapAnimData["Skill"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;

	case STATE_SKILL:
		switch (m_KeyState)
		{
		case STATE_IDLE:
			m_PlayerState = STATE_SKILL_TO_IDLE;
			m_MapAnimData["Idle"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		case STATE_FORWARD:
			m_PlayerState = STATE_SKILL_TO_FORWARD;
			m_MapAnimData["Run"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_JUMP:
			m_PlayerState = STATE_SKILL_TO_JUMP;
			m_MapAnimData["Jump"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_ATTACK:
			m_PlayerState = STATE_SKILL_TO_ATTACK;
			m_MapAnimData["Attack"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;

		case STATE_FALL:
			m_PlayerState = STATE_SKILL_TO_FALL;
			m_MapAnimData["Fall"]->m_Time = 0.0f;
			m_BlendFrame = 1.0f;
			break;
		}
		break;

	case STATE_FALL_TO_FORWARD:
	case STATE_ATTACK_TO_FORWARD:
	case STATE_JUMP_TO_FORWARD:
	case STATE_IDLE_TO_FORWARD:
	case STATE_SKILL_TO_FORWARD:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_FORWARD;
		m_BlendFrame += 60.f / (1.f / deltaT);
		break;

	case STATE_FALL_TO_IDLE:
	case STATE_ATTACK_TO_IDLE:
	case STATE_FORWARD_TO_IDLE:
	case STATE_JUMP_TO_IDLE:
	case STATE_SKILL_TO_IDLE:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_IDLE;
		m_BlendFrame += 60.f / (1.f / deltaT);
		break;
	case STATE_FALL_TO_JUMP:
	case STATE_ATTACK_TO_JUMP:
	case STATE_FORWARD_TO_JUMP:
	case STATE_IDLE_TO_JUMP:
	case STATE_SKILL_TO_JUMP:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_JUMP;
		m_BlendFrame += 60.f / (1.f / deltaT);
		break;

	case STATE_FALL_TO_ATTACK:
	case STATE_JUMP_TO_ATTACK:
	case STATE_FORWARD_TO_ATTACK:
	case STATE_IDLE_TO_ATTACK:
	case STATE_SKILL_TO_ATTACK:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_ATTACK;
		m_BlendFrame += 60.f / (1.f / deltaT);
		break;

	case STATE_IDLE_TO_FALL:
	case STATE_FORWARD_TO_FALL:
	case STATE_JUMP_TO_FALL:
	case STATE_ATTACK_TO_FALL:
	case STATE_SKILL_TO_FALL:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_FALL;
		m_BlendFrame += 60.f / (1.f / deltaT);
		break;

	case STATE_IDLE_TO_SKILL:
	case STATE_FORWARD_TO_SKILL:
	case STATE_JUMP_TO_SKILL:
	case STATE_ATTACK_TO_SKILL:
	case STATE_FALL_TO_SKILL:
		if (m_BlendFrame > m_MaxBlendFrames)
			m_PlayerState = STATE_SKILL;
		m_BlendFrame += 60.f / (1.f / deltaT);
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
	case Character::STATE_ATTACK:
		strState = "Attack";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time
		);
		break;
	case Character::STATE_FALL:
		strState = "Fall";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time
		);
		break;
	case Character::STATE_SKILL:
		strState = "Skill";
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time
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
	case Character::STATE_IDLE_TO_ATTACK:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_IDLE_TO_FALL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_IDLE_TO_SKILL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
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
	case Character::STATE_FORWARD_TO_ATTACK:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
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
	case Character::STATE_FORWARD_TO_FALL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FORWARD_TO_SKILL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
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
	case Character::STATE_JUMP_TO_FORWARD:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_JUMP_TO_ATTACK:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_JUMP_TO_FALL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_JUMP_TO_SKILL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_ATTACK_TO_FORWARD:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_ATTACK_TO_JUMP:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_ATTACK_TO_IDLE:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_ATTACK_TO_FALL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_ATTACK_TO_SKILL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;

	case Character::STATE_FALL_TO_FORWARD:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FALL_TO_JUMP:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FALL_TO_IDLE:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FALL_TO_ATTACK:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_FALL_TO_SKILL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_SKILL_TO_FORWARD:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			m_MapAnimData["Run"]->m_Name,
			m_MapAnimData["Run"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_SKILL_TO_JUMP:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			m_MapAnimData["Jump"]->m_Name,
			m_MapAnimData["Jump"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_SKILL_TO_IDLE:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			m_MapAnimData["Idle"]->m_Name,
			m_MapAnimData["Idle"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_SKILL_TO_ATTACK:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			m_MapAnimData["Attack"]->m_Name,
			m_MapAnimData["Attack"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	case Character::STATE_SKILL_TO_FALL:
		m_SkinnedModelInst->ChangeSkinnedAnimation(
			m_MapAnimData["Skill"]->m_Name,
			m_MapAnimData["Skill"]->m_Time,
			m_MapAnimData["Fall"]->m_Name,
			m_MapAnimData["Fall"]->m_Time,
			(m_BlendFrame / m_MaxBlendFrames)
		);
		break;
	}

	if (strState == "")
		return;

	if (m_MapAnimData[strState]->m_Time > m_SkinnedModelInst->SkinnedInfo->GetClipEndTime(strState))
	{
		m_MapAnimData[strState]->m_Time = 0.f;
	}

}
Character::Character(std::string type, std::string id) :
	GameObject(type, id),
	m_PlayerController(nullptr),
	m_MyCamera(nullptr),
	m_PlayerState(PlayerState::STATE_IDLE),
	m_KeyState(PlayerState::STATE_IDLE)
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
	m_MapAnimData["Attack"] = std::make_unique<AnimData>("Attack", 0.f);
	m_MapAnimData["Fall"] = std::make_unique<AnimData>("Fall", 0.f);
	m_MapAnimData["Skill"] = std::make_unique<AnimData>("Skill", 0.f);
	m_MaxBlendFrames = 10.f;
}


Character::~Character()
{
}

void Character::InitializeTransform()
{
	GameObject::InitializeTransform();

	m_Position = { 0,0,0 };
	m_Right = { 1,0,0 };
	m_Up = { 0,1,0 };
	m_Look = { 0,0,1 };
}

void Character::Update(const float deltaT)
{
	SetState(deltaT);
	if (m_PlayerController)
		m_PlayerController->Update(deltaT);
	m_Bounds.Center = GetPosition();
	for (auto& p : m_MapAnimData)
	{
		p.second->m_Time += deltaT;
	}
	UpdateBoneTransforms();
	if (m_MyCamera != NULL) {
		CameraType cType = m_MyCamera->GetCameraType();
		if (cType == CameraType::Third)
		{
			XMFLOAT3 target;
			if (IsDead) {
				target = WatchPlayerPosition;
			}
			else
				target = GetPosition();
			target.y += 100;
			m_MyCamera->Update(target, deltaT);
			m_MyCamera->SetTarget(target);
		}
	}
}

void Character::SetCamera(Camera* myCamera, CameraType cameraType, bool IsFirst)
{
	m_MyCamera = myCamera;

	SetCamera(cameraType, IsFirst);
}

void Character::SetCamera(CameraType cameraType, bool IsFirst)
{
	if (m_MyCamera == nullptr) return;
	m_MyCamera->SetCamera(cameraType, this);

	switch (cameraType)
	{
	case CameraType::First:
		// ���� �����Ǻ��Ͱ� �߳��� ���� ������ �Ÿ��̵��ؾ���
		m_MyCamera->SetTimeLag(0.f);
		m_MyCamera->SetOffset(XMFLOAT3(0.0f, 180.f, 30.f));
		m_MyCamera->SetPosition(MathHelper::Add(GetPosition(), m_MyCamera->GetOffset()));
		break;
	case CameraType::Third:
		m_MyCamera->SetTimeLag(0.1f);
		if (IsFirst)
			m_MyCamera->SetOffset(XMFLOAT3(0.0f, 150.f, -300.f));
		else
			m_MyCamera->SetOffset(XMFLOAT3(0.0f, 110.f, -300.f));
		m_MyCamera->SetPosition(MathHelper::Add(GetPosition(), m_MyCamera->GetOffset()));
		break;
	case CameraType::Free:
		break;
	}
}

void Character::SetController()
{
	m_PlayerController = make_unique<PlayerController>(this);
}

void Character::SetParts(CharacterParts* parts)
{
	m_Parts[parts->m_PartsName] = parts;
}

void Character::SetDir(float angle)
{
	int R = 360.f - m_CurrentAngle;

	GameObject::Rotate(0, R + angle, 0);
	for (auto& p : m_Parts)
	{
		p.second->Rotate(0, R + angle, 0);
	}

	m_CurrentAngle = angle;
}

bool Character::ReleaseTransform()
{
	if (m_MyCamera)
	{
		if (m_MyCamera->GetCameraType() == CameraType::Third)
		{
			XMFLOAT3 extent = m_Bounds.Extents;
			if (m_Bounds.Extents.x > 100.f)
				m_MyCamera->SetOffset(XMFLOAT3(0.0f, 150.f, -300.f));
			else
				m_MyCamera->SetOffset(XMFLOAT3(0.0f, 150.f, -300.f));

		}
	}

	return true;
}

void Character::SetPosition(float posX, float posY, float posZ)
{
	GameObject::SetPosition(posX, posY, posZ);
	for (auto& p : m_Parts)
	{
		p.second->SetPosition(posX, posY, posZ);
	}
}

void Character::SetPosition(DirectX::XMFLOAT3 xmPos)
{
	float posX = xmPos.x;
	float posY = xmPos.y;
	float posZ = xmPos.z;
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



void Character::SetAnimationPlayerState(PlayerState playerState)
{
	m_PlayerState = playerState;
}

void Character::SetAnimationKeyState(PlayerState keyState)
{
	m_KeyState = keyState;
}

void Character::SetHide(bool hide)
{
	IsHide = hide;
}

bool Character::GetHide()const {
	return IsHide;
}

void Character::SetSnowmanNum(int num)
{
	SnowmanNum = num;
}

int Character::GetSnowmanNum()const {
	return SnowmanNum;
}

void Character::SetSkillCool(bool cool)
{
	IsSkillCool = cool;
}

bool Character::GetSkillCool()
{
	return IsSkillCool;
}

void Character::SetWatchPlayerPosition(float x, float y, float z)
{
	WatchPlayerPosition = XMFLOAT3(x, y, z);
}

void Character::SetWatchPlayerPosition(XMFLOAT3 v)
{
	WatchPlayerPosition = v;
}

