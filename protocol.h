#pragma once
#include <DirectXMath.h>
#define SERVER_PORT			3500
///////////////////////////////
#define SC_LOGIN_OK			1
#define SC_READY			2
#define SC_GAMESTART		3
#define SC_REMOVE_PLAYER	4
#define SC_POS				5
#define SC_ANIM				6
#define SC_HIT				7
#define SC_FALL				8
#define SC_RESET			10
#define SC_GAMEOVER			11
#define SC_BLOCK			12
#define SC_TIME				13
#define SC_HAIL				14
#define SC_FOX_SKILL				15
#define SC_HUSKY_SKILL				16
#define SC_PENGUIN_SKILL				17
#define SC_SEAL_SKILL				24
#define SC_BEAR_SKILL				26

#define SC_CHOOSE_PENGUIN			18
#define SC_CHOOSE_HUSKY				19
#define SC_CHOOSE_FOX				20
#define SC_CHOOSE_POLARBEAR			21
#define SC_CHOOSE_SEAL				22

#define SC_CRASH					23

#define SC_HITPLAYER			    25

///////////////////////////////
#define CS_PLAYER_UP_UP				1

#define CS_PLAYER_DOWN_UP			2

#define CS_PLAYER_LEFT_UP			3
#define CS_PLAYER_RIGHT_UP			4

#define CS_PLAYER_UP_DOWN			5

#define CS_PLAYER_DOWN_DOWN			6

#define CS_PLAYER_LEFT_DOWN			7
#define CS_PLAYER_RIGHT_DOWN		8

#define CS_PLAYER_IDLE				9
#define CS_PLAYER_MOVE				10
#define CS_PLAYER_JUMP				11
#define CS_PLAYER_ATTACK			12
#define CS_PLAYER_LOSE				13
#define CS_PLAYER_INTERACT				14
#define CS_PLAYER_SKILL				15

#define CS_READY_PENGUIN				17
#define CS_READY_HUSKY				18
#define CS_READY_FOX				19
#define CS_READY_POLARBEAR				20
#define CS_READY_SEAL				21

#define CS_CHOOSE_PENGUIN			22
#define CS_CHOOSE_HUSKY				23
#define CS_CHOOSE_FOX				24
#define CS_CHOOSE_POLARBEAR			25
#define CS_CHOOSE_SEAL				26







// ANIMATION TYPES
#define	ANIM_IDLE			0
#define ANIM_MOVE			1
#define ANIM_ATTACK			2
#define ANIM_JUMP		    3
#define ANIM_FALL			4
#define ANIM_SKILL			5

// player type
#define CHARACTER_NONE 0
#define CHARACTER_HUSKY 1
#define CHARACTER_PENGUIN 2
#define CHARACTER_POLARBEAR 3
#define CHARACTER_ARCTICFOX 4
#define CHARACTER_SEAL 5

#pragma pack(push ,1)


struct Pro_Player {
	char id;
	DirectX::XMFLOAT3 pos;
	char dir;
	char anim;
	char Character_type;
	bool IsHide;
	int SnowmanNum;
	bool IsSkillCool;
	int lifecnt;
};

struct Block {
	char id;
	DirectX::XMFLOAT3 pos;
	char destuctioncnt;
};

struct LobbyPlayerState
{
	char character_type;
	char ready;
};

/* Server to Client */
struct sc_packet_login_ok {
	short size;
	char type;
	char id;
	LobbyPlayerState players[5];
};

struct sc_packet_ready {
	short size;
	char type;
	char id;
	char ready;
	char Character_type;
};

struct sc_packet_choose {
	short size;
	char type;
	char id;
};

struct sc_packet_game_start {
	short size;
	char type;
	Pro_Player players[5];
	int iglooLocation[2];
	int SnowmanLocation[4];
	Block blocks[25];
};

struct sc_packet_remove_player {
	short size;
	char type;
	char id;
};

struct sc_packet_pos
{
	short size;
	char type;
	Pro_Player players[5];
};

struct sc_packet_anim
{
	short size;
	char type;
	Pro_Player players[5];
};
struct sc_packet_fall
{
	short size;
	char type;
	char id;
};

struct sc_packet_reset
{
	short size;
	char type;
	char id;
	char life;
};
struct sc_packet_hit {
	short size;
	char type;
	char id;
	char life;
};

struct sc_packet_gameover
{
	short size;
	char type;
	char id;	//winner id -> ������ �̱�Ŵ� �ٸ��� ����
};

struct sc_packet_block
{
	short size;
	char type;
	Block blocks[25];
};

struct sc_packet_crash
{
	short size;
	char type;
	int blocknum;
};

struct sc_packet_hitplayer
{
	short size;
	char type;
	char typenum;
	DirectX::XMFLOAT3 hit_pos;
};

struct sc_packet_time
{
	short size;
	char type;
	int time;
};
struct sc_packet_hail
{
	short size;
	char type;
	DirectX::XMFLOAT3 pos[5];
};
struct sc_packet_foxskill
{
	short size;
	char type;
	bool foxskill;
};
struct sc_packet_huskyskill
{
	short size;
	char type;
	bool huskyskill;
};
struct sc_packet_penguinskill
{
	short size;
	char type;
	bool penguinskill;
};
struct sc_packet_sealskill
{
	short size;
	char type;
	bool sealskill;
};
struct sc_packet_bearskill
{
	short size;
	char type;
	bool bearskill;
};

/* Client to Server */

struct cs_packet {
	char type;
};
#pragma pack (pop)