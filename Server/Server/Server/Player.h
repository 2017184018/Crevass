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

private:
	bool m_keyW = false;
	bool m_keyS = false;
	bool m_keyA = false;
	bool m_keyD = false;

};