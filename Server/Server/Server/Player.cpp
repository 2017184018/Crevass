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

char Player::GetAnimType()
{
	// 이동하면서 발사하면 그냥 공격애니메이션만 나온다.(현재는)
	//if( !m_ ) return ANIM_IDLE;
	if (is_hitted) {
		return ANIM_FALL;
	}

	if(is_attack){
		return ANIM_ATTACK;
	}
	if( is_jump ){
		return ANIM_JUMP;
	}
	if( m_keyW )
		return ANIM_MOVE;
	else if( m_keyS )
		return ANIM_MOVE;
	else if( m_keyA || (m_keyA && m_keyW) || (m_keyA && m_keyS) )
		return ANIM_MOVE;
	else if( m_keyD || (m_keyD && m_keyW) || (m_keyD && m_keyS) )
		return ANIM_MOVE;
	else
		return ANIM_IDLE;
}
