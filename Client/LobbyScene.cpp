#include "pch.h"
#include "LobbyScene.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "MaterialReference.h"
#include "CREVASS.h"

void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
	AppContext->CreateLobby();
}

bool LobbyScene::Enter()
{
	cout << "Lobby Scene" << endl;
	m_SceneBounds.Center = XMFLOAT3(500.11, 0, 500);
	m_SceneBounds.Radius = sqrtf(2000.f * 2000.f + 2000.f * 2000.f);

	/* Light Setting */
	CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction = { 0.57735f, -0.57735f, 0.57735f };
	return false;
}

void LobbyScene::Exit()
{
	cout << "===========================================" << endl << endl;
}

void LobbyScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update(fDeltaTime);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);
	
}

void LobbyScene::Render()
{
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
}
