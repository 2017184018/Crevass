#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
#include <Winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
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

// ����� ���� ��� �����Դϴ�.
#include "../protocol.h"
#include "define.h"