#include "pch.h"
#include "InputHandler.h"
#include "CREVASS.h"

namespace InputHandler
{
	bool	KeyFlag[256] = { 0, };

	bool InputHandler::IsKeyDown(int key)
	{
		//if (key == VK_RIGHT) {
		//	cout << KeyFlag[key] << endl;
		//}
		if (GetAsyncKeyState(key) & 0x8000) {
			if (!KeyFlag[key]) {
				KeyFlag[key] = true;
				return KeyFlag[key];
			}
		}
		return false;
	}

	bool InputHandler::IsKeyUp(int key)
	{
		if (GetAsyncKeyState(key) & 0x8000) {
			KeyFlag[key] = true;

		}
		else {
			if (KeyFlag[key]) {
				KeyFlag[key] = false;
				return !(KeyFlag[key]);
			}
		}
		return false;
	}
	bool IsOverlap(int key)
	{
		return (GetAsyncKeyState(key) & 0x8000);
	}
}
