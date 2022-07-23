#include "pch.h"
#include "GameplayScene.h"
#include "CREVASS.h"
#include "MeshReference.h"
#include "MaterialReference.h"
#include "CommandContext.h"
#include "ApplicationContext.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "CommandCenter.h"

#include "GameObject.h"
#include "Particle.h"
#include "Character.h"
#include "CharacterParts.h"
#include "Network.h"
#include "MainFramework.h"
#include <random>

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,8 };

using namespace Core;

void GameplayScene::Initialize()
{
	m_SceneController = new GameplayController(this);
	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreateBlocks();
	AppContext->Createigloos();
	AppContext->CreateWave();
	AppContext->CreateBackground();
	AppContext->CreateSnowmans();
	AppContext->CreateHail();
	AppContext->CreateWaterDrop();
	AppContext->CreateMinimap();
	AppContext->CreateOutline();

	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			AppContext->CreateUI2D("player_" + std::to_string(j) + "hp" + std::to_string(i), "player_" + std::to_string(j) + "hp" + std::to_string(i), 19, -350.f + i * 23.f, 180.f - j * 30.f, 20.f, 10.5f);
		}
	}
	AppContext->CreateUI2D("ui_Penguin", "ui_Penguin", 20, -380.f, 180.f, UI_SIZEX, UI_SIZEY);
	AppContext->CreateUI2D("ui_husky", "ui_husky", 21, -380.f, 150.f, UI_SIZEX, UI_SIZEY);
	AppContext->CreateUI2D("ui_Seal", "ui_Seal", 22, -380.f, 120.f, UI_SIZEX, UI_SIZEY);
	AppContext->CreateUI2D("ui_PolarBear", "ui_PolarBear", 23, -380.f, 90.f, UI_SIZEX, UI_SIZEY);
	AppContext->CreateUI2D("ui_ArcticFox", "ui_ArcticFox", 24, -380.f, 60.f, UI_SIZEX, UI_SIZEY);
	AppContext->CreateUI2D("ui_SkillOn", "ui_SkillOn", 25, -280.f, -260.f, 130.f, 40.f);
	for (int i = 0; i < 25; i++) {
		AppContext->CreateParticle("crushparticle", "crushparticle" + std::to_string(i), "Particle_Ice", false);
	}
	/*AppContext->CreateParticle("starParticle", "Penguin_star_particle" , "Particle_star");
	AppContext->CreateParticle("starParticle", "husky_star_particle", "Particle_star");
	AppContext->CreateParticle("starParticle", "PolarBear_star_particle", "Particle_star");
	AppContext->CreateParticle("starParticle", "ArcticFox_star_particle", "Particle_star");
	AppContext->CreateParticle("starParticle", "Seal_star_particle", "Particle_star");*/

	//AppContext->CreateParticle("snowParticle", "snowParticle", "Particle_snow");

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (AppContext->BlockCheck(5 * i + j))
			{
				AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j))->SetParticle("crushparticle", "crushparticle" + std::to_string(5 * i + j));
			}
			else {
				AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j))->SetParticle("crushparticle", "crushparticle" + std::to_string(5 * i + j));
			}
		}
	}

#ifdef DEBUG_SHADOW
	AppContext->CreateDebugBoundingBox("huskyBB", "huskyBB0");
#endif
	for (int i = 0; i < 25; ++i) {
		//IsShake[i] = false;
		//IsRight[i] = true;
		//ShakeCnt[i] = 0;
		DestructionCnt[i] = 0;
		//IsDown[i] = true;
	}

}

void GameplayScene::OnResize()
{
	GraphicsContext::GetApp()->OnResizeBlur();
}

bool GameplayScene::Enter()
{
	cout << "GamePlay Scene" << endl;
	/* Create SceneBounds for Shadow */
	m_SceneBounds.Center = XMFLOAT3(500.f, 0.0f, 500.f);
	m_SceneBounds.Radius = sqrtf(2000.f * 2000.f + 2000.f * 2000.f);
	/* Light Setting */
	CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction = { 0.47735f, -0.81735f, 1.07735 };

	AppContext->DisplayOutline();
	AppContext->DisplayMinimap();
	AppContext->DisplaySnowmans();
	AppContext->DisplayWaterDrop();
	AppContext->DisplayHail();
	AppContext->Displayigloos();
	AppContext->DisplayBlocks();
	AppContext->DisplayBackground();

	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			AppContext->DisplayUI("player_" + std::to_string(j) + "hp" + std::to_string(i), "player_" + std::to_string(j) + "hp" + std::to_string(i), 19, -350.f + i * 23.f, 180.f - j * 30.f, 20.f, 10.5f);
		}
	}
	AppContext->DisplayUI("ui_Penguin", "ui_Penguin", 20, -380.f, 180.f, UI_SIZEX, UI_SIZEY);
	AppContext->DisplayUI("ui_husky", "ui_husky", 21, -380.f, 150.f, UI_SIZEX, UI_SIZEY);
	AppContext->DisplayUI("ui_Seal", "ui_Seal", 22, -380.f, 120.f, UI_SIZEX, UI_SIZEY);
	AppContext->DisplayUI("ui_PolarBear", "ui_PolarBear", 23, -380.f, 90.f, UI_SIZEX, UI_SIZEY);
	AppContext->DisplayUI("ui_ArcticFox", "ui_ArcticFox", 24, -380.f, 60.f, UI_SIZEX, UI_SIZEY);
	AppContext->DisplayUI("ui_SkillOn", "ui_SkillOn", 25, -280.f, -260.f, 130.f, 40.f);

	//m_PlayerID = g_pFramework->m_pNetwork->m_pGameInfo->m_ClientID;
	//나 
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		int ty = g_pFramework->m_pNetwork->GetCharacterType(i);
		if (ty == CHARACTER_PENGUIN) {
			m_Users[i] = AppContext->FindObject<Character>("Penguin", "Penguin0");
		}
		else if (ty == CHARACTER_ARCTICFOX) {
			m_Users[i] = AppContext->FindObject<Character>("ArcticFox", "ArcticFox0");
		}
		else if (ty == CHARACTER_HUSKY) {
			m_Users[i] = AppContext->FindObject<Character>("husky", "husky0");
		}
		else if (ty == CHARACTER_SEAL) {
			m_Users[i] = AppContext->FindObject<Character>("Seal", "Seal0");
		}
		else /*if (ty == CHARACTER_POLARBEAR)*/ {
			m_Users[i] = AppContext->FindObject<Character>("PolarBear", "PolarBear0");
		}
		m_Users[i]->m_IsVisible = true;

	}
	WatchPlayerIdx = m_PlayerID;
	m_Users[m_PlayerID]->SetCamera(CREVASS::GetApp()->m_Camera, CameraType::Third, true);
	m_Users[m_PlayerID]->SetController();

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		m_Users[i]->SetPosition(g_pFramework->m_pNetwork->GetPlayerPos(i));
		m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
	}
	//이글루
	for (int i = 0; i < 2; ++i) {
		float scale = 2.0f / 10.0f;
		iglooIndex[i] = iglooLocaArray[g_pFramework->m_pNetwork->GetiglooLocation(i)];
		if (iglooIndex[i] % 4) {
			XMStoreFloat4x4(&AppContext->m_RItemsVec[76 + i]->m_World, XMMatrixScaling(scale, scale + 0.05, scale) * XMMatrixRotationY(3.14 * 5 / 6));
		}
		else {
			XMStoreFloat4x4(&AppContext->m_RItemsVec[76 + i]->m_World, XMMatrixScaling(scale, scale + 0.05, scale) * XMMatrixRotationY(3.14 * 7 / 6));
		}
	}
	//눈사람
	for (int i = 0; i < 4; ++i) {
		float scale = 3.0f / 5.0f;
		SnowmanIndex[i] = g_pFramework->m_pNetwork->GetSnowmanLocation(i);
	}


	//여기서 초기화
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		AppContext->FindObject<GameObject>("ui_" + m_Users[i]->GetType(), "ui_" + m_Users[i]->GetType())->SetPosition(-380.f, 180.f - i * 30.f, 1.f);
		AppContext->FindObject<GameObject>("ui_" + m_Users[i]->GetType(), "ui_" + m_Users[i]->GetType())->m_positionRatio = { (-380.f - (UI_SIZEX / 20.f)) / 800.f, (180.f - i * 30.f - (UI_SIZEY / 20.f)) / 600.f };

	}

	//눈 파티클 시작
	AppContext->DisplayParticle("snowParticle", "snowParticle", XMFLOAT3(500, 500, 800));

	return false;
}

void GameplayScene::Exit()
{
	//CommandCenter Reset
	CommandCenter::GetApp()->Release();
	GraphicsContext::GetApp()->OnBlurEffect(false);

	AppContext->HiddenBlocks();
	AppContext->HiddenBackground();
	AppContext->HiddenOutline();
	AppContext->HiddenMinimap();
	AppContext->HiddenSnowmans();
	AppContext->HiddenWaterDrop();
	AppContext->HiddenHail();
	AppContext->Hiddenigloos();

	AppContext->HiddenCharacter("Penguin", "Penguin0");
	AppContext->HiddenCharacter("Husky", "Husky0");
	AppContext->HiddenCharacter("ArcticFox", "ArcticFox0");
	AppContext->HiddenCharacter("PolarBear", "PolarBear0");
	AppContext->HiddenCharacter("Seal", "Seal0");

	AppContext->HiddenParticle("starParticle", "Penguin_star_particle");
	AppContext->HiddenParticle("starParticle", "husky_star_particle");
	AppContext->HiddenParticle("starParticle", "PolarBear_star_particle");
	AppContext->HiddenParticle("starParticle", "ArcticFox_star_particle");
	AppContext->HiddenParticle("starParticle", "Seal_star_particle");


	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			AppContext->HiddenUI("player_" + std::to_string(i) + "hp" + std::to_string(j), "player_" + std::to_string(i) + "hp" + std::to_string(j));
		}
	}
	AppContext->HiddenUI("ui_Penguin", "ui_Penguin");
	AppContext->HiddenUI("ui_husky", "ui_husky");
	AppContext->HiddenUI("ui_Seal", "ui_Seal");
	AppContext->HiddenUI("ui_PolarBear", "ui_PolarBear");
	AppContext->HiddenUI("ui_ArcticFox", "ui_ArcticFox");
	AppContext->HiddenUI("ui_SkillOn", "ui_SkillOn");



	//particle
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			AppContext->HiddenParticle("crushparticle", "crushparticle" + std::to_string(5 * i + j));
		}
	}

	AppContext->HiddenParticle("snowParticle", "snowParticle");

	cout << "exit===========================================" << endl << endl;
}

void GameplayScene::Update(const float& fDeltaTime)
{
	m_Timer = 300 - (g_pFramework->m_pNetwork->Gettime()) / 60.0f;
	m_SceneController->Update(fDeltaTime);
	static float time = 0.0f;
	static XMFLOAT3 huskyimagepos[4];
	static XMFLOAT4X4 huskyimagerota[4];
	time += fDeltaTime;
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		m_Users[i]->SetHide(g_pFramework->m_pNetwork->GetPlayerHide(i));
		m_Users[i]->SetSnowmanNum(g_pFramework->m_pNetwork->GetPlayerSnowmanNum(i));

		if (g_pFramework->m_pNetwork->GetHuskySkill()) {
			if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY && time >= 0.04f) {
				for (int j = 3; j >= 0; --j) {
					if (j != 0) {
						huskyimagepos[j] = huskyimagepos[j - 1];
						huskyimagerota[j] = huskyimagerota[j - 1];
					}
					else {
						huskyimagepos[j] = g_pFramework->m_pNetwork->GetPlayerPos(i);
						const std::map<std::string, UINT>& info = AppContext->m_RItemsMap["husky"]->GetinstanceKeymap();
						huskyimagerota[j] = AppContext->m_RItemsVec[info.begin()->second]->m_World;
						time = 0.0f;
					}
				}
			}
		}
		else {
			for (int j = 0; j < 4; ++j) {
				huskyimagepos[j] = XMFLOAT3(-1000, -1000, -1000);
			}
		}

		if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
			m_Users[i]->m_PlayerController->SetLoop(g_pFramework->m_pNetwork->GetPenguinSkill());
		}

		m_Users[i]->SetPosition(g_pFramework->m_pNetwork->GetPlayerPos(i));
		m_Users[i]->SetDir((g_pFramework->m_pNetwork->GetPlayerDir(i)) * 45);

		if (i != m_PlayerID) {//애니메이션은 나는 제외 
			switch (g_pFramework->m_pNetwork->GetPlayerAnim(i))
			{
			case ANIM_IDLE:
				m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
				m_Users[i]->is_StartFallAnim = false;
				break;
			case ANIM_MOVE:
				m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_FORWARD);
				m_Users[i]->is_StartFallAnim = false;
				break;
			case ANIM_ATTACK:
				m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_ATTACK);
				m_Users[i]->is_StartFallAnim = false;
				break;
			case ANIM_JUMP:
				m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_JUMP);
				m_Users[i]->is_StartFallAnim = false;
				break;
			case ANIM_SKILL:
				m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_SKILL);
				m_Users[i]->is_StartFallAnim = false;
				break;
			}
		}
		if (g_pFramework->m_pNetwork->GetCharacterFall(i) || g_pFramework->m_pNetwork->GetPlayerAnim(i) == ANIM_FALL) {
			m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_FALL);
			m_Users[i]->is_StartFallAnim = true;
		}
		else {
			m_Users[i]->is_StartFallAnim = false;
		}
	}
	m_Users[m_PlayerID]->SetSkillCool(g_pFramework->m_pNetwork->GetPlayerSkillCool(m_PlayerID));

	if (m_Users[m_PlayerID]->GetSkillCool()) {
		AppContext->FindObject<GameObject>("ui_SkillOn", "ui_SkillOn")->m_MaterialIndex = 26;
	}
	else if (!m_Users[m_PlayerID]->GetSkillCool()) {
		AppContext->FindObject<GameObject>("ui_SkillOn", "ui_SkillOn")->m_MaterialIndex = 25;
	}

	AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(11))->UpdateParticleTime(fDeltaTime);

	for (int i = 0; i < 25; i++)
		AppContext->FindObject<Particle>("crushparticle", "crushparticle" + std::to_string(i))->Update(fDeltaTime);

	AppContext->FindObject<Particle>("snowParticle", "snowParticle")->Update(fDeltaTime);
	AppContext->FindObject<Particle>("starParticle", "husky_star_particle")->Update(fDeltaTime);
	AppContext->FindObject<Particle>("starParticle", "Penguin_star_particle")->Update(fDeltaTime);
	AppContext->FindObject<Particle>("starParticle", "PolarBear_star_particle")->Update(fDeltaTime);
	AppContext->FindObject<Particle>("starParticle", "ArcticFox_star_particle")->Update(fDeltaTime);
	AppContext->FindObject<Particle>("starParticle", "Seal_star_particle")->Update(fDeltaTime);


	for (int i = 0; i < 25; ++i) {
		AppContext->m_RItemsVec[2 * i + 1]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[2 * (i + 1)]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[51 + i]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		DestructionCnt[i] = g_pFramework->m_pNetwork->GetBlockDestructionCnt(i);
		static 	XMMATRIX mat = XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World);
		if (DestructionCnt[i] == 0) {
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._11 = 1;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._22 = 1;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._33 = 1;
			//cout << "how meny -" << AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(1))->m_Particles.size() << endl;

			AppContext->m_RItemsVec[2 * i + 1]->m_World._11 = 1;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._22 = 1;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._33 = 1;
			AppContext->m_RItemsVec[i + 51]->m_World._11 = 7.5 / 10.0;
			AppContext->m_RItemsVec[i + 51]->m_World._22 = 1;
			AppContext->m_RItemsVec[i + 51]->m_World._33 = 7.5 / 10.0;

			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._11 = 1.0f;
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._22 = 1.0f;
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._33 = 1.0f;
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._11 = 1.0f;
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._22 = 1.0f;
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._33 = 1.0f;
			AppContext->m_RItemsVec[i + 51 + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._11 = 7.5 / 10.0;
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._22 = 1.0f;
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._33 = 7.5 / 10.0;
			XMStoreFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World) * mat);
			XMStoreFloat4x4(&AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World) * mat);
			XMStoreFloat4x4(&AppContext->m_RItemsVec[i + 51 + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[i + 51 + 222]->m_World) * mat);
		}
		if (DestructionCnt[i] == 1) {
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._11 = 0;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._22 = 0;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._33 = 0;

			auto matrix = XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World);
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._11 = 0.0f;
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._22 = 0.0f;
			AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._33 = 0.0f;
			XMStoreFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World) * matrix);
		}
		else if (DestructionCnt[i] == 2) {
			AppContext->m_RItemsVec[2 * i + 1]->m_World._11 = 0;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._22 = 0;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._33 = 0;

			auto matrix = XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World);
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._11 = 0.0f;
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._22 = 0.0f;
			AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World._33 = 0.0f;
			XMStoreFloat4x4(&AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[2 * i + 1 + 222]->m_World) * matrix);
		}
		else if (DestructionCnt[i] == 3) {
			AppContext->m_RItemsVec[i + 51]->m_World._11 = 0;
			AppContext->m_RItemsVec[i + 51]->m_World._22 = 0;
			AppContext->m_RItemsVec[i + 51]->m_World._33 = 0;

			auto matrix = XMLoadFloat4x4(&AppContext->m_RItemsVec[i + 51 + 222]->m_World);
			AppContext->m_RItemsVec[i + 51 + 222]->m_World = MathHelper::Identity4x4();
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._11 = 0.0f;
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._22 = 0.0f;
			AppContext->m_RItemsVec[i + 51 + 222]->m_World._33 = 0.0f;
			XMStoreFloat4x4(&AppContext->m_RItemsVec[i + 51 + 222]->m_World, XMLoadFloat4x4(&AppContext->m_RItemsVec[i + 51 + 222]->m_World) * matrix);
		}
		AppContext->m_RItemsVec[2 * i + 1]->m_World._41 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._41 = AppContext->m_RItemsVec[51 + i]->m_World._41;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._42 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._42 = AppContext->m_RItemsVec[51 + i]->m_World._42;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._43 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._43 = AppContext->m_RItemsVec[51 + i]->m_World._43;
	}

	for (auto& p : m_Users)
	{
		if (!p.second) continue;

		p.second->Update(fDeltaTime);
	}

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		if (g_pFramework->m_pNetwork->GetCharacterFall(i) == true)
		{
			if (i == m_PlayerID)
			{

				m_Users[i]->m_PlayerController->Fall();
				Fall(i);
			}
		}
	}
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		//누군가 떨어졌을때
		if (g_pFramework->m_pNetwork->GetCharacterReset(i) == true)
		{
			Player_Lifecnt[i] = g_pFramework->m_pNetwork->GetPlayerLifeCnt(i);

			for (int j = 4; j > Player_Lifecnt[i] - 1; j--) {
				AppContext->HiddenUI("player_" + std::to_string(i) + "hp" + std::to_string(j), "player_" + std::to_string(i) + "hp" + std::to_string(j));
			}

			g_pFramework->m_pNetwork->SetCharacterReset(i);
			g_pFramework->m_pNetwork->SetCharacterFall(i);

			//내가 떨어졌을때 
			m_Users[m_PlayerID]->m_PlayerController->SetIsFall();
			GraphicsContext::GetApp()->OnBlurEffect(false);

			IsFall[m_PlayerID] = false;
			if (Player_Lifecnt[m_PlayerID] == 0) {
				g_pFramework->m_pNetwork->Send(CS_PLAYER_LOSE);
			}
		}
		if (Player_Lifecnt[m_PlayerID] == 0) {
			m_Users[m_PlayerID]->m_IsVisible = false;

			while (Player_Lifecnt[WatchPlayerIdx] == 0) {
				++WatchPlayerIdx;
				if (WatchPlayerIdx == g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum) {
					WatchPlayerIdx = 0;
				}
			}

			if (!Inactive) {
				static bool up = false;
				static bool down = false;
				if (GetAsyncKeyState(VK_UP) & 0x8000) {
					if (up) {
						do {
							++WatchPlayerIdx;
							if (WatchPlayerIdx == g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum) {
								WatchPlayerIdx = 0;
							}
						} while (Player_Lifecnt[WatchPlayerIdx] == 0);
						up = false;
					}
				}
				else {
					up = true;
				}
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
					if (down) {
						do {
							--WatchPlayerIdx;
							if (WatchPlayerIdx == -1) {
								WatchPlayerIdx = g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum - 1;
							}
						} while (Player_Lifecnt[WatchPlayerIdx] == 0);
						down = false;
					}
				}
				else {
					down = true;
				}
			}
			m_Users[WatchPlayerIdx]->SetCamera(CREVASS::GetApp()->m_Camera, CameraType::Third, false);
			m_Users[WatchPlayerIdx]->m_MyCamera->SetLook(0,0,1);
			m_Users[m_PlayerID]->IsDead = true;
			m_Users[m_PlayerID]->SetWatchPlayerPosition(m_Users[WatchPlayerIdx]->GetPosition());
		}
	}
	{
		//syncro igloo
		if (iglooIndex[0] % 4) {
			AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * iglooIndex[0] + 1]->m_World._41 - 20;
		}
		else {
			AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * iglooIndex[0] + 1]->m_World._41 + 20;
		}
		AppContext->m_RItemsVec[76]->m_World._42 = AppContext->m_RItemsVec[2 * iglooIndex[0] + 1]->m_World._42 + 60;
		AppContext->m_RItemsVec[76]->m_World._43 = AppContext->m_RItemsVec[2 * iglooIndex[0] + 1]->m_World._43 + 20;

		if (iglooIndex[1] % 4) {
			AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * iglooIndex[1] + 1]->m_World._41 - 20;
		}
		else {
			AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * iglooIndex[1] + 1]->m_World._41 + 20;
		}

		AppContext->m_RItemsVec[77]->m_World._42 = AppContext->m_RItemsVec[2 * iglooIndex[1] + 1]->m_World._42 + 60;
		AppContext->m_RItemsVec[77]->m_World._43 = AppContext->m_RItemsVec[2 * iglooIndex[1] + 1]->m_World._43 + 20;
	}
	{
		//syncro snowman
		for (int i = 0; i < 4; ++i) {

			float scale = 0;
			if (DestructionCnt[SnowmanIndex[i]] < 3) {
				scale = 0.6 - DestructionCnt[SnowmanIndex[i]] * 0.1;
			}
			else {
				scale = 0;
			}

			//static float rota = 0;
			//static bool direct = true;
			if (SnowmanIndex[i] % 5 == 1 || SnowmanIndex[i] % 5 == 3) {
				XMStoreFloat4x4(&AppContext->m_RItemsVec[209 + i]->m_World, XMMatrixScaling(scale, scale, scale) * XMMatrixRotationY(3.14 * 5 / 6));
			}
			else {
				XMStoreFloat4x4(&AppContext->m_RItemsVec[209 + i]->m_World, XMMatrixScaling(scale, scale, scale) * XMMatrixRotationY(3.14 * 7 / 6));
			}

			if (SnowmanIndex[i] % 5) {
				AppContext->m_RItemsVec[209 + i]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[i] + 1]->m_World._41 - 20;
			}
			else {
				AppContext->m_RItemsVec[209 + i]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[i] + 1]->m_World._41 - 20;
			}
			AppContext->m_RItemsVec[209 + i]->m_World._42 = AppContext->m_RItemsVec[2 * SnowmanIndex[i] + 1]->m_World._42 + 20;
			AppContext->m_RItemsVec[209 + i]->m_World._43 = AppContext->m_RItemsVec[2 * SnowmanIndex[i] + 1]->m_World._43 + 20;

		}
	}

	{	//waterdrop
		for (int i = 0; i < 4; ++i) {
			AppContext->FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i))->m_World._41 = m_Users[m_PlayerID]->GetPosition().x;
			AppContext->FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i))->m_World._42 = m_Users[m_PlayerID]->GetPosition().y + 380;
			AppContext->FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->GetPosition().z - 30;
			/*if (i == 0 || i == 3)
				AppContext->FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->GetPosition().z - 85;
			else
				AppContext->FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->GetPosition().z + 25;*/
		}
	}

	{
		for (int j = 0; j < 3; ++j) {
			AppContext->FindObject<GameObject>("husky", "husky" + std::to_string(105 + j))->m_World = huskyimagerota[j + 1];
			AppContext->FindObject<GameObject>("husky", "husky" + std::to_string(105 + j))->SetPosition(huskyimagepos[j + 1]);
			AppContext->FindObject<GameObject>("husky", "husky" + std::to_string(105 + j))->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["huskyimage" + std::to_string(j + 1)]->MatCBIndex;
			AppContext->FindObject<GameObject>("husky", "husky" + std::to_string(105 + j))->m_IsVisible = true;
		}
	}
	{		//minimap
		float FallDistance = 200;
		{
			const std::map<std::string, UINT>& Sea = AppContext->m_RItemsMap["MinimapSea"]->GetinstanceKeymap();
			if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
			{
				AppContext->m_RItemsVec[Sea.begin()->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
			}
			else {
				AppContext->m_RItemsVec[Sea.begin()->second]->m_World._41 = m_Users[m_PlayerID]->m_World._41 + 10;
				AppContext->m_RItemsVec[Sea.begin()->second]->m_World._42 = m_Users[m_PlayerID]->m_World._42 + 100;
				AppContext->m_RItemsVec[Sea.begin()->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43;
			}
		}
		{
			for (int i = 0; i < 25; ++i) {
				if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
				{
					if (i == 0 || i == 2 || i == 4 || i == 10 || i == 12 || i == 14 || i == 20 || i == 22 || i == 24) {
						AppContext->FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
					}
					else {
						AppContext->FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
					}
					AppContext->FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
					AppContext->FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(i))->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
				}
				else {
					if (i == 0 || i == 2 || i == 4 || i == 10 || i == 12 || i == 14 || i == 20 || i == 22 || i == 24) {
						AppContext->FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(i))->m_World._41 = MinimapCubePos[i].x + m_Users[m_PlayerID]->m_World._41 + 10;
						AppContext->FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(i))->m_World._42 = MinimapCubePos[i].y + m_Users[m_PlayerID]->m_World._42 + 100;
						AppContext->FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(i))->m_World._43 = MinimapCubePos[i].z + m_Users[m_PlayerID]->m_World._43;
					}
					else {
						AppContext->FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(i))->m_World._41 = MinimapCubePos[i].x + m_Users[m_PlayerID]->m_World._41 + 10;
						AppContext->FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(i))->m_World._42 = MinimapCubePos[i].y + m_Users[m_PlayerID]->m_World._42 + 100;
						AppContext->FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(i))->m_World._43 = MinimapCubePos[i].z + m_Users[m_PlayerID]->m_World._43;
					}
					AppContext->FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(i))->m_World._41 = MinimapCubePos[i].x + m_Users[m_PlayerID]->m_World._41 + 10;
					AppContext->FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(i))->m_World._42 = MinimapCubePos[i].y + m_Users[m_PlayerID]->m_World._42 + 100;
					AppContext->FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(i))->m_World._43 = MinimapCubePos[i].z + m_Users[m_PlayerID]->m_World._43;
					AppContext->FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(i))->m_World._41 = MinimapCubePos[i].x + m_Users[m_PlayerID]->m_World._41 + 10;
					AppContext->FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(i))->m_World._42 = MinimapCubePos[i].y + m_Users[m_PlayerID]->m_World._42 + 100;
					AppContext->FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(i))->m_World._43 = MinimapCubePos[i].z + m_Users[m_PlayerID]->m_World._43;
				}
				//cout << i << ": " << AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._41 << ", " << AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._42 <<
				//	", " << AppContext->m_RItemsVec[2 * (i + 1) + 222]->m_World._43 << endl;
			}
		}
		{
			const std::map<std::string, UINT>& icon = AppContext->m_RItemsMap["myicon"]->GetinstanceKeymap();
			if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
			{
				AppContext->m_RItemsVec[icon.begin()->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
			}
			else {
				AppContext->m_RItemsVec[icon.begin()->second]->m_World._41 = m_Users[m_PlayerID]->m_World._41 / 200.0f * 400.0f / 11.0f - 800.0f / 11.0f + m_Users[m_PlayerID]->m_World._41 + 10;
				AppContext->m_RItemsVec[icon.begin()->second]->m_World._42 = m_Users[m_PlayerID]->m_World._43 / 200.0f * 400.0f / 11.0f + 20.0f + m_Users[m_PlayerID]->m_World._42 + 4;
				AppContext->m_RItemsVec[icon.begin()->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 / 200.0f * 0.56f - 42.73f + m_Users[m_PlayerID]->m_World._43 - 14;
			}
		}
		{
			const std::map<std::string, UINT>& snowmanicon = AppContext->m_RItemsMap["snowmanicon"]->GetinstanceKeymap();
			auto i = snowmanicon.begin();
			for (int j = 0; j < 4; ++j) {
				if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
				{
					AppContext->m_RItemsVec[i->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
				}
				else {
					if (DestructionCnt[SnowmanIndex[j]] < 3) {
						AppContext->m_RItemsVec[i->second]->m_World._41 = (SnowmanIndex[j] / 5) * 400.0f / 11.0f - 800.0f / 11.0f + m_Users[m_PlayerID]->m_World._41 + 10;
						AppContext->m_RItemsVec[i->second]->m_World._42 = (SnowmanIndex[j] % 5) * 400.0f / 11.0f + 20.0f + m_Users[m_PlayerID]->m_World._42 + 3;
						AppContext->m_RItemsVec[i->second]->m_World._43 = (SnowmanIndex[j] % 5) * 0.56f - 42.73f + m_Users[m_PlayerID]->m_World._43 - 13;
					}
					else {
						AppContext->m_RItemsVec[i->second]->m_World._41 = -1000;
						AppContext->m_RItemsVec[i->second]->m_World._42 = -1000;
						AppContext->m_RItemsVec[i->second]->m_World._43 = -1000;
					}
				}
				++i;
			}
		}
		{
			const std::map<std::string, UINT>& iglooicon = AppContext->m_RItemsMap["iglooicon"]->GetinstanceKeymap();
			auto i = iglooicon.begin();
			for (int j = 0; j < 2; ++j) {
				if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
				{
					AppContext->m_RItemsVec[i->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
				}
				else {
					AppContext->m_RItemsVec[i->second]->m_World._41 = (iglooIndex[j] / 5) * 400.0f / 11.0f - 800.0f / 11.0f + m_Users[m_PlayerID]->m_World._41 + 10;
					AppContext->m_RItemsVec[i->second]->m_World._42 = (iglooIndex[j] % 5) * 400.0f / 11.0f + 20.0f + m_Users[m_PlayerID]->m_World._42 + 3;
					AppContext->m_RItemsVec[i->second]->m_World._43 = (iglooIndex[j] % 5) * 0.56f - 42.73f + m_Users[m_PlayerID]->m_World._43 - 13;
				}
				++i;
			}
		}
		{
			const std::map<std::string, UINT>& enermyicon = AppContext->m_RItemsMap["enermyicon"]->GetinstanceKeymap();
			auto tmp = enermyicon.begin();
			for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
				if (i != m_PlayerID) {
					if (g_pFramework->m_pNetwork->GetCharacterFall(m_PlayerID))
					{
						AppContext->m_RItemsVec[tmp->second]->m_World._43 = m_Users[m_PlayerID]->m_World._43 - FallDistance;
					}
					else {
						if ((g_pFramework->m_pNetwork->GetFoxSkill() && g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) ||
							m_Users[i]->GetHide() || distance(m_Users[m_PlayerID]->GetPosition(), m_Users[i]->GetPosition()) > 400.0f) {
							AppContext->m_RItemsVec[tmp->second]->m_World._41 = -1000;
							AppContext->m_RItemsVec[tmp->second]->m_World._42 = -1000;
							AppContext->m_RItemsVec[tmp->second]->m_World._43 = -1000;
						}
						else {
							AppContext->m_RItemsVec[tmp->second]->m_World._41 = m_Users[i]->m_World._41 / 200.0f * 400.0f / 11.0f - 800.0f / 11.0f + m_Users[m_PlayerID]->m_World._41 + 10;
							AppContext->m_RItemsVec[tmp->second]->m_World._42 = m_Users[i]->m_World._43 / 200.0f * 400.0f / 11.0f + 20.0f + m_Users[m_PlayerID]->m_World._42 + 3.03;
							AppContext->m_RItemsVec[tmp->second]->m_World._43 = m_Users[i]->m_World._43 / 200.0f * 0.56f - 42.73f + m_Users[m_PlayerID]->m_World._43 - 13.03;
						}
					}
					++tmp;
				}
			}
		}
	}
	{	//outline
		float outlinesize = 1.05;
		float syncoutline = (outlinesize - 1) * 20;
		for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
			if (i != m_PlayerID) {
				if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
					const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["PenguinOutline"]->GetinstanceKeymap();
					auto tmp = myoutline.begin();
					XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
					tmpuser._11 = outlinesize;
					tmpuser._22 = outlinesize;
					tmpuser._33 = outlinesize;

					XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[i]->m_World));
					AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
					AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
					AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
				}
				else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
					const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["ArcticFoxOutline"]->GetinstanceKeymap();
					auto tmp = myoutline.begin();
					XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
					tmpuser._11 = outlinesize;
					tmpuser._22 = outlinesize;
					tmpuser._33 = outlinesize;

					XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[i]->m_World));
					AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
					AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
					AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
				}
				else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
					const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["huskyOutline"]->GetinstanceKeymap();
					auto tmp = myoutline.begin();
					XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
					tmpuser._11 = outlinesize;
					tmpuser._22 = outlinesize;
					tmpuser._33 = outlinesize;

					XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[i]->m_World));
					AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
					AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
					AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
				}
				else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
					const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["SealOutline"]->GetinstanceKeymap();
					auto tmp = myoutline.begin();
					XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
					tmpuser._11 = outlinesize;
					tmpuser._22 = outlinesize;
					tmpuser._33 = outlinesize;

					XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[i]->m_World));
					AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
					AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline / 20 * 15;
					AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
					if (g_pFramework->m_pNetwork->GetSealSkill()) {
						AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 2;
					}
					else {
						AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 1;
					}
				}
				else {
					const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["PolarBearOutline"]->GetinstanceKeymap();
					auto tmp = myoutline.begin();
					XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
					tmpuser._11 = outlinesize;
					tmpuser._22 = outlinesize;
					tmpuser._33 = outlinesize;

					XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[i]->m_World));
					AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
					AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
					AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
				}
			}
		}
		if (g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) == CHARACTER_PENGUIN) {
			const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["PenguinOutline"]->GetinstanceKeymap();
			auto tmp = myoutline.begin();
			XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
			tmpuser._11 = outlinesize;
			tmpuser._22 = outlinesize;
			tmpuser._33 = outlinesize;

			XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[m_PlayerID]->m_World));
			AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
			AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
			AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 0;
			AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
		}
		else if (g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) == CHARACTER_ARCTICFOX) {
			const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["ArcticFoxOutline"]->GetinstanceKeymap();
			auto tmp = myoutline.begin();
			XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
			tmpuser._11 = outlinesize;
			tmpuser._22 = outlinesize;
			tmpuser._33 = outlinesize;

			XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[m_PlayerID]->m_World));
			AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
			AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
			AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 0;
			AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
		}
		else if (g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) == CHARACTER_HUSKY) {
			const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["huskyOutline"]->GetinstanceKeymap();
			auto tmp = myoutline.begin();
			XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
			tmpuser._11 = outlinesize;
			tmpuser._22 = outlinesize;
			tmpuser._33 = outlinesize;

			XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[m_PlayerID]->m_World));
			AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
			AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
			AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 0;
			AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
		}
		else if (g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) == CHARACTER_SEAL) {
			const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["SealOutline"]->GetinstanceKeymap();
			auto tmp = myoutline.begin();
			XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
			tmpuser._11 = outlinesize;
			tmpuser._22 = outlinesize;
			tmpuser._33 = outlinesize;

			XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[m_PlayerID]->m_World));
			AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
			AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline / 20 * 15;
			if (g_pFramework->m_pNetwork->GetSealSkill()) {
				AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 2;
			}
			else {
				AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 0;
			}
			AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
		}
		else {
			const std::map<std::string, UINT>& myoutline = AppContext->m_RItemsMap["PolarBearOutline"]->GetinstanceKeymap();
			auto tmp = myoutline.begin();
			XMFLOAT4X4 tmpuser = MathHelper::Identity4x4();
			tmpuser._11 = outlinesize;
			tmpuser._22 = outlinesize;
			tmpuser._33 = outlinesize;

			XMStoreFloat4x4(&tmpuser, XMLoadFloat4x4(&tmpuser) * XMLoadFloat4x4(&m_Users[m_PlayerID]->m_World));
			AppContext->m_RItemsVec[tmp->second]->m_World = tmpuser;
			AppContext->m_RItemsVec[tmp->second]->m_World._42 -= syncoutline;
			AppContext->m_RItemsVec[tmp->second]->m_MaterialIndex = 0;
			AppContext->m_RItemsVec[tmp->second]->m_IsVisible = true;
		}
	}

	{	//승리자 포커싱
		if (g_pFramework->m_pNetwork->m_pGameInfo->m_WinnerID != -1) {
			auto tmppso = m_Users[g_pFramework->m_pNetwork->m_pGameInfo->m_WinnerID]->GetPosition();
			m_Users[m_PlayerID]->m_MyCamera->SetPosition(tmppso.x, tmppso.y + 150, tmppso.z - 300);
		}
	}
	MaterialReference::GetApp()->Update(fDeltaTime);

	int i = MathHelper::Rand(4, Core::mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, Core::mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	Core::mWaves->Disturb(i, j, r);


	if (IsFall[m_PlayerID]) {
		AppContext->m_RItemsVec[133 + Player_Lifecnt[m_PlayerID]]->m_MaterialIndex = 6;
		static float time = 0;
		time += fDeltaTime;
		GraphicsContext::GetApp()->OnBlurEffect(true);

		if (time >= 3) {
			time = 0;
		}
		else if (time < 0.03) {
			if (FallZ < 4) FallZ = 4;
			if (FallX < 4) FallX = 4;
			if (FallZ > 123) FallZ = 123;
			if (FallX > 123) FallX = 123;
			Core::mWaves->Disturb(FallZ, FallX, 4);
		}
	}
	// Update the wave simulation.
	Core::mWaves->Update(fDeltaTime);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["huskyBB"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icecubeBB"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["life"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lifeline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["igloo"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["fishrack"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["waterdrop"], AppContext->m_RItemsVec);

	//minimap
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["MinimapSea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapicecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapsnowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapicicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapsnow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["myicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowmanicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["iglooicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["enermyicon"], AppContext->m_RItemsVec);


	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);

	/*Characters*/
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["huskyOutline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["PenguinOutline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ArcticFoxOutline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["PolarBearOutline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["SealOutline"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Penguin, MeshReference::GetApp()->m_SkinnedModelInsts["Penguin"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Husky, MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Fox, MeshReference::GetApp()->m_SkinnedModelInsts["ArcticFox"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::PolarBear, MeshReference::GetApp()->m_SkinnedModelInsts["PolarBear"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Seal, MeshReference::GetApp()->m_SkinnedModelInsts["Seal"].get());


	/*UI*/	for (int j = 0; j < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++j)
	{
		for (int i = 0; i < 5; i++) {
			GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);
			GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);
		}
	}

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_" + m_Users[i]->GetType()], AppContext->m_RItemsVec);
		GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_" + m_Users[i]->GetType()], AppContext->m_RItemsVec);
		//이부분 
	}
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateUIPassCB(0.75f);

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["crushparticle"], AppContext->m_RItemsVec, true);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowParticle"], AppContext->m_RItemsVec, true);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["starParticle"], AppContext->m_RItemsVec, true);

	///*Shadow*/
	GraphicsContext::GetApp()->UpdateShadowTransform(CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL].get(), m_SceneBounds);
	GraphicsContext::GetApp()->UpdateShadowPassCB();


	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);

}

void GameplayScene::Render()
{
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_OpaquePSO.Get());
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	g_CommandList->RSSetViewports(1, &mMinimapViewport);
	g_CommandList->RSSetScissorRects(1, &mMinimapScissorRect);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["MinimapSea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Minimapicecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Minimapsnowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Minimapsnow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Minimapicicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["myicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowmanicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["iglooicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["enermyicon"], AppContext->m_RItemsVec);

	g_CommandList->RSSetViewports(1, &mScreenViewport);
	g_CommandList->RSSetScissorRects(1, &mScissorRect);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["life"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lifeline"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["igloo"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fishrack"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["waterdrop"], AppContext->m_RItemsVec);

	//GraphicsContext::GetApp()->SetPipelineState(Graphics::g_BB.Get());
	//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["huskyBB"], AppContext->m_RItemsVec);


	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	bool ty[5]{ false,false,false,false,false };
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
		if (m_Users[i]->GetHide()) {
			if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
				ty[0] = true;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
				ty[1] = true;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
				ty[2] = true;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
				ty[3] = true;
			}
			else {
				ty[4] = true;
			}
		}
		else {
			if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_PENGUIN) {
				ty[0] = false;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_ARCTICFOX) {
				ty[1] = false;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY) {
				ty[2] = false;
			}
			else if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_SEAL) {
				ty[3] = false;
			}
			else {
				ty[4] = false;
			}
		}
	}

	if (!ty[0])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	if (!g_pFramework->m_pNetwork->GetFoxSkill()) {
		const std::map<std::string, UINT>& info = AppContext->m_RItemsMap["ArcticFox"]->GetinstanceKeymap();
		AppContext->m_RItemsVec[info.begin()->second]->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ArcticFox"]->MatCBIndex;
	}
	if (!ty[1] && !(g_pFramework->m_pNetwork->GetFoxSkill() && g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) != CHARACTER_ARCTICFOX)) {
		if (g_pFramework->m_pNetwork->GetFoxSkill() && g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) == CHARACTER_ARCTICFOX) {
			const std::map<std::string, UINT>& info = AppContext->m_RItemsMap["ArcticFox"]->GetinstanceKeymap();
			AppContext->m_RItemsVec[info.begin()->second]->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["TranslucentArcticFox"]->MatCBIndex;
		}
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	}
	if (!ty[2]) {
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	}
	if (!ty[3])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);
	if (!ty[4])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_OutlinePSO.Get());
	if (!ty[0])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PenguinOutline"], AppContext->m_RItemsVec);
	if (!ty[1] && !(g_pFramework->m_pNetwork->GetFoxSkill() && g_pFramework->m_pNetwork->GetCharacterType(m_PlayerID) != CHARACTER_ARCTICFOX))
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFoxOutline"], AppContext->m_RItemsVec);
	if (!ty[2])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["huskyOutline"], AppContext->m_RItemsVec);
	if (!ty[3])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["SealOutline"], AppContext->m_RItemsVec);
	if (!ty[4])
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBearOutline"], AppContext->m_RItemsVec);

	//debug
#ifdef DEBUG_SHADOW
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_DebugPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["quad"], AppContext->m_RItemsVec);
#endif
	//sky
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	/*Particle*/
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_ParticlePSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["crushparticle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowParticle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["starParticle"], AppContext->m_RItemsVec);


	/* UI */
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_UIPSO.Get());
	for (int j = 0; j < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++j)
	{
		for (int i = 0; i < 5; i++) {
			GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);

		}
	}

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_" + m_Users[i]->GetType()], AppContext->m_RItemsVec);
	}

	/*GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_p"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_h"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_s"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_b"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_f"], AppContext->m_RItemsVec);*/

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);


}

void GameplayScene::RenderUI()
{
	// Timer
	GraphicsContext::GetApp()->SetTextSize(Core::g_DisplayWidth / 20);
	GraphicsContext::GetApp()->DrawD2DText(std::to_wstring(m_Timer / 60) + L" : " + std::to_wstring(m_Timer % 60), 0, -Core::g_DisplayHeight / 2.3, Core::g_DisplayWidth);

	// skill cool time
	//GraphicsContext::GetApp()->SetTextSize(Core::g_DisplayWidth / 20);
	//GraphicsContext::GetApp()->DrawD2DText(std::to_wstring(m_Timer % 60), -Core::g_DisplayWidth/2.9, Core::g_DisplayHeight / 2.3, Core::g_DisplayWidth);

}

void GameplayScene::WriteShadow()
{
	TEST();
}

void GameplayScene::TEST()
{

	/*Shadow*/
	GraphicsContext::GetApp()->SetResourceShadowPassCB();
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_ShadowOpaquePSO.Get());

	/*Shadow Props*/
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);


	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["igloo"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fishrack"], AppContext->m_RItemsVec);

	/*Shadow Characters*/

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedShadowOpaquePSO.Get());

	for (auto& p : m_Users)
	{
		if (!p.second) continue;

		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap[p.second->GetType()], AppContext->m_RItemsVec);
	}

	GraphicsContext::GetApp()->ShadowTransitionResourceBarrier();
}

float GameplayScene::distance(XMFLOAT3 a, XMFLOAT3 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

void GameplayScene::Fall(int num) {
	if (!IsFall[num]) {
		FallX = round(m_Users[m_PlayerID]->GetPosition().x * 119 / 1300 + 699.0 / 26.0);
		FallZ = round(m_Users[m_PlayerID]->GetPosition().z * -119 / 1300 + 2603.0 / 26.0);
		IsFall[num] = true;
	}
}