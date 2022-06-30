#pragma once
#include "Interface.h"

class Player :public Interface
{
public:
	Player();
	~Player();

	void SetKeyW(bool input);

	void SetKeyS(bool input);

	void SetKeyA(bool input);
	void SetKeyD(bool input);

	bool GetKeyW();
	bool GetKeyS();
	bool GetKeyA();
	bool GetKeyD();

	void SetCharacterType(char c_type);
	char GetCharacterType();

	char GetAnimType();

	float GetSpeed() const;
	float GetCrossSpeed() const;
	float GetHittedSpeed() const;
	void SetSpeed(float speed) ;
	void SetCrossSpeed(float crossspeed) ;
	void SetHittedSpeed(float crossspeed) ;
public:
	bool m_keyW = false;
	bool m_keyS = false;
	bool m_keyA = false;
	bool m_keyD = false;
	bool m_keySpace = false;
	bool m_keyAttack = false;

	bool is_attack = false;
	float AttackTimeCount = 0.0f;
	bool is_jump = false;
	float JumpTimeCount = 0.0f;
	bool is_hitted = false;
	float hittedTimeCount = 0.0f;
	bool is_Skill = false;

	float gravity = 0.0f;
	bool is_reset = false;
	float ResetTimeCount = 0.0f;

	bool IsHide = false;
	int SnowmanNum = -1;

	int CurrentBlockNum = -1;
	int TimeWhileBlock = 0;

public:
	char dir = 0;
	char anim = 0;
	char hitted_dir = 0;
	
	DirectX::BoundingBox BB;
	DirectX::BoundingSphere Hit_BB;
	char m_CharacterType;


private:
	float Speed = 1.5f;
	float Crossspeed = cos(45) * 1.5f;
	float Hittedspeed = 0.5f;
};