#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"

namespace Core
{
	extern HWND g_hMainWnd;
}

namespace InputHandler
{
	bool IsKeyDown(int key);
	bool IsKeyUp(int key);
	bool IsOverlap(int key);
	
}