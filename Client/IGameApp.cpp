#include "pch.h"
#include "IGameApp.h"

bool IGameApp::IsDone(void)
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		return true;
	return false;
}
