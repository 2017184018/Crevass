#pragma once
#include "pch.h"
#include "Global.h"
#include "PhysicsThread.h"

/*	Ŭ���̾�Ʈ�� ������ ����Ǿ� �ְ�, ���� �����̸�
	 count�� ������ŵ�ϴ�. ����� Ŭ���̾�Ʈ�� ���� 2�� �̻��̰�
	 count�� �� ���� ������ true�� ��ȯ�մϴ�.*/

bool CheckGameStart()
{
	int count = 0;
	for (int i = 0; i < numOfCls; ++i)
	{
		g_ConnectedClsLock.lock();
		if (g_connectedCls[i].is_connected != false)
		{
			g_ConnectedClsLock.unlock();
			g_PlayerReadyInfoLock.lock();
			if (g_playerReadyInfo[i].ready != 0)
			{
				g_PlayerReadyInfoLock.unlock();
				count += 1;
				g_PlayerReadyInfoLock.lock();
			}
			g_PlayerReadyInfoLock.unlock();
			g_ConnectedClsLock.lock();
		}
		g_ConnectedClsLock.unlock();
	}

	std::cout << "numof cl : " << numOfCls << ", count : " << count << std::endl;

	if (numOfCls > 1 && count == numOfCls)
		return true;
	else
		return false;
}

void ErrQuit(const char* msg)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (WCHAR*)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void ErrDisplay(const char* msg)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	cout << "Error! - " << msg << " descripton: " << lpMsgBuf << endl;
}