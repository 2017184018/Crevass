#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <WindowsX.h>
#include <windows.h>
#include <shellapi.h>
#include <wrl.h>
#include <Mmsystem.h>

// C 런타임 헤더 파일입니다.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>
#include <comdef.h>
#include <float.h>
#include <cstdint>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <string>
#include <cmath>
#include <map>
#include <set>
#include <unordered_map>
#include <sstream>

// DirectX 런타임 헤더 파일입니다.
#include "d3dx12.h"
#include <d3d11_1.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace DirectX::PackedVector;

#define SAFE_DELETE_PTR(ptr)	\
{								\
	if (ptr != nullptr)			\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}							\
}	

#define TOTAL_USER_COUNT 4
#define CHARACTER_INDEX_MASTER 0 
enum class RenderLayer { ID_OPAQUE, ID_SKY, ID_COUNT, ID_SkinnedOpaque };
enum class CameraType { First, Third, Free };

// Move & Rotate
#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

#define CAMERA_ZNEAR 1.f
#define CAMERA_ZFAR 10000.f