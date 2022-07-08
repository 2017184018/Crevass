#include "pch.h"
#include "LobbyScene.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "MaterialReference.h"

void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
	AppContext->CreateLobby();
}

bool LobbyScene::Enter()
{
	cout << "Lobby Scene" << endl;
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
