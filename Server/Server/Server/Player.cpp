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

void Player::SetCharacterType(char c_type)
{
	m_CharacterType = c_type;
}

char Player::GetCharacterType()
{
	return m_CharacterType;
}

//char Player::GetAnimType()
//{
//	// 이동하면서 발사하면 그냥 공격애니메이션만 나온다.(현재는)
//	if( !m_isMovable ) return ANIM_IDLE;
//
//	if( m_isShoot ){
//		//m_isShoot = false;
//		return ANIM_ATTACK;
//	}
//
//	if( m_animJump ){
//		return ANIM_JUMP;
//	}
//
//	if( m_keyW )
//		return ANIM_FORWARD;
//	else if( m_keyS )
//		return ANIM_BACKWARD;
//	else if( m_keyA || (m_keyA && m_keyW) || (m_keyA && m_keyS) )
//		return ANIM_LEFT_STRAFE;
//	else if( m_keyD || (m_keyD && m_keyW) || (m_keyD && m_keyS) )
//		return ANIM_RIGHT_STRAFE;
//	else
//		return ANIM_IDLE;
//}
