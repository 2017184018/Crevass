#include "pch.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"

#include "MeshReference.h"
#include "MaterialReference.h"
#include "Character.h"
#include "CharacterParts.h"

#include <random>
using namespace Core;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,24 }; //눈사람 위치
uniform_int_distribution<> uid2{ 0,3 }; //블록 덮개 회전

#define SCALE 0.5

void CREVASS::Startup(void)
{
	m_Camera.SetPosition(45.0f * 4, 45.0f * 2, -45.0f * 3);
	m_Camera.SetLens(0.25f * MathHelper::Pi, static_cast<float>(g_DisplayWidth) / g_DisplayHeight, 1.0f, 1000.0f);

	// Build Mesh & Material & Texture
	m_MeshRef = new MeshReference;
	m_MaterialRef = new MaterialReference;

	//Penguin
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/ice_cube_2.mesh", "icecube");    //fbx
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_top.mesh", "snow_top");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowman.mesh", "snowman");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/icicle_1.mesh", "icicle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/Penguin.mesh", "Penguin");

	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());
	
	//animation
	//m_MeshRef->BuildGeometry(g_Device.Get(), g_CommandList.Get(), "./Models/soldier.m3d", "Models\\soldier.m3d");
	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "Penguin_LOD0skin");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Idle");


	m_MaterialRef->BuildMaterials();


	// Build RenderItem
	BuildScene();
	
	GraphicsContext::GetApp()->passCount = 1;
	GraphicsContext::GetApp()->skinnedObjectCount = TOTAL_USER_COUNT;
	GraphicsContext::GetApp()->materialCount = m_MaterialRef->m_Materials.size();

	for (auto& p : m_RItemsMap)
	{
		GraphicsContext::GetApp()->BuildInstanceBuffer(p.second);
	}

	for (int i = 0; i < 25; ++i) {
		IsShake[i] = false;
		IsRight[i] = true;
		ShakeCnt[i] = 0;
		DestructionCnt[i] = 0;
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

void CREVASS::Update(float deltaT)
{
	OnKeyboardInput(deltaT);
	for (int i = 0; i < 25; ++i) {
		if (IsShake[i]) {
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
	}

	//Map = object info
	//Vec = game object
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icecube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snowman"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icicle"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Penguin"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["sky"], m_RItemsVec);

	GraphicsContext::GetApp()->UpdateMaterialBuffer(m_MaterialRef->m_Materials);
	GraphicsContext::GetApp()->UpdateMainPassCB(m_Camera);

	/*Characters*/
	//for (int i = 0; i < m_MeshRef->mSkinnedMats.size(); ++i) {
	//	std::string submeshName = "sm_" + std::to_string(i);
	//	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Models\\soldier.m3d" + submeshName], m_RItemsVec);
	//}
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["Penguin_LOD0skin"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateSkinnedCBs(CHARACTER_INDEX_MASTER, m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skin"].get());
	
}

void CREVASS::RenderScene(void)
{

	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icecube"], m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snowman"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icicle"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Penguin"], m_RItemsVec);
	
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["sky"], m_RItemsVec);

	/*test Animation*/
	//for (int i = 0; i < m_MeshRef->mSkinnedMats.size(); ++i) {
	//	std::string submeshName = "sm_" + std::to_string(i);
	//	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Models\\soldier.m3d" + submeshName],m_RItemsVec);
	//}

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Penguin_LOD0skin"], m_RItemsVec);

}

void CREVASS::shake(GameObject* object, int index) {
	if (IsRight[index]) {
		if (object->m_World._41 < 100 * (index / 5) + 3) {
			object->m_World._41 += 0.07f;
		}
		else
			IsRight[index] = false;
	}
	else {
		if (object->m_World._41 > 100 * (index / 5) - 3) {
			object->m_World._41 -= 0.07f;
		}
		else
			IsRight[index] = true;
	}
	if (IsRight[index] && (object->m_World._41 - 0.001f <= 100 * (index / 5) && object->m_World._41 + 0.001f >= 100 * (index / 5)))
		++ShakeCnt[index];
	
}

void CREVASS::OnKeyboardInput(const float deltaT)
{
	static bool pushone = false;
	if (GetAsyncKeyState('1') & 0x8000) {
		if (pushone) {
			IsShake[uid(dre)] = true;
			pushone = false;
		}
	}
	else {
		pushone = true;
	}

	if (GetAsyncKeyState('W') & 0x8000)
		m_Camera.Walk(20.0f * deltaT);

	if (GetAsyncKeyState('S') & 0x8000)
		m_Camera.Walk(-20.0f * deltaT);

	if (GetAsyncKeyState('A') & 0x8000)
		m_Camera.Strafe(-20.0f * deltaT);

	if (GetAsyncKeyState('D') & 0x8000)
		m_Camera.Strafe(20.0f * deltaT);

	if (GetAsyncKeyState('Z') & 0x8000) {
		auto tmp = m_Camera.GetPosition3f();
		tmp.y += 20.0f * deltaT;
		m_Camera.SetPosition(tmp);
	}

	if (GetAsyncKeyState('X') & 0x8000) {
		auto tmp = m_Camera.GetPosition3f();
		tmp.y -= 20.0f * deltaT;
		m_Camera.SetPosition(tmp);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
		m_Camera.RotateY(0.005);

	if (GetAsyncKeyState('E') & 0x8000)
		m_Camera.RotateY(-0.005);

	if (GetAsyncKeyState('R') & 0x8000)
		m_Camera.RotateX(0.005);

	if (GetAsyncKeyState('T') & 0x8000)
		m_Camera.RotateX(-0.005);

	m_Camera.UpdateViewMatrix();
}

void CREVASS::BuildScene()
{
	//0: Skybox,  1~50: 홀수는 블록, 짝수는 덮개, 51~75: 고드름, 76~77: 눈사람, 78: 펭귄

	//layer ,mesh type , id 
	GameObject* skyRitem = CreateObject<GameObject>(RenderLayer::ID_SKY, "sky", "sky0");
	skyRitem->Geo = m_MeshRef->m_GeometryMesh["geo"].get();
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->m_MaterialIndex = 0;

	skyRitem->m_World = MathHelper::Identity4x4();
	skyRitem->m_TexTransform = MathHelper::Identity4x4();

	// Instaincing Obj
	/*GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "icecube", "icebue0");
	instancingObj->Geo = m_MeshRef->m_GeometryMesh["icecube"].get();
	instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
	instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
	instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;

	instancingObj->m_MaterialIndex = 1;*/

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "icecube", "icebue" + std::to_string(5 * i + j));
			instancingObj->Geo = m_MeshRef->m_GeometryMesh["icecube"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;
			instancingObj->m_MaterialIndex = 1;
			instancingObj->m_World = MathHelper::Identity4x4();
			instancingObj->m_World._11 = SCALE;
			instancingObj->m_World._22 = SCALE;
			instancingObj->m_World._33 = SCALE;
			int distance = SCALE * 200;
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._43 = distance * j;
			instancingObj->m_TexTransform = MathHelper::Identity4x4();

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

	for (int i = 0; i < 5; ++i) {
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
			int distance = SCALE * 200;
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._42 = 20;
			instancingObj->m_World._43 = distance * j;
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}

	int RandomLocation[2] = { -1,-1 };
	for (int i = 0; i < 2; ++i) {
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
		if (RandomLocation[i] % 2) {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 5 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = RandomLocation[i] % 5 * distance - 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = RandomLocation[i] / 5 * distance + 15.0f;
		}
		else {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 7 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = RandomLocation[i] % 5 * distance + 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = RandomLocation[i] / 5 * distance; +15.0f;
		}
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	{
		GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "Penguin", "Penguin");
		instancingObj->Geo = m_MeshRef->m_GeometryMesh["Penguin"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["Penguin"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["Penguin"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["Penguin"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = 1;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = 15;
		instancingObj->m_World._22 = 15;
		instancingObj->m_World._33 = 15;

		XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World)* XMMatrixRotationY(3.14 ));
		XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World)* XMMatrixRotationX(3.14/2));

		instancingObj->m_World._41 = 200;
		instancingObj->m_World._42 = 30;
		instancingObj->m_World._43 = 80;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	/*Characters*/
	// student
	Character* character1 = CreateObject<Character>(RenderLayer::ID_SkinnedOpaque, "Penguin_LOD0skin", "Penguin_LOD0skin0");
	character1->m_TexTransform = MathHelper::Identity4x4();
	character1->m_MaterialIndex = 1;
	character1->Geo = m_MeshRef->m_GeometryMesh["Penguin_LOD0skin"].get();
	character1->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	character1->IndexCount = character1->Geo->DrawArgs["Penguin_LOD0skin"].IndexCount;
	character1->StartIndexLocation = character1->Geo->DrawArgs["Penguin_LOD0skin"].StartIndexLocation;
	character1->BaseVertexLocation = character1->Geo->DrawArgs["Penguin_LOD0skin"].BaseVertexLocation;
	character1->m_SkinnedCBIndex = CHARACTER_INDEX_MASTER;
	character1->m_SkinnedModelInst = m_MeshRef->m_SkinnedModelInsts["Penguin_LOD0skin"].get();


	//character1->Scale(100, 100, 100);
	character1->Scale(60, 60, 60);
	character1->Rotate(-90.0f, 180.0f, 0);
	character1->SetPosition(250, 20, 0);
	
	// Parts 생성
	//for (UINT i = 0; i < m_MeshRef->mSkinnedMats.size(); ++i)
	//{
	//	std::string submeshName = "sm_" + std::to_string(i);

	//	// TypeKey(GeoKey)는 동일해야하고 Instance Key는 달라져야한다.
	//	CharacterParts* cParts = CreateObject<CharacterParts>(RenderLayer::ID_SkinnedOpaque, "Penguin_LOD0skin","Penguin_LOD0skin0" );
	//	cParts->m_PartsName = "Models\\soldier.m3d" + submeshName;
	//	cParts->m_World._11 *= 1.5f;
	//	cParts->m_World._22 *= 1.5f;
	//	cParts->m_World._33 *= -1.5f;
 //       cParts->m_TexTransform = MathHelper::Identity4x4();
	//	cParts->m_MaterialIndex = 1;
	//	cParts->Geo = m_MeshRef->m_GeometryMesh["Models\\soldier.m3d"].get();
	//	cParts->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//	cParts->IndexCount = cParts->Geo->DrawArgs[submeshName].IndexCount;
	//	cParts->StartIndexLocation = cParts->Geo->DrawArgs[submeshName].StartIndexLocation;
	//	cParts->BaseVertexLocation = cParts->Geo->DrawArgs[submeshName].BaseVertexLocation;

	//	// All render items for this solider.m3d instance share
	//	// the same skinned model instance.
	//	// 이 파라미터들은 CharacterParts 클래스로 이동될 계획
	//	cParts->m_SkinnedModelInst = m_MeshRef->m_SkinnedModelInsts["Models\\soldier.m3d"].get();
	//	cParts->m_SkinnedCBIndex = CHARACTER_INDEX_MASTER;
	//	character1->SetParts(cParts);

	//}

	// SetPosition
	//character1->Scale(2.5f, 2.5f, 2.5f);
	
	//character1->SetPosition(250,0,0);
	/*m_Player->Rotate({0.f,1.f,0.f},180.f);
	m_Player->SetPosition(0.f, 0.f, 0.f);
	character1->SetPosition(originMap->playerVector[0].position.x, originMap->playerVector[0].position.y, originMap->playerVector[0].position.z);*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}
