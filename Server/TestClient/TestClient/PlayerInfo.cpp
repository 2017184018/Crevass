#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	MYInfo = false;
	m_ID = -1;
	m_ReadyState = false;
}

PlayerInfo::~PlayerInfo()
{
}


void PlayerInfo::Initialize()
{
	m_ReadyState = false;
}

void PlayerInfo::SetID(char id, bool isMe)
{
	m_ID = static_cast<int>(id);
	MYInfo = isMe;
}

int PlayerInfo::GetID() const
{
	return m_ID;
}

void PlayerInfo::SetReadyState(bool readyState)
{
	m_ReadyState = readyState;
}

bool PlayerInfo::GetReadyState() const
{
	return m_ReadyState;
}
