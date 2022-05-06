#pragma once
#include "Interface.h"

class Player :public Interface
{
public:
	Player();
	~Player();

	void SetKeyW(bool input);
	void SetKeyWA(bool input);
	void SetKeyWD(bool input);

	void SetKeyS(bool input);
	void SetKeySA(bool input);
	void SetKeySD(bool input);

	void SetKeyA(bool input);
	void SetKeyD(bool input);

	bool GetKeyW();
	bool GetKeyS();
	bool GetKeyA();
	bool GetKeyD();

	void SetCharacterType(char c_type);
	char GetCharacterType();

private:
	bool m_keyW = false;
	bool m_keyS = false;
	bool m_keyA = false;
	bool m_keyD = false;

public:
	char dir = 0;
	DirectX::BoundingBox BB;
	char m_CharacterType;
};