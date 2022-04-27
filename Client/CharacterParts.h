#pragma once
#include "GameObject.h"

class CharacterParts : public GameObject
{
public:
	explicit CharacterParts(std::string type, std::string id);
	virtual ~CharacterParts();

public:
	std::string m_PartsName;
	float m_Offset; /* player의 중점좌표와의 거리 */
};

