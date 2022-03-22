#include "pch.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "Waves.h"

#include "MeshReference.h"
#include "MaterialReference.h"

#include <random>
using namespace Core;

random_device rd;
default_random_engine dre(rd());
uniform_int_distribution<> uid{ 0,24 }; //´«»ç¶÷ À§Ä¡
uniform_int_distribution<> uid2{ 0,3 }; //ºí·Ï µ¤°³ È¸Àü

#define SCALE 0.5

void CREVASS::Startup(void)
{
	m_Camera.SetPosition(45.0f * 4, 45.0f * 2, -45.0f * 3);
	m_Camera.SetLens(0.25f * MathHelper::Pi, static_cast<float>(g_DisplayWidth) / g_DisplayHeight, 1.0f, 1000.0f);

	// Build Mesh & Material & Texture
	m_MeshRef = new MeshReference;
	m_MaterialRef = new MaterialReference;
	//Penguin
	m_MeshRef->BuildSkullGeometry(g_Device.Get(), g_CommandList.Get());
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/ice_cube_2.mesh", "icecube");    //fbx
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_top.mesh", "snow_top");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowman.mesh", "snowman");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/icicle_1.mesh", "icicle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/Penguin.mesh", "Penguin");

	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());

	m_MaterialRef->BuildMaterials();

	// Build RenderItem
	BuildScene();

	mWaves = std::make_unique<Waves>(128, 128, 1.0f, 0.03f, 4.0f, 0.2f);

	GraphicsContext::GetApp()->passCount = 1;
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
			shake(m_RItemsVec[2 * i + 1], i);	//ºí·Ï
			shake(m_RItemsVec[2 * (i + 1)], i);	//µ¤°³
			shake(m_RItemsVec[51 + i], i);	//°íµå¸§
		}
		if (ShakeCnt[i] == 3) {
			ShakeCnt[i] = 0;
			IsShake[i] = false;
			++DestructionCnt[i];
			if (DestructionCnt[i] == 1) {
				m_RItemsVec[2 * (i + 1)]->World._11 = 0;
				m_RItemsVec[2 * (i + 1)]->World._22 = 0;
				m_RItemsVec[2 * (i + 1)]->World._33 = 0;
			}
			else if (DestructionCnt[i] == 2) {
				m_RItemsVec[2 * i + 1]->World._11 = 0;
				m_RItemsVec[2 * i + 1]->World._22 = 0;
				m_RItemsVec[2 * i + 1]->World._33 = 0;
			}
			else if (DestructionCnt[i] == 3) {
				m_RItemsVec[i + 51]->World._11 = 0;
				m_RItemsVec[i + 51]->World._22 = 0;
				m_RItemsVec[i + 51]->World._33 = 0;
			}
		}
	}


	m_MaterialRef->Update(deltaT);


	//Map = object info
	//Vec = game object
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap, m_RItemsVec);
	GraphicsContext::GetApp()->UpdateMaterialBuffer(m_MaterialRef->m_Materials);
	GraphicsContext::GetApp()->UpdateMainPassCB(m_Camera);


}

void CREVASS::RenderScene(void)
{
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icecube"], m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snowman"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["icicle"], m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Penguin"], m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["Sea"], m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["sky"], m_RItemsVec);
}

void CREVASS::shake(GameObject* object, int index) {
	if (IsRight[index]) {
		if (object->World._41 < 100 * (index / 5) + 3) {
			object->World._41 += 0.07f;
		}
		else
			IsRight[index] = false;
	}
	else {
		if (object->World._41 > 100 * (index / 5) - 3) {
			object->World._41 -= 0.07f;
		}
		else
			IsRight[index] = true;
	}
	if (IsRight[index] && (object->World._41 - 0.001f <= 100 * (index / 5) && object->World._41 + 0.001f >= 100 * (index / 5)))
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

	m_Camera.UpdateViewMatrix();
}

void CREVASS::BuildScene()
{
	//0: Skybox,  1~50: È¦¼ö´Â ºí·Ï, Â¦¼ö´Â µ¤°³, 51~75: °íµå¸§, 76~77: ´«»ç¶÷, 78: Æë±Ï, 79: ¹Ù´Ù

	//layer ,mesh type , id 
	GameObject* skyRitem = CreateObject<GameObject>(RenderLayer::ID_SKY, "sky", "sky0");
	skyRitem->Geo = m_MeshRef->m_GeometryMesh["geo"].get();
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->m_MaterialIndex = 0;

	skyRitem->World = MathHelper::Identity4x4();
	skyRitem->TexTransform = MathHelper::Identity4x4();

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "icecube", "icebue" + std::to_string(5 * i + j));
			instancingObj->Geo = m_MeshRef->m_GeometryMesh["icecube"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;
			instancingObj->m_MaterialIndex = 1;
			instancingObj->World = MathHelper::Identity4x4();
			instancingObj->World._11 = SCALE;
			instancingObj->World._22 = SCALE;
			instancingObj->World._33 = SCALE;
			int distance = SCALE * 200;
			instancingObj->World._41 = distance * i;
			instancingObj->World._43 = distance * j;
			instancingObj->TexTransform = MathHelper::Identity4x4();

			GameObject* top = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "snow_top", "snow_top" + std::to_string(5 * i + j));
			top->Geo = m_MeshRef->m_GeometryMesh["snow_top"].get();
			top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			top->IndexCount = top->Geo->DrawArgs["snow_top"].IndexCount;
			top->StartIndexLocation = top->Geo->DrawArgs["snow_top"].StartIndexLocation;
			top->BaseVertexLocation = top->Geo->DrawArgs["snow_top"].BaseVertexLocation;
			top->m_MaterialIndex = 1;
			top->World = MathHelper::Identity4x4();
			top->World._11 = SCALE;
			top->World._22 = SCALE;
			top->World._33 = SCALE;
			XMStoreFloat4x4(&top->World, XMLoadFloat4x4(&top->World) * XMMatrixRotationY(3.141592 * uid2(dre)));
			top->World._41 = distance * i;
			top->World._42 = 0;
			top->World._43 = distance * j;
			top->TexTransform = MathHelper::Identity4x4();
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
			instancingObj->World = MathHelper::Identity4x4();
			instancingObj->World._11 = 0.5;
			instancingObj->World._22 = 0.4;
			instancingObj->World._33 = 0.5;
			int distance = SCALE * 200;
			instancingObj->World._41 = distance * i;
			instancingObj->World._42 = 20;
			instancingObj->World._43 = distance * j;
			instancingObj->TexTransform = MathHelper::Identity4x4();
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
		instancingObj->World = MathHelper::Identity4x4();
		instancingObj->World._11 = SCALE - 0.2f;
		instancingObj->World._22 = SCALE - 0.2f;
		instancingObj->World._33 = SCALE - 0.2f;
		RandomLocation[i] = uid(dre);
		while (i == 1 && RandomLocation[0] == RandomLocation[1]) {
			RandomLocation[i] = uid(dre);
		}
		if (RandomLocation[i] % 2) {
			XMStoreFloat4x4(&instancingObj->World, XMLoadFloat4x4(&instancingObj->World) * XMMatrixRotationY(3.14 * 5 / 6));
			int distance = SCALE * 200;
			instancingObj->World._41 = RandomLocation[i] % 5 * distance - 15.0f;
			instancingObj->World._42 = 10;
			instancingObj->World._43 = RandomLocation[i] / 5 * distance + 15.0f;
		}
		else {
			XMStoreFloat4x4(&instancingObj->World, XMLoadFloat4x4(&instancingObj->World) * XMMatrixRotationY(3.14 * 7 / 6));
			int distance = SCALE * 200;
			instancingObj->World._41 = RandomLocation[i] % 5 * distance + 15.0f;
			instancingObj->World._42 = 10;
			instancingObj->World._43 = RandomLocation[i] / 5 * distance; +15.0f;
		}
		instancingObj->TexTransform = MathHelper::Identity4x4();
	}
	{
		GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "Penguin", "Penguin");
		instancingObj->Geo = m_MeshRef->m_GeometryMesh["Penguin"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["Penguin"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["Penguin"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["Penguin"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = 2;
		instancingObj->World = MathHelper::Identity4x4();
		instancingObj->World._11 = 15;
		instancingObj->World._22 = 15;
		instancingObj->World._33 = 15;

		XMStoreFloat4x4(&instancingObj->World, XMLoadFloat4x4(&instancingObj->World)* XMMatrixRotationY(3.14 ));
		XMStoreFloat4x4(&instancingObj->World, XMLoadFloat4x4(&instancingObj->World)* XMMatrixRotationX(3.14/2));

		instancingObj->World._41 = 200;
		instancingObj->World._42 = 30;
		instancingObj->World._43 = 80;
		instancingObj->TexTransform = MathHelper::Identity4x4();
	}

	GameObject* Sea = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "Sea", "Sea0");
	Sea->Geo = m_MeshRef->m_GeometryMesh["geo"].get();
	Sea->IndexCount = Sea->Geo->DrawArgs["grid"].IndexCount;
	Sea->StartIndexLocation = Sea->Geo->DrawArgs["grid"].StartIndexLocation;
	Sea->BaseVertexLocation = Sea->Geo->DrawArgs["grid"].BaseVertexLocation;
	Sea->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	Sea->m_MaterialIndex = 3;

	Sea->World = MathHelper::Identity4x4();

	Sea->World._11 = 750;
	Sea->World._22 = 750;
	Sea->World._33 = 750;

	Sea->World._41 = SCALE*400;
	Sea->World._42 = -SCALE * 100;
	Sea->World._43 = SCALE*400;
	
	Sea->TexTransform = MathHelper::Identity4x4();
}
