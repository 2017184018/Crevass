#pragma once

#define BUFSIZE (1024*16)	// 일단 여기 추가함
#define TOTAL_PLAYER_NUM 3
#define TOTAL_BULLET_NUM 100
#define TOTAL_LIFE_NUM	5

#define WINDOW_SIZE_X 500
#define WINDOW_SIZE_Y 500

#define _MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);

#define SAFE_DELETE_PTR(ptr)	\
{								\
	if (ptr != nullptr)			\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}							\
}								

typedef struct tag_point
{
	float x;
	float y;

	tag_point() : x(0.f), y(0.f) {};
	tag_point(const float& _x, const float& _y) : x(_x), y(_y) {};
	~tag_point() = default;
} FPOINT;

typedef struct tag_dir
{
	bool up;
	bool down;
	bool left;
	bool right;

	tag_dir() : up(false), down(false), left(false), right(false) {};
	tag_dir(const bool& _up, const bool& _down, const bool& _left, const bool& _right) :
		up(_up), down(_down), left(_left), right(_right) {};
	~tag_dir() = default;
} DIRECTION;