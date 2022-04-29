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
//#include "Network.h"
//#include "MainFramework.h"
#include <random>

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,8 }; //눈사람 위치
uniform_int_distribution<> uid2{ 0,3 }; //블록 덮개 회전
uniform_int_distribution<> uid3{ 0,24 }; //블록 선택

using namespace Core;

void GameplayScene::Initialize()
{
	m_SceneController = new GameplayController(this);

	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreateBlocks();
	AppContext->CreateSnowmans();
	AppContext->CreateWave();
	AppContext->CreateBackground();

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

	m_PlayerID = 0;

	m_Users[m_PlayerID] = AppContext->FindObject<Character>("husky", "husky0");

	//m_Users[m_PlayerID]->m_PlayerRole = ROLE_MASTER;
	//m_Users[1] = AppContext->FindObject<Character>(CHARACTER_DRUID, CHARACTER_DRUID);
	//m_Users[2] = AppContext->FindObject<Character>(CHARACTER_BAIRD, CHARACTER_BAIRD);
	//m_Users[3] = AppContext->FindObject<Character>(CHARACTER_SORCERER, CHARACTER_SORCERER);

	m_Users[m_PlayerID]->SetCamera(CREVASS::GetApp()->m_Camera, CameraType::Third);
	m_Users[m_PlayerID]->SetController();

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
	if (m_Users[m_PlayerID])
		m_Users[m_PlayerID]->Update(fDeltaTime);
	float speed = 200 * fDeltaTime;
	if (m_Users[m_PlayerID]) {
		if (m_Users[m_PlayerID]->bJump == true && (m_Users[m_PlayerID]->is_Inair == true)) {
			m_Users[m_PlayerID]->Move(DIR_UP, speed * 2, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y > 70) {
			m_Users[m_PlayerID]->is_Inair = false;

		}

		if (m_Users[m_PlayerID]->GetPosition().y > 30) {
			m_Users[m_PlayerID]->Move(DIR_DOWN, speed, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y <= 30 && m_Users[m_PlayerID]->bJump == true) {
			m_Users[m_PlayerID]->bJump = false;
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
		}

		m_Users[m_PlayerID]->Update(fDeltaTime);
	}
	//OnKeyboardInput(deltaT);
	for (int i = 0; i < 25; ++i) {
		if (IsShake[i] || !IsDown[i]) {
			shake(AppContext->m_RItemsVec[2 * i + 1], i);	//블록
			shake(AppContext->m_RItemsVec[2 * (i + 1)], i);	//덮개
			shake(AppContext->m_RItemsVec[51 + i], i);	//고드름
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
		//블록 위치조정
		AppContext->m_RItemsVec[2 * i + 1]->m_World._41 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._41 = AppContext->m_RItemsVec[51 + i]->m_World._41;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._42 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._42 = AppContext->m_RItemsVec[51 + i]->m_World._42;
		AppContext->m_RItemsVec[2 * i + 1]->m_World._43 = AppContext->m_RItemsVec[2 * (i + 1)]->m_World._43 = AppContext->m_RItemsVec[51 + i]->m_World._43;
	}
	//눈사람 위치조정
	if (SnowmanIndex[0] % 4) {
		AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 - 15;
	}
	else {
		AppContext->m_RItemsVec[76]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 + 15;
	}
	AppContext->m_RItemsVec[76]->m_World._42 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._42 + 10;
	AppContext->m_RItemsVec[76]->m_World._43 = AppContext->m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._43 + 15;

	if (SnowmanIndex[1] % 4) {
		AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 - 15;
	}
	else {
		AppContext->m_RItemsVec[77]->m_World._41 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 + 15;
	}

	AppContext->m_RItemsVec[77]->m_World._42 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._42 + 10;
	AppContext->m_RItemsVec[77]->m_World._43 = AppContext->m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._43 + 15;

	static bool pushone = false;
	if (GetAsyncKeyState('1') & 0x8000) {
		if (!pushone) {
			LoseLife();
			pushone = true;
		}
	}
	else {
		pushone = false;
	}

	MaterialReference::GetApp()->Update(fDeltaTime);

	int i = MathHelper::Rand(4, Core::mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, Core::mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	Core::mWaves->Disturb(i, j, r);
	if (IsFall) {
		static float time = 0;
		time += fDeltaTime;
		BlurCnt = 3;
		if (time >= 4) {
			time = 0;
			IsFall = false;
			BlurCnt = 0;
		}
		else if (time < 0.03) {
			Core::mWaves->Disturb(50, 57, 2);
		}
	}


	// Update the wave simulation.
	Core::mWaves->Update(fDeltaTime);

	//블록
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["life"], AppContext->m_RItemsVec);

	//meterial

	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);

	/*Characters*/
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateSkinnedCBs(CHARACTER_INDEX_MASTER, MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get());
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);

	//FindObject<GameObject>("huskyBB", "husky0BB")->SetPosition(FindObject<GameObject>("husky", "husky0")->GetPosition());

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (BlockCheck(5 * i + j)) {
				if (tmp==-1 && AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					tmp = 5 * i + j;
					if (!BlockIn) {
						IsShake[5 * i + j] = true;
						IsDown[5 * i + j] = true;
						BlockIn = true;
					}
				}
				 if (tmp!=-1 && !AppContext->FindObject<GameObject>("icecube", "icecube" + std::to_string(tmp))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					BlockIn = false;
					tmp = -1;
				}
			}
			else {
				if (AppContext->FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					IsShake[5 * i + j] = true;
					IsDown[5 * i + j] = true;
					cout << 5 * i + j << endl;
				}
			}
		}
	}
}

void GameplayScene::Render()
{
	cout << "안녕" << endl;
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["life"], AppContext->m_RItemsVec);
	//디버그 주석
	//GraphicsContext::GetApp()->SetPipelineState(Graphics::g_BB.Get());
	//GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icecubeBB"], m_RItemsVec);
	//GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["huskyBB"], m_RItemsVec);
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);

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
	if (BlockCheck(index)) {		//부숴짐
		if (IsRight[index]) {
			if (object->m_World._41 < 100 * (index / 5) + 3) {
				object->m_World._41 += 0.14f;
			}
			else
				IsRight[index] = false;
		}
		else {
			if (object->m_World._41 > 100 * (index / 5) - 3) {
				object->m_World._41 -= 0.14f;
			}
			else
				IsRight[index] = true;
		}
		if (IsRight[index] && (object->m_World._41 - 0.001f <= 100 * (index / 5) && object->m_World._41 + 0.001f >= 100 * (index / 5)))
			++ShakeCnt[index];
	}
	else {		//내려감
		if (IsDown[index]) {
			if (object->m_World._42 <= -70)
				IsDown[index] = false;
			else {
				object->m_World._42 -= (-object->m_World._42 / 150 + 1.0 / 30.0);
			}
		}
		else {
			if (object->m_World._42 <= 0) {
				object->m_World._42 += 0.07f;
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

void GameplayScene::LoseLife() {
	if (Lifecnt > 0) {
		AppContext->m_RItemsVec[133 + Lifecnt--]->m_MaterialIndex = 6;
	}
}