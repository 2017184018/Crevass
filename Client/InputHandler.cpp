#include "pch.h"
#include "InputHandler.h"
#include "CREVASS.h"

namespace InputHandler
{
	bool	KeyFlag[256];

	bool InputHandler::IsKeyDown(int key)
	{
		if (GetAsyncKeyState(key) & 0x8000) {
			if (!KeyFlag[key])
				return KeyFlag[key] = true;
		}
		return false;
	}

	bool InputHandler::IsKeyUp(int key)
	{
		if (GetAsyncKeyState(key) & 0x8000)
			KeyFlag[key] = true;
		else
			if (KeyFlag[key])
				return !(KeyFlag[key] = false);
		return false;
	}
}
