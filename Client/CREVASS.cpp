#include "pch.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "InputHandler.h"

#include "MeshReference.h"
#include "MaterialReference.h"
#include "Character.h"
#include "CharacterParts.h"

#include "Network.h"
#include "MainFramework.h"

#include "../protocol.h"

#include <random>
using namespace Core;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,8 }; //눈사람 위치
uniform_int_distribution<> uid2{ 0,3 }; //블록 덮개 회전
uniform_int_distribution<> uid3{ 0,24 }; //블록 선택
#define SCALE 0.5

void CREVASS::Startup(void)
{

	m_Camera = new Camera;
	m_Camera->SetPosition(45.0f * 4, 45.0f * 2, -45.0f * 3);
	m_Camera->SetLens(0.25f * MathHelper::Pi, static_cast<float>(g_DisplayWidth) / g_DisplayHeight, 1.0f, 1000.0f);

	// Build Mesh & Material & Texture
	m_MeshRef = new MeshReference;
	m_MaterialRef = new MaterialReference;

	//Penguin
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/ice_cube_2.mesh", "icecube");    //fbx
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_top.mesh", "snow_top");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowman.mesh", "snowman");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/icicle_1.mesh", "icicle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_cube.mesh", "snowcube");

	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());
	//m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "icecube", m_MeshRef->m_GeometryMesh["icecube"].get()->DrawArgs["icecube"].Bounds);

	//animation
	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "Penguin_LOD0skin");
	//m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "Penguin_LOD0skin", m_MeshRef->m_GeometryMesh["Penguin_LOD0skin"].get()->DrawArgs["Penguin_LOD0skin"].Bounds);
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Walk");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Peck");
	mWaves = std::make_unique<Waves>(128, 128, 1.0f, 0.03f, 4.0f, 0.2f);

	m_MeshRef->BuildWaves(g_Device.Get(), g_CommandList.Get(), mWaves.get());

	m_MaterialRef->BuildMaterials();

	// Build RenderItem
	BuildScene();


	GraphicsContext::GetApp()->VertexCount = mWaves->VertexCount();
	GraphicsContext::GetApp()->passCount = 1;
	GraphicsContext::GetApp()->skinnedObjectCount = TOTAL_USER_COUNT;
	GraphicsContext::GetApp()->materialCount = m_MaterialRef->m_Materials.size();

	for (auto& p : m_RItemsMap)
	{
		GraphicsContext::GetApp()->BuildInstanceBuffer(p.second);
	}

	m_PlayerID = 0;
	m_Users[m_PlayerID] = FindObject<Character>("Penguin_LOD0skin", "Penguin_LOD0skin0");

	//// Player type, id 등등 세팅
	m_Users[m_PlayerID]->SetCamera(m_Camera, CameraType::Third);
	m_Users[m_PlayerID]->SetController();

	for (int i = 0; i < 25; ++i) {
		IsShake[i] = false;
		IsRight[i] = true;
		ShakeCnt[i] = 0;
		DestructionCnt[i] = 0;
		IsDown[i] = true;
	}
}

void CREVASS::Cleanup(void)
{
	for (auto& p : m_AllRItems)
		SAFE_DELETE_PTR(p);
	m_AllRItems.clear();

	SAFE_DELETE_PTR(m_MeshRef);
	SAFE_DELETE_PTR(m_MaterialRef);

	for (auto& p : m_RItemsMap)
	{
		SAFE_DELETE_PTR(p.second);
	}

	for (auto& p : m_RItemsVec)
		SAFE_DELETE_PTR(p);
	m_RItemsVec.clear();
	m_RItemsMap.clear();

	/* Release References */
	SAFE_DELETE_PTR(m_MeshRef);
	SAFE_DELETE_PTR(m_MaterialRef);
}

bool CREVASS::BlockCheck(int idx) {
	if (idx == 0 || idx == 2 || idx == 4 || idx == 10 || idx == 12 || idx == 14 || idx == 20 || idx == 22 || idx == 24)
		return false;
	return true;
}

BOOL B = true;

void CREVASS::Update(float deltaT)
{
	//이거 풀면 플레이어 3인칭 기준 카메라 적용
	float speed = 100 * deltaT;
	if (m_Users[m_PlayerID]) {
		if (m_Users[m_PlayerID]->bJump == true && B == true) {
			m_Users[m_PlayerID]->Move(DIR_UP, speed * 2, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y > 70) {
			B = false;

		}

		if (m_Users[m_PlayerID]->GetPosition().y > 30) {
			m_Users[m_PlayerID]->Move(DIR_DOWN, speed, true);
		}

		if (m_Users[m_PlayerID]->GetPosition().y <= 30 && m_Users[m_PlayerID]->bJump == true) {
			m_Users[m_PlayerID]->bJump = false;
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
		}

		m_Users[m_PlayerID]->Update(deltaT);
	}

	OnKeyboardInput(deltaT);
	for (int i = 0; i < 25; ++i) {
		if (IsShake[i] || !IsDown[i]) {
			shake(m_RItemsVec[2 * i + 1], i);	//블록
			shake(m_RItemsVec[2 * (i + 1)], i);	//덮개
			shake(m_RItemsVec[51 + i], i);	//고드름
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++DestructionCnt[i];
			if (DestructionCnt[i] == 1) {
				m_RItemsVec[2 * (i + 1)]->m_World._11 = 0;
				m_RItemsVec[2 * (i + 1)]->m_World._22 = 0;
				m_RItemsVec[2 * (i + 1)]->m_World._33 = 0;
			}
			else if (DestructionCnt[i] == 2) {
				m_RItemsVec[2 * i + 1]->m_World._11 = 0;
				m_RItemsVec[2 * i + 1]->m_World._22 = 0;
				m_RItemsVec[2 * i + 1]->m_World._33 = 0;
			}
			else if (DestructionCnt[i] == 3) {
				m_RItemsVec[i + 51]->m_World._11 = 0;
				m_RItemsVec[i + 51]->m_World._22 = 0;
				m_RItemsVec[i + 51]->m_World._33 = 0;
			}
		}
		//블록 위치조정
		m_RItemsVec[2 * i + 1]->m_World._41 = m_RItemsVec[2 * (i + 1)]->m_World._41 = m_RItemsVec[51 + i]->m_World._41;
		m_RItemsVec[2 * i + 1]->m_World._42 = m_RItemsVec[2 * (i + 1)]->m_World._42 = m_RItemsVec[51 + i]->m_World._42;
		m_RItemsVec[2 * i + 1]->m_World._43 = m_RItemsVec[2 * (i + 1)]->m_World._43 = m_RItemsVec[51 + i]->m_World._43;

		//눈사람 위치조정
		if (SnowmanIndex[0] % 4) {
			m_RItemsVec[76]->m_World._41 = m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 - 15;
		}
		else {
			m_RItemsVec[76]->m_World._41 = m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._41 + 15;
		}
		m_RItemsVec[76]->m_World._42 = m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._42 + 10;
		m_RItemsVec[76]->m_World._43 = m_RItemsVec[2 * SnowmanIndex[0] + 1]->m_World._43 + 15;
		if (SnowmanIndex[1] % 4) {
			m_RItemsVec[77]->m_World._41 = m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 - 15;
		}
		else {
			m_RItemsVec[77]->m_World._41 = m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._41 + 15;
		}
		m_RItemsVec[77]->m_World._42 = m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._42 + 10;
		m_RItemsVec[77]->m_World._43 = m_RItemsVec[2 * SnowmanIndex[1] + 1]->m_World._43 + 15;

	}

	m_MaterialRef->Update(deltaT);

	int i = MathHelper::Rand(4, mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	mWaves->Disturb(i, j, r);
	if (IsFall) {
		static float time = 0;
		time += deltaT;
		BlurCnt = 3;
		if (time >= 4) {
			time = 0;
			IsFall = false;
			BlurCnt = 0;
		}
		else if (time < 0.03) {
			mWaves->Disturb(50, 57, 2);
		}
	}


	// Update the wave simulation.
	mWaves->Update(deltaT);

	//Map = object info
	//Vec = game object
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icecube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snowman"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icicle"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Sea"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["sky"], m_RItemsVec);

	/*GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icecubeBB"], m_RItemsVec);*/

	GraphicsContext::GetApp()->UpdateMaterialBuffer(m_MaterialRef->m_Materials);
	m_Camera->UpdateViewMatrix();
	GraphicsContext::GetApp()->UpdateMainPassCB(*m_Camera);

	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Penguin_LOD0skin"], m_RItemsVec);
	/*GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Penguin_LOD0skinBB"], m_RItemsVec);*/
	GraphicsContext::GetApp()->UpdateSkinnedCBs(CHARACTER_INDEX_MASTER, m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skin"].get());
	GraphicsContext::GetApp()->UpdateWave(mWaves.get(), wave);

	//FindObject<GameObject>("Penguin_LOD0skinBB", "Penguin_LOD0skin0BB")->SetPosition(FindObject<GameObject>("Penguin_LOD0skin", "Penguin_LOD0skin0")->GetPosition());

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (BlockCheck(5 * i + j)) {
				if (FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					cout << 5 * i + j << "하고 충돌" << endl;

				}
			}
			else {
				if (FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j))->m_Bounds.Intersects(m_Users[m_PlayerID]->m_Bounds)) {
					cout << 5 * i + j << "하고 충돌" << endl;

				}
			}
		}
	}

	//cout << " x = " << m_Users[m_PlayerID]->m_Bounds.Center.x<< " y = " << m_Users[m_PlayerID]->m_Bounds.Center.y << "z = " << m_Users[m_PlayerID]->m_Bounds.Center.z << endl;
}

void CREVASS::RenderScene(void)
{

	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icecube"], m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snowman"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icicle"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Sea"], m_RItemsVec);

	//디버그 주석
	//GraphicsContext::GetApp()->SetPipelineState(Graphics::g_BB.Get());
	//GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icecubeBB"], m_RItemsVec);
	//GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Penguin_LOD0skinBB"], m_RItemsVec);
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["sky"], m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Penguin_LOD0skin"], m_RItemsVec);

	mBlurFilter->Execute(g_CommandList.Get(), mPostProcessRootSignature.Get(),
		Graphics::HorBlur.Get(), Graphics::VerBlur.Get(), BackBuffer, BlurCnt);

	// Prepare to copy blurred output to the back buffer.
	g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(BackBuffer,
		D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST));

	g_CommandList->CopyResource(BackBuffer, mBlurFilter->Output());

	// Transition to PRESENT state.
	g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(BackBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT));
}

void CREVASS::shake(GameObject* object, int index) {
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

void CREVASS::OnKeyboardInput(const float deltaT)
{
	static bool pushone = false;
	if (GetAsyncKeyState('1') & 0x8000) {
		if (pushone) {
			int tmp = uid3(dre);
			IsShake[tmp] = true;
			IsDown[tmp] = true;
			pushone = false;
		}
	}
	else {
		pushone = true;
	}

	static bool pushtwo = false;
	if (GetAsyncKeyState('2') & 0x8000) {
		if (pushtwo) {
			pushtwo = false;
			IsFall = true;
		}
	}
	else {
		pushtwo = true;
	}

	float speed = 100 * deltaT;

	if (GetAsyncKeyState('G') & 0x8000) {
		m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skin"]->ClipName = "Peck";
	}


	if (GetAsyncKeyState('W') & 0x8000) {
		m_Users[m_PlayerID]->Move(DIR_FORWARD, speed, true);
		if (!m_Users[m_PlayerID]->bJump) {
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		}
		m_Users[m_PlayerID]->SetDir(0);
	}


	if (GetAsyncKeyState('A') & 0x8000) {
		m_Users[m_PlayerID]->Move(DIR_LEFT, speed, true);
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		m_Users[m_PlayerID]->SetDir(270);
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		m_Users[m_PlayerID]->Move(DIR_BACKWARD, speed, true);
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		m_Users[m_PlayerID]->SetDir(180);
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		m_Users[m_PlayerID]->Move(DIR_RIGHT, speed, true);
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		m_Users[m_PlayerID]->SetDir(90);
	}

	if (GetAsyncKeyState('F') & 0x8000 && m_Users[m_PlayerID]->bJump == false) {
		m_Users[m_PlayerID]->bJump = true;
		B = true;
		m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_JUMP;

	}

	if (GetAsyncKeyState('W') & 0x8000 && GetAsyncKeyState('D') & 0x8000) {
		if (!m_Users[m_PlayerID]->bJump) {

			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		}
		m_Users[m_PlayerID]->SetDir(45);
	}

	if (GetAsyncKeyState('S') & 0x8000 && GetAsyncKeyState('D') & 0x8000) {
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		m_Users[m_PlayerID]->SetDir(135);
	}

	if (GetAsyncKeyState('S') & 0x8000 && GetAsyncKeyState('A') & 0x8000) {
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;
		m_Users[m_PlayerID]->SetDir(225);
	}

	if (GetAsyncKeyState('A') & 0x8000 && GetAsyncKeyState('W') & 0x8000) {
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_FORWARD;

		m_Users[m_PlayerID]->SetDir(315);
	}

	if (InputHandler::IsKeyUp('W'))
	{
		if (!m_Users[m_PlayerID]->bJump) {

			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
		}
	}
	if (InputHandler::IsKeyUp('A'))
	{
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
	}
	if (InputHandler::IsKeyUp('S'))
	{
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
	}
	if (InputHandler::IsKeyUp('D'))
	{
		if (!m_Users[m_PlayerID]->bJump)
			m_Users[m_PlayerID]->m_KeyState = Character::PlayerState::STATE_IDLE;
	}

	if (InputHandler::IsKeyUp('B'))
	{
		g_pFramework->m_pNetwork->Send(CS_READY);
	}


}

void CREVASS::BuildScene()
{
	//0: Skybox,  1~50: 홀수는 블록, 짝수는 덮개, 51~75: 고드름, 76~77: 눈사람, 78: 바다, 79:펭귄

	//layer ,mesh type , id		
	GameObject* skyRitem = CreateObject<GameObject>(RenderLayer::ID_SKY, "sky", "sky0");		//0
	skyRitem->Geo = m_MeshRef->m_GeometryMesh["geo"].get();
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->m_MaterialIndex = 0;

	skyRitem->m_World = MathHelper::Identity4x4();
	skyRitem->m_TexTransform = MathHelper::Identity4x4();

	int distance = SCALE * 200;

	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "icecube", "icecube" + std::to_string(5 * i + j));
				instancingObj->Geo = m_MeshRef->m_GeometryMesh["icecube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["icecube"].Bounds;

				instancingObj->m_MaterialIndex = 1;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;
				
				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._43 = distance * j;
				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["icecube"].Bounds.Center, instancingObj->GetPosition());

			}
			else {
				instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "snowcube", "snowcube" + std::to_string(5 * i + j));
				instancingObj->Geo = m_MeshRef->m_GeometryMesh["snowcube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowcube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowcube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowcube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["snowcube"].Bounds;

				instancingObj->m_MaterialIndex = 1;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;
				
				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._43 = distance * j;
				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["snowcube"].Bounds.Center, instancingObj->GetPosition());

			}

			

			GameObject* top = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "snow_top", "snow_top" + std::to_string(5 * i + j));
			top->Geo = m_MeshRef->m_GeometryMesh["snow_top"].get();
			top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			top->IndexCount = top->Geo->DrawArgs["snow_top"].IndexCount;
			top->StartIndexLocation = top->Geo->DrawArgs["snow_top"].StartIndexLocation;
			top->BaseVertexLocation = top->Geo->DrawArgs["snow_top"].BaseVertexLocation;
			top->m_MaterialIndex = 1;
			top->m_World = MathHelper::Identity4x4();
			top->m_World._11 = SCALE;
			top->m_World._22 = SCALE;
			top->m_World._33 = SCALE;
			XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationY(3.141592 * uid2(dre)));
			top->m_World._41 = distance * i;
			top->m_World._42 = 0;
			top->m_World._43 = distance * j;
			top->m_TexTransform = MathHelper::Identity4x4();
		}
	}



	for (int i = 0; i < 5; ++i) {	//51~75
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "icicle", "icicle" + std::to_string(5 * i + j));
			instancingObj->Geo = m_MeshRef->m_GeometryMesh["icicle"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icicle"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icicle"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icicle"].BaseVertexLocation;
			instancingObj->m_MaterialIndex = 1;
			instancingObj->m_World = MathHelper::Identity4x4();
			instancingObj->m_World._11 = 0.5;
			instancingObj->m_World._22 = 0.4;
			instancingObj->m_World._33 = 0.5;
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._42 = 0;
			instancingObj->m_World._43 = distance * j;
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}

	int RandomLocation[2] = { -1,-1 };
	for (int i = 0; i < 2; ++i) {		//76, 77
		GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "snowman", "snowman" + std::to_string(i));
		instancingObj->Geo = m_MeshRef->m_GeometryMesh["snowman"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowman"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowman"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowman"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = 1;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = SCALE - 0.2f;
		instancingObj->m_World._22 = SCALE - 0.2f;
		instancingObj->m_World._33 = SCALE - 0.2f;
		RandomLocation[i] = uid(dre);
		while (i == 1 && RandomLocation[0] == RandomLocation[1]) {
			RandomLocation[i] = uid(dre);
		}
		SnowmanIndex[i] = SnowmanLocaArray[RandomLocation[i]];
		if (SnowmanIndex[i] % 4) {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 5 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = SnowmanIndex[i] / 5 * distance - 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = SnowmanIndex[i] % 5 * distance + 15.0f;
		}
		else {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 7 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = SnowmanIndex[i] / 5 * distance + 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = SnowmanIndex[i] % 5 * distance; +15.0f;
		}
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	{		//78
		GameObject* Sea = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "Sea", "Sea0");
		Sea->Geo = m_MeshRef->m_GeometryMesh["wave"].get();
		Sea->IndexCount = Sea->Geo->DrawArgs["wave"].IndexCount;
		Sea->StartIndexLocation = Sea->Geo->DrawArgs["wave"].StartIndexLocation;
		Sea->BaseVertexLocation = Sea->Geo->DrawArgs["wave"].BaseVertexLocation;
		Sea->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Sea->m_MaterialIndex = 3;

		Sea->m_World = MathHelper::Identity4x4();

		Sea->m_World._11 = 7;
		Sea->m_World._33 = 7;

		Sea->m_World._41 = SCALE * 400;
		Sea->m_World._42 = -SCALE * 100;
		Sea->m_World._43 = SCALE * 400;

		Sea->m_TexTransform = MathHelper::Identity4x4();
		wave = Sea;
	}

	{//79
		/*Characters*/
		// student
		Character* character1 = CreateObject<Character>(RenderLayer::ID_SkinnedOpaque, "Penguin_LOD0skin", "Penguin_LOD0skin0");
		character1->m_TexTransform = MathHelper::Identity4x4();
		character1->m_MaterialIndex = 2;
		character1->Geo = m_MeshRef->m_GeometryMesh["Penguin_LOD0skin"].get();
		character1->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		character1->IndexCount = character1->Geo->DrawArgs["Penguin_LOD0skin"].IndexCount;
		character1->StartIndexLocation = character1->Geo->DrawArgs["Penguin_LOD0skin"].StartIndexLocation;
		character1->BaseVertexLocation = character1->Geo->DrawArgs["Penguin_LOD0skin"].BaseVertexLocation;
		character1->m_SkinnedCBIndex = CHARACTER_INDEX_MASTER;
		character1->m_SkinnedModelInst = m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skin"].get();
		character1->m_Bounds = character1->Geo->DrawArgs["Penguin_LOD0skin"].Bounds;

		character1->Scale(20, 20, 20);
		character1->SetPosition(250, 30, 0);
		//디버그
	/*	Character* character1BB = CreateObject<Character>(RenderLayer::ID_SkinnedOpaque, "Penguin_LOD0skinBB", "Penguin_LOD0skin0BB");
		character1BB->m_TexTransform = MathHelper::Identity4x4();
		character1BB->m_MaterialIndex = 2;
		character1BB->Geo = m_MeshRef->m_GeometryMesh["Penguin_LOD0skinBB"].get();
		character1BB->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		character1BB->IndexCount = character1BB->Geo->DrawArgs["Penguin_LOD0skinBB"].IndexCount;
		character1BB->StartIndexLocation = character1BB->Geo->DrawArgs["Penguin_LOD0skinBB"].StartIndexLocation;
		character1BB->BaseVertexLocation = character1BB->Geo->DrawArgs["Penguin_LOD0skinBB"].BaseVertexLocation;
		character1BB->m_SkinnedCBIndex = CHARACTER_INDEX_MASTER;
		character1BB->m_SkinnedModelInst = m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skinBB"].get();
		character1BB->m_Bounds = character1BB->Geo->DrawArgs["Penguin_LOD0skinBB"].Bounds;*/


	}
}