#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#define _CRT_SECURE_NO_WARNINGS

// Windows ��� �����Դϴ�.
#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <fstream>

// C++ ��Ÿ�� ��� �����Դϴ�.
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

// ���� ������� �Դϴ�.
#include "define.h"

// �ܺ� ������� �Դϴ�.
#include "../../protocol.h"
#define BUFSIZE 512
