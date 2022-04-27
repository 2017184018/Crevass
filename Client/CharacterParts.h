#pragma once
#include "GameObject.h"

class CharacterParts : public GameObject
{
public:
	explicit CharacterParts(std::string type, std::string id);
	virtual ~CharacterParts();

public:
	std::string m_PartsName;
	float m_Offset; /* player�� ������ǥ���� �Ÿ� */
};

