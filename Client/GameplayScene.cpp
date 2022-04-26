#include "pch.h"
#include "GamePlayScene.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "SceneManager.h"

#include "MeshReference.h"
#include "MaterialReference.h"
#include "Character.h"
#include "CharacterParts.h"

#include "Network.h"
#include "MainFramework.h"


#include <random>
void GamePlayScene::Initialize()
{
	m_SceneController = new GameplayController;
}

bool GamePlayScene::Enter()
{
	cout << "Gameresult Scene" << endl;

	/*m_PlayerID = 0;

	m_Users[m_PlayerID] = AppContext->FindObject<Character>(CHARACTER_WIZARD, CHARACTER_WIZARD);
	m_Users[m_PlayerID]->m_IsVisible = true;
	m_Users[m_PlayerID]->m_PlayerRole = ROLE_MASTER;

	m_Users[1] = AppContext->FindObject<Character>(CHARACTER_DRUID, CHARACTER_DRUID);
	m_Users[1]->m_IsVisible = true;
	m_Users[2] = AppContext->FindObject<Character>(CHARACTER_BAIRD, CHARACTER_BAIRD);
	m_Users[2]->m_IsVisible = true;
	m_Users[3] = AppContext->FindObject<Character>(CHARACTER_SORCERER, CHARACTER_SORCERER);
	m_Users[3]->m_IsVisible = true;

	m_Users[m_PlayerID]->SetCamera(CACHE_CACHE::GetApp()->m_Camera, CameraType::Third);
	m_Users[m_PlayerID]->SetController(m_MapName);
	m_Users[m_PlayerID]->SetMapName(m_MapName);*/
	return false;
}

void GamePlayScene::Exit()
{
}

void GamePlayScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update();

}

void GamePlayScene::Render()
{
}
