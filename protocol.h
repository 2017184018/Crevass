#pragma once
#include <DirectXMath.h>
#define SERVER_PORT			3500
///////////////////////////////
#define SC_LOGIN_OK			1
#define SC_READY			2
#define SC_GAMESTART		3
#define SC_REMOVE_PLAYER	4
#define SC_POS				5
#define SC_BULLETS			6
#define SC_HIT				7
#define SC_GAMEOVER			8
///////////////////////////////
#define CS_PLAYER_UP_UP			1
#define CS_PLAYER_DOWN_UP		2
#define CS_PLAYER_LEFT_UP		3
#define CS_PLAYER_RIGHT_UP		4

#define CS_PLAYER_UP_DOWN		5
#define CS_PLAYER_DOWN_DOWN		6
#define CS_PLAYER_LEFT_DOWN		7
#define CS_PLAYER_RIGHT_DOWN	8

#define CS_SHOOT_UP				9
#define CS_SHOOT_DOWN			10
#define CS_SHOOT_LEFT			11
#define CS_SHOOT_RIGHT			12

#define CS_SHOOT_RIGHT_UP		13
#define CS_SHOOT_RIGHT_DOWN		14
#define CS_SHOOT_LEFT_UP		15
#define CS_SHOOT_LEFT_DOWN		16

#define CS_READY				17







// ANIMATION TYPES
#define	ANIM_IDLE			0
#define ANIM_FORWARD		1
#define ANIM_ATTACK			2
#define ANIM_RUNNING		3
#define ANIM_JUMP		    4
#define ANIM_FALL		    5

#pragma pack(push ,1)


struct Pro_Player {
	char id;
	DirectX::XMFLOAT3 pos;
};

/* Server to Client */
struct sc_packet_login_ok {
	short size;
	char type;
	char id;
};

struct sc_packet_ready {
	short size;
	char type;
	char id;
	char ready;
};

struct sc_packet_game_start {
	short size;
	char type;
	Pro_Player players[3];
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
	Pro_Player players[3];
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
	char id;	//winner id -> 같으면 이긴거다 다르면 진거
};

/* Client to Server */

struct cs_packet {
	char type;
};
#pragma pack (pop)