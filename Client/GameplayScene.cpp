#include "pch.h"
#include "GameplayScene.h"
#include "CREVASS.h"
#include "MeshReference.h"
#include "MaterialReference.h"
#include "CommandContext.h"
#include "ApplicationContext.h"
#include "InputHandler.h"


#include "GameObject.h"
#include "Character.h"
#include "CharacterParts.h"
#include "Network.h"
#include "MainFramework.h"
#include <random>

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,8 }; //����� ��ġ
uniform_int_distribution<> uid2{ 0,3 }; //���� ���� ȸ��
uniform_int_distribution<> uid3{ 0,24 }; //���� ����

using namespace Core;

void GameplayScene::Initialize()
{
	m_SceneController = new GameplayController(this);

	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreateBlocks();
	AppContext->CreateSnowmans();
	AppContext->CreateWave();
	AppContext->CreateBackground();
	//AppContext->CreateDebugBoundingBox("huskyBB", "huskyBB0");
	//AppContext->CreateDebugBoundingBox("icecubeBB", "icecubeBB0");
	for (int i = 0; i < 25; ++i) {
		IsShake[i] = false;
		IsRight[i] = true;
		ShakeCnt[i] = 0;
		DestructionCnt[i] = 0;
		IsDown[i] = true;
	}
}

bool GameplayScene::Enter()
{
	cout << "GamePlay Scene" << endl;

	m_PlayerID = g_pFramework->m_pNetwork->m_pGameInfo->m_ClientID;

	//나 
	m_Users[0] = AppContext->FindObject<Character>("husky", "husky0");
	m_Users[0]->m_IsVisible = true;

	m_Users[1] = AppContext->FindObject<Character>("Penguin_LOD0skin", "Penguin_LOD0skin0");
	m_Users[1]->m_IsVisible = true;

	m_Users[2] = AppContext->FindObject<Character>("ArcticFox", "ArcticFox0");
	m_Users[2]->m_IsVisible = true;

	m_Users[3] = AppContext->FindObject<Character>("PolarBear", "PolarBear0");
	m_Users[3]->m_IsVisible = true;

	m_Users[4] = AppContext->FindObject<Character>("Seal", "Seal0");
	m_Users[4]->m_IsVisible = true;

	m_Users[m_PlayerID]->SetCamera(CREVASS::GetApp()->m_Camera, CameraType::Third);
	m_Users[m_PlayerID]->SetController();

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		m_Users[i]->SetPosition(g_pFramework->m_pNetwork->m_pGameInfo->m_PlayersInfo[i]->GetPosition());
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
	}
	//cout <<"tkdlwm ==" << m_Users.size() << endl;
	for (auto& p : m_Users)
	{
		if (!p.second) continue;

		p.second->Update(fDeltaTime);
	}

	static float gra = 0.1;


	float speed = 100 * fDeltaTime;
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
			gra = 0.1;
			m_Users[m_PlayerID]->Move(DIR_UP, speed * 2, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y > HighY) {
			m_Users[m_PlayerID]->is_Inair = false;
		}

		if (!m_Users[m_PlayerID]->is_Inair)
			gra += 0.05;

		if (tmp != -1) {
			YSave = 30;
		}

		if (m_Users[m_PlayerID]->GetPosition().y <= YSave && m_Users[m_PlayerID]->bJump == true) {
			m_Users[m_PlayerID]->bJump = false;
			YSave = 30;
			IsFirst = true;
			HighY = YSave + 40;
			gra = 0.1;
		}

	}

	for (int i = 0; i < 25; ++i) {
		if (IsShake[i] || !IsDown[i]) {
			shake(AppContext->m_RItemsVec[2 * i + 1], i);	//����
			shake(AppContext->m_RItemsVec[2 * (i + 1)], i);	//����
			shake(AppContext->m_RItemsVec[51 + i], i);	//���帧
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++DestructionCnt[i];
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
		}
		//���� ��ġ����
		AppContext->m_RItemsVec[2 * i + 1]->m_World._41 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._41 = AppContext->m_RItemsVec[51 + i]->m_World._41;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._42 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._42 = AppContext->m_RItemsVec[51 + i]->m_World._42;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._43 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._43 = AppContext->m_RItemsVec[51 + i]->m_World._43;
	}
	//����� ��ġ����
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

	for (int i = 0; i < 5; ++i) {
		auto CameraPOS = m_Users[m_PlayerID]->m_MyCamera->GetPosition();

		AppContext->m_RItemsVec[139 + i]->m_World._41 = AppContext->m_RItemsVec[134 + i]->m_World._41 = XMVectorGetX(CameraPOS) - 45 + 7.7 * i;
		if (!IsFall || i < Lifecnt - 1) {
			AppContext->m_RItemsVec[139 + i]->m_World._42 = AppContext->m_RItemsVec[134 + i]->m_World._42 = XMVectorGetY(CameraPOS) + 14;
			AppContext->m_RItemsVec[139 + i]->m_World._42 += 7.f;
		}
		AppContext->m_RItemsVec[139 + i]->m_World._43 = AppContext->m_RItemsVec[134 + i]->m_World._43 = XMVectorGetZ(CameraPOS) + 100;
		AppContext->m_RItemsVec[139 + i]->m_World._43 += 0.01;
	}

	MaterialReference::GetApp()->Update(fDeltaTime);

	int i = MathHelper::Rand(4, Core::mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, Core::mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	Core::mWaves->Disturb(i, j, r);
	if (IsFall) {
		static bool Isup = true;
		if (Isup && AppContext->m_RItemsVec[133 + Lifecnt]->m_World._42 <= XMVectorGetY(m_Users[m_PlayerID]->m_MyCamera->GetPosition()) + 14 + 15) {
			AppContext->m_RItemsVec[133 + Lifecnt]->m_World._42 += 0.1f;
			AppContext->m_RItemsVec[133 + Lifecnt + 5]->m_World._42 += 0.1f;
			if (AppContext->m_RItemsVec[133 + Lifecnt]->m_World._42 >= XMVectorGetY(m_Users[m_PlayerID]->m_MyCamera->GetPosition()) + 14 + 15) {
				AppContext->m_RItemsVec[133 + Lifecnt]->m_MaterialIndex = 6;
				Isup = false;
			}
		}
		else if (AppContext->m_RItemsVec[133 + Lifecnt]->m_World._42 > XMVectorGetY(m_Users[m_PlayerID]->m_MyCamera->GetPosition()) + 14) {
			AppContext->m_RItemsVec[133 + Lifecnt]->m_World._42 -= 0.1f;
			AppContext->m_RItemsVec[133 + Lifecnt + 5]->m_World._42 -= 0.1f;
		}
		static float time = 0;
		time += fDeltaTime;
		BlurCnt = 3;
		if (time >= 3) {
			time = 0;
			IsFall = false;
			BlurCnt = 0;
			if (Lifecnt > 0) {
				--Lifecnt;
				Isup = true;
			}
		}
		else if (time < 0.03) {
			Core::mWaves->Disturb(FallZ, FallX, 4);
		}
	}

	// Update the wave simulation.
	Core::mWaves->Update(fDeltaTime);

	//����
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

	//meterial

	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);

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

	//cout << m_Users[m_PlayerID]->GetPosition().x << ", " << m_Users[m_PlayerID]->GetPosition().z << endl;

	//AppContext->FindObject<GameObject>("huskyBB", "huskyBB0")->SetPosition(AppContext->FindObject<GameObject>("husky", "husky0")->GetPosition());
	//AppContext->FindObject<GameObject>("icecubeBB", "icecubeBB0")->SetPosition(AppContext->FindObject<GameObject>("snowcube", "snowcube0")->GetPosition());
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (BlockCheck(5 * i + j)) {
				if (tmp == -1 && AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					tmp = 5 * i + j;
					if (!BlockIn) {
						IsShake[5 * i + j] = true;
						IsDown[5 * i + j] = true;
						BlockIn = true;
					}
				}
				if (tmp != -1 && !AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(tmp))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					BlockIn = false;
					tmp = -1;
				}
			}
			else {
				if (tmp2 == -1 && AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					tmp2 = 5 * i + j;
					IsShake[5 * i + j] = true;
					IsDown[5 * i + j] = true;
				}
				if (tmp2 != -1 && !AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(tmp2))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					IsDown[tmp2] = false;
					tmp2 = -1;
				}
			}
		}
	}

	speed += gra;
	if (tmp == -1 && tmp2 == -1) {
		m_Users[m_PlayerID]->Move(DIR_DOWN, speed, true);
	}
	else {
		if (tmp != -1) {
			if (m_Users[m_PlayerID]->GetPosition().y > 10 && m_Users[m_PlayerID]->bJump == false && DestructionCnt[tmp] != 3) {
				m_Users[m_PlayerID]->SetPosition(m_Users[m_PlayerID]->GetPosition().x,
					AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(tmp))->GetPosition().y + 60, m_Users[m_PlayerID]->GetPosition().z);
			}
			else {
				m_Users[m_PlayerID]->Move(DIR_DOWN, speed, true);
			}
		}
		else {
			if (m_Users[m_PlayerID]->GetPosition().y - AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(tmp2))->GetPosition().y >= 50 &&
				m_Users[m_PlayerID]->bJump == false) {
				m_Users[m_PlayerID]->SetPosition(m_Users[m_PlayerID]->GetPosition().x,
					AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(tmp2))->GetPosition().y + 60, m_Users[m_PlayerID]->GetPosition().z);
			}
			else {
				m_Users[m_PlayerID]->Move(DIR_DOWN, speed, true);
			}
		}
		//	g_pFramework->m_pNetwork->Send(CS_PLAYER_UP_UP);
	}

	if (m_Users[m_PlayerID]->GetPosition().y <= -100) {
		Fall();
		m_Users[m_PlayerID]->Move(DIR_UP, speed, true);
	}
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
	//����� �ּ�
	//GraphicsContext::GetApp()->SetPipelineState(Graphics::g_BB.Get());
//	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecubeBB"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["huskyBB"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin_LOD0skin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);


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

void GameplayScene::shake(GameObject* object, int index) {
	if (BlockCheck(index)) {		//�ν���
		if (IsRight[index]) {
			if (object->m_World._41 < 200 * (index / 5) + 5) {
				object->m_World._41 += 0.18f;
			}
			else
				IsRight[index] = false;
		}
		else {
			if (object->m_World._41 > 200 * (index / 5) - 5) {
				object->m_World._41 -= 0.18f;
			}
			else
				IsRight[index] = true;
		}
		if (IsRight[index] && (object->m_World._41 - 0.001f <= 200 * (index / 5) && object->m_World._41 + 0.001f >= 200 * (index / 5)))
			++ShakeCnt[index];
	}
	else {
		if (IsDown[index] && tmp2 != -1) {
			if (object->m_World._42 <= -170)
				IsDown[index] = false;
			else {
				object->m_World._42 -= (-object->m_World._42 / 140.0 - 4.0 / 35.0);
			}
		}
		else {
			if (object->m_World._42 <= -30 && (tmp2 == -1 || m_Users[m_PlayerID]->bJump)) {
				object->m_World._42 += 0.2f;
			}
			else {
				IsDown[index] = true;
			}
			IsShake[index] = false;
		}
	}
}

bool GameplayScene::BlockCheck(int idx) {
	if (idx == 0 || idx == 2 || idx == 4 || idx == 10 || idx == 12 || idx == 14 || idx == 20 || idx == 22 || idx == 24)
		return false;
	return true;
}

void GameplayScene::Fall() {
	if (!IsFall) {
		FallX = round(m_Users[m_PlayerID]->GetPosition().x * 119 / 1300 + 699.0/ 26.0);
		FallZ = round(m_Users[m_PlayerID]->GetPosition().z * -119 / 1300 + 2603.0/ 26.0);
		IsFall = true;
	}
}