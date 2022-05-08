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

	float gravity = 0.0f;
	bool is_reset = false;
	float ResetTimeCount = 0.0f;

public:
	char dir = 0;
	char anim = 0;
	char hitted_dir = 0;
	
	DirectX::BoundingBox BB;
	DirectX::BoundingSphere Hit_BB;
	char m_CharacterType;
};