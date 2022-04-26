#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <Winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <string>
#include <array>
#include <list>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <thread>
#define USE_STD using namespace std;
#define STD std::

// 사용자 정의 헤더 파일입니다.
#include "../protocol.h"
#include "define.h"