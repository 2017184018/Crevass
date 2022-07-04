#include "pch.h"
#include "GameplayScene.h"
#include "CREVASS.h"
#include "MeshReference.h"
#include "MaterialReference.h"
#include "CommandContext.h"
#include "ApplicationContext.h"
#include "InputHandler.h"
#include "SceneManager.h"

#include "GameObject.h"
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
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			AppContext->CreateUI2D("player_"+ std::to_string(j)+"hp" + std::to_string(i), "player_" + std::to_string(j) + "hp" + std::to_string(i), 19, -350.f + i*23.f, 180.f -j*30.f, 20.f, 10.5f);
		}
	}
	//AppContext->CreateUI2D("hp" , "hp", 19, 0.f, 0.f, 50.f, 30.5f);
	//AppContext->CreateUI2D("hp1", "hp1", 19, 100.f, 100.f, 50.f, 30.5f);
	AppContext->CreateUI2D("ui_p", "ui_p", 20, -380.f, 180.f, 30.f, 20.f);
	AppContext->CreateUI2D("ui_h", "ui_h", 21, -380.f, 150.f, 30.f, 20.f);
	AppContext->CreateUI2D("ui_s", "ui_s", 22, -380.f, 120.f, 30.f, 20.f);
	AppContext->CreateUI2D("ui_b", "ui_b", 23, -380.f, 90.f, 30.f, 20.f);
	AppContext->CreateUI2D("ui_f", "ui_f", 24, -380.f, 60.f, 30.f, 20.f);
	AppContext->CreateUI2D("ui_SkillOn", "ui_SkillOn", 25, -280.f, -260.f, 130.f, 40.f);
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

	m_PlayerID = g_pFramework->m_pNetwork->m_pGameInfo->m_ClientID;
	//나 
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		int ty = g_pFramework->m_pNetwork->GetCharacterType(i);
		if (ty == CHARACTER_PENGUIN) {
			m_Users[i] = AppContext->FindObject<Character>("Penguin_LOD0skin", "Penguin_LOD0skin0");
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

	m_Users[m_PlayerID]->SetCamera(CREVASS::GetApp()->m_Camera, CameraType::Third);
	m_Users[m_PlayerID]->SetController();

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		m_Users[i]->SetPosition(g_pFramework->m_pNetwork->GetPlayerPos(i));
		m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
	}
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
	for (int i = 0; i < 4; ++i) {
		float scale = 3.0f / 5.0f;
		SnowmanIndex[i] = g_pFramework->m_pNetwork->GetSnowmanLocation(i);
	}
	for (int i = 0; i < 25; ++i) {
		AppContext->m_RItemsVec[2 * i + 1]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[2 * (i + 1)]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[51 + i]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		DestructionCnt[i] = g_pFramework->m_pNetwork->GetBlockDestructionCnt(i);
	}
	return false;
}

void GameplayScene::Exit()
{
	/*AppContext->HiddenProps(m_MapName);
	AppContext->HiddenCharacter(CHARACTER_DRUID);*/
	AppContext->HiddenBlocks();
	//AppContext->HiddenCharacter("Husky");
	cout << "===========================================" << endl << endl;
}

void GameplayScene::Update(const float& fDeltaTime)
{

	m_Timer = 300 - g_pFramework->m_pNetwork->Gettime();

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
			if (g_pFramework->m_pNetwork->GetCharacterType(i) == CHARACTER_HUSKY && time >= 0.05f) {
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
		if (g_pFramework->m_pNetwork->GetPlayerAnim(i) == ANIM_FALL) {
			m_Users[i]->SetAnimationKeyState(Character::PlayerState::STATE_FALL);
			m_Users[i]->is_StartFallAnim = true;
		}
		else {
			m_Users[i]->is_StartFallAnim = false;
		}
	}

	m_Users[m_PlayerID]->m_PlayerController->SetSkillCool(g_pFramework->m_pNetwork->GetPlayerSkillCool(m_PlayerID));
	if (m_Users[m_PlayerID]->m_PlayerController->GetSkillCool()) {
		AppContext->FindObject<GameObject>("ui_SkillOn", "ui_SkillOn")->m_MaterialIndex = 26;
	}
	else if (!m_Users[m_PlayerID]->m_PlayerController->GetSkillCool()) {
		AppContext->FindObject<GameObject>("ui_SkillOn", "ui_SkillOn")->m_MaterialIndex = 25;
	}
	for (int i = 0; i < 25; ++i) {
		AppContext->m_RItemsVec[2 * i + 1]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[2 * (i + 1)]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[51 + i]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		DestructionCnt[i] = g_pFramework->m_pNetwork->GetBlockDestructionCnt(i);
		if (DestructionCnt[i] == 0) {
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._11 = 1;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._22 = 1;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._33 = 1;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._11 = 1;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._22 = 1;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._33 = 1;
			AppContext->m_RItemsVec[i + 51]->m_World._11 = 7.5 / 10.0;
			AppContext->m_RItemsVec[i + 51]->m_World._22 = 1;
			AppContext->m_RItemsVec[i + 51]->m_World._33 = 7.5 / 10.0;
		}
		if (DestructionCnt[i] == 1) {
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._11 = 0;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._22 = 0;
			AppContext->m_RItemsVec[2 * (i + 1)]->m_World._33 = 0;
		}
		else if (DestructionCnt[i] == 2) {
			AppContext->m_RItemsVec[2 * i + 1]->m_World._11 = 0;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._22 = 0;
			AppContext->m_RItemsVec[2 * i + 1]->m_World._33 = 0;
		}
		else if (DestructionCnt[i] == 3) {
			AppContext->m_RItemsVec[i + 51]->m_World._11 = 0;
			AppContext->m_RItemsVec[i + 51]->m_World._22 = 0;
			AppContext->m_RItemsVec[i + 51]->m_World._33 = 0;
		}
		AppContext->m_RItemsVec[2 * i + 1]->m_World._41 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._41 = AppContext->m_RItemsVec[51 + i]->m_World._41;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._42 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._42 = AppContext->m_RItemsVec[51 + i]->m_World._42;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._43 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._43 = AppContext->m_RItemsVec[51 + i]->m_World._43;

	}

	{		//우박 hail
		for (int i = 0; i < 5; ++i) {
			AppContext->m_RItemsVec[213 + i]->SetPosition(g_pFramework->m_pNetwork->GetHailPos(i));

		}
	}

	for (auto& p : m_Users)
	{
		if (!p.second) continue;

		p.second->Update(fDeltaTime);
	}

	// SceneBounds Update
	//m_SceneBounds.Center = { m_Users[m_PlayerID]->GetPosition().x, 0 , m_Users[m_PlayerID]->GetPosition().z };


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
		if (g_pFramework->m_pNetwork->GetCharacterReset(i) == true)
		{
			//m_Users[m_PlayerID]->is_fall = false;
			//cout << "reset" << endl;
			g_pFramework->m_pNetwork->SetCharacterReset(i);
			g_pFramework->m_pNetwork->SetCharacterFall(i);
			if (i == m_PlayerID)
			{
				m_Users[i]->m_PlayerController->SetIsFall();
				GraphicsContext::GetApp()->OnBlurEffect(false);

				IsFall[m_PlayerID] = false;
				if (Lifecnt > 0) {
					--Lifecnt;
					if (Lifecnt == 0) {
						//	SceneManager::GetApp()->EnterScene(SceneType::GameResult);
							//서버에 패배 전송
						g_pFramework->m_pNetwork->Send(CS_PLAYER_LOSE);
					}
				}
			}
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
			/*static int cnt = 0;
			if (direct) {
				if (cnt < 3) {
					if (rota < 1 / 9.0) {
						rota += 0.003;
					}
					else {
						direct = false;
					}
				}
			}
			else {
				if (rota > -1/9.0) {
					rota -= 0.003;
				}
				else {
					direct = true;
				}
			}*/
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

	for (int i = 0; i < 5; ++i) {	//life
		auto CameraPOS = m_Users[m_PlayerID]->m_MyCamera->GetPosition();

		AppContext->m_RItemsVec[139 + i]->m_World._41 = AppContext->m_RItemsVec[134 + i]->m_World._41 = XMVectorGetX(CameraPOS) - 45 + 7.7 * i;
		AppContext->m_RItemsVec[139 + i]->m_World._42 = AppContext->m_RItemsVec[134 + i]->m_World._42 = XMVectorGetY(CameraPOS) + 14;
		AppContext->m_RItemsVec[139 + i]->m_World._42 += 9.f;
		AppContext->m_RItemsVec[139 + i]->m_World._43 = AppContext->m_RItemsVec[134 + i]->m_World._43 = XMVectorGetZ(CameraPOS) + 100;
		AppContext->m_RItemsVec[139 + i]->m_World._43 += 0.02;
	}
	{	//waterdrop
		for (int i = 0; i < 4; ++i) {
			AppContext->m_RItemsVec[218 + i]->m_World._41 = m_Users[m_PlayerID]->GetPosition().x + (100 * (i / 2) - 50);
			AppContext->m_RItemsVec[218 + i]->m_World._42 = m_Users[m_PlayerID]->GetPosition().y + 330;
			if (i == 0 || i == 3)
				AppContext->m_RItemsVec[218 + i]->m_World._43 = m_Users[m_PlayerID]->GetPosition().z - 85;
			else
				AppContext->m_RItemsVec[218 + i]->m_World._43 = m_Users[m_PlayerID]->GetPosition().z + 25;
		}
	}

	{
		//huskyafterimage
		const std::map<std::string, UINT>& info = AppContext->m_RItemsMap["husky"]->GetinstanceKeymap();
		auto i = info.begin();

		++i;
		AppContext->m_RItemsVec[(i)->second]->m_World = huskyimagerota[1];
		AppContext->m_RItemsVec[(i)->second]->m_World._41 = huskyimagepos[1].x;
		AppContext->m_RItemsVec[(i)->second]->m_World._42 = huskyimagepos[1].y;
		AppContext->m_RItemsVec[(i)->second]->m_World._43 = huskyimagepos[1].z;
		AppContext->m_RItemsVec[(i)->second]->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["huskyimage1"]->MatCBIndex;
		++i;
		AppContext->m_RItemsVec[(i)->second]->m_World = huskyimagerota[2];
		AppContext->m_RItemsVec[(i)->second]->m_World._41 = huskyimagepos[2].x;
		AppContext->m_RItemsVec[(i)->second]->m_World._42 = huskyimagepos[2].y;
		AppContext->m_RItemsVec[(i)->second]->m_World._43 = huskyimagepos[2].z;
		AppContext->m_RItemsVec[(i)->second]->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["huskyimage2"]->MatCBIndex;
		++i;
		AppContext->m_RItemsVec[(i)->second]->m_World = huskyimagerota[3];
		AppContext->m_RItemsVec[(i)->second]->m_World._41 = huskyimagepos[3].x;
		AppContext->m_RItemsVec[(i)->second]->m_World._42 = huskyimagepos[3].y;
		AppContext->m_RItemsVec[(i)->second]->m_World._43 = huskyimagepos[3].z;
		AppContext->m_RItemsVec[(i)->second]->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["huskyimage3"]->MatCBIndex;


	}
	MaterialReference::GetApp()->Update(fDeltaTime);

	int i = MathHelper::Rand(4, Core::mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, Core::mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	Core::mWaves->Disturb(i, j, r);


	if (IsFall[m_PlayerID]) {
		AppContext->m_RItemsVec[133 + Lifecnt]->m_MaterialIndex = 6;
		static float time = 0;
		static int tmpidx = -1;
		time += fDeltaTime;
		GraphicsContext::GetApp()->OnBlurEffect(true);

		if (time >= 3) {
			time = 0;
			//IsFall[m_PlayerID] = false;
			//BlurCnt = 0;
			//if (Lifecnt > 0) {
			//	--Lifecnt;
			//	if (Lifecnt == 0) {
			//		//	SceneManager::GetApp()->EnterScene(SceneType::GameResult);
			//			//서버에 패배 전송
			//	}
			//}
			tmpidx = -1;
		}
		else if (time >= 2.9) {
			//			m_Users[m_PlayerID]->is_fall = false;
			//			m_Users[m_PlayerID]->SetPosition(tmpidx / 3 * 400, 200, tmpidx % 3 * 400);
				//		Gravity = 0.01;
		}
		else if (time < 0.03) {
			if (FallZ < 4) FallZ = 4;
			if (FallX < 4) FallX = 4;
			if (FallZ > 123) FallZ = 123;
			if (FallX > 123) FallX = 123;
			tmpidx = uid(dre);
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


	/*Characters*/
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Penguin_LOD0skin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Penguin, MeshReference::GetApp()->m_SkinnedModelInsts["Penguin_LOD0skin"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Husky, MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Fox, MeshReference::GetApp()->m_SkinnedModelInsts["ArcticFox"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::PolarBear, MeshReference::GetApp()->m_SkinnedModelInsts["PolarBear"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Seal, MeshReference::GetApp()->m_SkinnedModelInsts["Seal"].get());
	
	
	/*UI*/
	/*GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["hp"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["hp"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["hp1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["hp1"], AppContext->m_RItemsVec);*/
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);
			GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);
		}
	}
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_p"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_p"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_h"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_h"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_s"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_s"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_b"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_b"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_f"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_f"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateUIPassCB(0.75f);

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);

	//여기가 문제일수도 있음 

	///*Shadow*/
	GraphicsContext::GetApp()->UpdateShadowTransform(CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL].get(), m_SceneBounds);
	GraphicsContext::GetApp()->UpdateShadowPassCB();


	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);


}

void GameplayScene::Render()
{
	//g_CommandList->RSSetViewports(1, &mMinimapViewport);
	//g_CommandList->RSSetScissorRects(1, &mMinimapScissorRect);
	//g_CommandList->RSSetViewports(1, &mScreenViewport);
	//g_CommandList->RSSetScissorRects(1, &mScissorRect);

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);

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

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

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
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin_LOD0skin"], AppContext->m_RItemsVec);
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

	//debug
#ifdef DEBUG_SHADOW
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_DebugPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["quad"], AppContext->m_RItemsVec);
#endif

	/* UI */
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_UIPSO.Get());
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["player_" + std::to_string(j) + "hp" + std::to_string(i)], AppContext->m_RItemsVec);

		}
	}
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_p"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_h"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_s"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_b"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_f"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ui_SkillOn"], AppContext->m_RItemsVec);
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

void GameplayScene::RenderUI()
{
	// Timer
	GraphicsContext::GetApp()->SetTextSize(Core::g_DisplayWidth / 20);
	GraphicsContext::GetApp()->DrawD2DText(std::to_wstring(m_Timer / 60) + L" : " + std::to_wstring(m_Timer % 60), 0, -Core::g_DisplayHeight / 2.3, Core::g_DisplayWidth);

	// skill cool time
	//GraphicsContext::GetApp()->SetTextSize(Core::g_DisplayWidth / 20);
	//GraphicsContext::GetApp()->DrawD2DText(std::to_wstring(m_Timer % 60), -Core::g_DisplayWidth/2.9, Core::g_DisplayHeight / 2.3, Core::g_DisplayWidth);

}

void GameplayScene::Fall(int num) {
	if (!IsFall[num]) {
		FallX = round(m_Users[m_PlayerID]->GetPosition().x * 119 / 1300 + 699.0 / 26.0);
		FallZ = round(m_Users[m_PlayerID]->GetPosition().z * -119 / 1300 + 2603.0 / 26.0);
		IsFall[num] = true;
	}
}