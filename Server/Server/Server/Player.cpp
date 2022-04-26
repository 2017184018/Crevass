#include "pch.h"
#include "Player.h"

Player::Player()
{
	m_keyW = false;
	m_keyS = false;
	m_keyA = false;
	m_keyD = false;
}

Player::~Player()
{

}

void Player::SetKeyW(bool input)
{
	m_keyW = input;
}
void Player::SetKeyS(bool input)
{
	m_keyS = input;
}
void Player::SetKeyA(bool input)
{
	m_keyA = input;
}
void Player::SetKeyD(bool input)
{
	m_keyD = input;
}

bool Player::GetKeyW()
{
	return m_keyW;
}
bool Player::GetKeyS()
{
	return m_keyS;
}
bool Player::GetKeyA()
{
	return m_keyA;
}
bool Player::GetKeyD()
{
	return m_keyD;
}
