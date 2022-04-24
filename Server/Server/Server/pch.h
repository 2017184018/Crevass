#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS

// Windows 헤더 파일입니다.
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>

// C 런타임 헤더 파일입니다.
#include <fstream>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <map>
#include <list>
#include <atomic>
#include <mutex>
#include <chrono>
using namespace std;
using namespace chrono;

// 공용 헤더파일 입니다.
#include "define.h"

// 외부 헤더파일 입니다.
#include "../../protocol.h"
#define BUFSIZE 512
