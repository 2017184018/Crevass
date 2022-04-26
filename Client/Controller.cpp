#include "pch.h"
#include "Controller.h"
#include "InputHandler.h"
#include "SceneManager.h"

void TitleController::Update()
{
	HandleInput();
}

void TitleController::HandleInput()
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene();
	}
}

void GameroomController::Update()
{
	HandleInput();
}

void GameroomController::HandleInput()
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene();
	}
}

void LobbyController::Update()
{
	HandleInput();
}

void LobbyController::HandleInput()
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene();
	}
}

void GameplayController::Update()
{
	HandleInput();
}

void GameplayController::HandleInput()
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene();
	}
}

void GameresultController::Update()
{
	HandleInput();
}

void GameresultController::HandleInput()
{
	if (InputHandler::IsKeyUp(VK_RETURN))
	{
		SceneManager::GetApp()->ChangeScene(SceneType::Lobby);
	}
}
