#include "headers.h"
#include "MainFramework.h"
#include "Network.h"
#include "GameInfo.h"

MainFramework::MainFramework() {};

MainFramework::~MainFramework() {};

void MainFramework::Initialize()
{
	m_pNetwork = new Network;
	m_pNetwork->InitSocket();
	m_pNetwork->Connect();

	m_pNetwork->m_pGameInfo = new GameInfo;
	m_pNetwork->m_pGameInfo->Initialize();
}
