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
	AppContext->CreateSnowmans();
	AppContext->CreateWave();
	AppContext->CreateBackground();
	AppContext->CreateDebugBoundingBox("huskyBB", "huskyBB0");
	//AppContext->CreateDebugBoundingBox("icecubeBB", "icecubeBB0");
	for (int i = 0; i < 25; ++i) {
		//IsShake[i] = false;
		//IsRight[i] = true;
		//ShakeCnt[i] = 0;
		DestructionCnt[i] = 0;
		//IsDown[i] = true;
	}
}

bool GameplayScene::Enter()
{
	cout << "GamePlay Scene" << endl;

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
		SnowmanIndex[i] = SnowmanLocaArray[g_pFramework->m_pNetwork->GetSnowmanLocation(i)];
		if (SnowmanIndex[i] % 4) {		//방향 오류, 가끔 게임 스타트 패킷을 2번 받는 듯
			XMStoreFloat4x4(&AppContext->m_RItemsVec[76 + i]->m_World, XMMatrixScaling(3.0f / 5.0f, 3.0f / 5.0f, 3.0f / 5.0f) * XMMatrixRotationY(3.14 * 5 / 6));
		}
		else {
			XMStoreFloat4x4(&AppContext->m_RItemsVec[76 + i]->m_World, XMMatrixScaling(3.0f / 5.0f, 3.0f / 5.0f, 3.0f / 5.0f) * XMMatrixRotationY(3.14 * 7 / 6));
		}
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
	m_SceneController->Update(fDeltaTime);
	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{

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

	for (int i = 0; i < 25; ++i) {
		AppContext->m_RItemsVec[2 * i + 1]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[2 * (i + 1)]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		AppContext->m_RItemsVec[51 + i]->SetPosition(g_pFramework->m_pNetwork->GetBlockPos(i));
		DestructionCnt[i] = g_pFramework->m_pNetwork->GetBlockDestructionCnt(i);
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

	//cout <<"tkdlwm ==" << m_Users.size() << endl;
	for (auto& p : m_Users)
	{
		if (!p.second) continue;

		p.second->Update(fDeltaTime);
	}

	/*float speed = 100 * fDeltaTime;
	if (m_Users[m_PlayerID]) {
		static float YSave = 30;
		static bool IsFirst = true;
		static float HighY = YSave + 40;
		if (m_Users[m_PlayerID]->bJump == true && (m_Users[m_PlayerID]->is_Inair == true)) {
			if (IsFirst) {
				if (m_Users[m_PlayerID]->GetPosition().y > 30)
					YSave = 30;
				else
					YSave = m_Users[m_PlayerID]->GetPosition().y;
				IsFirst = false;
				HighY = YSave + 40;
			}
			Gravity = 0.1;
			m_Users[m_PlayerID]->Move(DIR_UP, speed * 2, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y > HighY) {
			m_Users[m_PlayerID]->is_Inair = false;
		}

		if (!m_Users[m_PlayerID]->is_Inair)
			Gravity += 0.05;

		if (tmp1 != -1) {
			YSave = 30;
		}

		if (m_Users[m_PlayerID]->GetPosition().y <= YSave && m_Users[m_PlayerID]->bJump == true) {
			m_Users[m_PlayerID]->bJump = false;
			YSave = 30;
			IsFirst = true;
			HighY = YSave + 40;
			Gravity = 0.1;
		}
	}*/

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		if (g_pFramework->m_pNetwork->GetCharacterFall(i) == true)
		{
			if (i == m_PlayerID)
			{
				m_Users[i]->m_PlayerController->Fall();
				Fall(i);
				//cout << "fall" << endl;
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
				BlurCnt = 0;
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
		//syncro snowman
		if (SnowmanIndex[0] % 4) {
			AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 - 20;
		}
		else {
			AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 + 20;
		}
		AppContext->m_RItemsVec[76]->m_World._42 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._42 + 20;
		AppContext->m_RItemsVec[76]->m_World._43 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._43 + 20;

		if (SnowmanIndex[1] % 4) {
			AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 - 20;
		}
		else {
			AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 + 20;
		}

		AppContext->m_RItemsVec[77]->m_World._42 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._42 + 20;
		AppContext->m_RItemsVec[77]->m_World._43 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._43 + 20;
	}

	for (int i = 0; i < 5; ++i) {	//life
		auto CameraPOS = m_Users[m_PlayerID]->m_MyCamera->GetPosition();

		AppContext->m_RItemsVec[139 + i]->m_World._41 = AppContext->m_RItemsVec[134 + i]->m_World._41 = XMVectorGetX(CameraPOS) - 45 + 7.7 * i;
		AppContext->m_RItemsVec[139 + i]->m_World._42 = AppContext->m_RItemsVec[134 + i]->m_World._42 = XMVectorGetY(CameraPOS) + 14;
		AppContext->m_RItemsVec[139 + i]->m_World._42 += 9.f;
		AppContext->m_RItemsVec[139 + i]->m_World._43 = AppContext->m_RItemsVec[134 + i]->m_World._43 = XMVectorGetZ(CameraPOS) + 100;
		AppContext->m_RItemsVec[139 + i]->m_World._43 += 0.02;
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
		BlurCnt = 3;

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
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["huskyBB"], AppContext->m_RItemsVec);
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

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);

	//여기가 문제일수도 있음 

	///*Shadow*/
	GraphicsContext::GetApp()->UpdateShadowTransform();
	GraphicsContext::GetApp()->UpdateShadowPassCB();


	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);


}

void GameplayScene::Render()
{
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
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
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_BB.Get());
	//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecubeBB"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["huskyBB"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin_LOD0skin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);



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

	/*Shadow Characters*/

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedShadowOpaquePSO.Get());

	for (auto& p : m_Users)
	{
		if (!p.second) continue;

			//GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
		
		GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap[p.second->GetType()], AppContext->m_RItemsVec);
	}

	GraphicsContext::GetApp()->ShadowTransitionResourceBarrier();

	mBlurFilter->Execute(g_CommandList.Get(), mPostProcessRootSignature.Get(),
		Graphics::HorBlur.Get(), Graphics::VerBlur.Get(), BackBuffer, BlurCnt);

	//Prepare to copy blurred output to the back buffer.
	g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(BackBuffer,
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST));

	g_CommandList->CopyResource(BackBuffer, mBlurFilter->Output());

	// Transition to PRESENT state.
	g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(BackBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT));
}

void GameplayScene::Fall(int num) {
	if (!IsFall[num]) {
		FallX = round(m_Users[m_PlayerID]->GetPosition().x * 119 / 1300 + 699.0 / 26.0);
		FallZ = round(m_Users[m_PlayerID]->GetPosition().z * -119 / 1300 + 2603.0 / 26.0);
		IsFall[num] = true;
	}
}