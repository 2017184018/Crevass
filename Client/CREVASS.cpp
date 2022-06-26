#include "pch.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "SceneManager.h"

#include "MeshReference.h"
#include "ApplicationContext.h"
#include "MaterialReference.h"

#include "CommandCenter.h"
//#include "Character.h"
//#include "CharacterParts.h"

#include "Network.h"
#include "MainFramework.h"

#include <random>
using namespace Core;

void CREVASS::Startup(void)
{
	//Lights
	m_Lights[LIGHT_NAME_DIRECTIONAL] = std::make_unique<Light>();
	m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction = { 0.57735f, -0.81735f, -1.07735 };
	m_Lights[LIGHT_NAME_DIRECTIONAL]->Strength = { 0.9f, 0.8f, 0.7f };

	//g_pFramework->m_pNetwork->Send(CS_READY);
	//g_pFramework->m_pNetwork->Recv();
	m_Camera = new Camera;

	m_Camera->SetPosition(45.0f * 4, 45.0f * 2, -45.0f * 3);
	m_Camera->SetLens(0.25f * MathHelper::Pi, static_cast<float>(g_DisplayWidth) / g_DisplayHeight, 1.0f, 2000.0f);

	m_SceneManager = SceneManager::GetApp();
	m_CommandCenter = CommandCenter::GetApp();

	// Build Mesh & Material & Texture
	m_MeshRef = MeshReference::GetApp();
	m_MaterialRef = MaterialReference::GetApp();

	BuildStream();

	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());
	m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "icecube", m_MeshRef->m_GeometryMesh["icecube"].get()->DrawArgs["icecube"].Bounds);

	//Character
	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "husky");
	m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "husky", m_MeshRef->m_GeometryMesh["husky"].get()->DrawArgs["husky"].Bounds);
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Attack");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Fall");

	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "Penguin_LOD0skin");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Attack");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Fall");

	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "ArcticFox");
	m_MeshRef->BuildSkinnedModelAnimation("ArcticFox", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("ArcticFox", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("ArcticFox", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("ArcticFox", "Attack");
	m_MeshRef->BuildSkinnedModelAnimation("ArcticFox", "Fall");

	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "PolarBear");
	m_MeshRef->BuildSkinnedModelAnimation("PolarBear", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("PolarBear", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("PolarBear", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("PolarBear", "Attack");
	m_MeshRef->BuildSkinnedModelAnimation("PolarBear", "Fall");

	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "Seal");
	m_MeshRef->BuildSkinnedModelAnimation("Seal", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("Seal", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("Seal", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("Seal", "Attack");
	m_MeshRef->BuildSkinnedModelAnimation("Seal", "Fall");

	mWaves = std::make_unique<Waves>(128, 128, 1.0f, 0.03f, 4.0f, 0.2f);

	m_MeshRef->BuildWaves(g_Device.Get(), g_CommandList.Get(), mWaves.get());

	MaterialReference::GetApp()->BuildMaterials();

	// Build RenderItem

	SceneManager::GetApp()->InitializeScenes();
	// 위 아래 바꾸면 이상함 
	BuildCharacters();
	SceneManager::GetApp()->EnterScene(SceneType::Lobby);

	GraphicsContext::GetApp()->VertexCount = mWaves->VertexCount();
	GraphicsContext::GetApp()->passCount = 2;
	GraphicsContext::GetApp()->skinnedObjectCount = BoneIndex::Count;
	GraphicsContext::GetApp()->materialCount = MaterialReference::GetApp()->m_Materials.size();

	for (auto& p : AppContext->m_RItemsMap)
	{
		GraphicsContext::GetApp()->BuildInstanceBuffer(p.second);
	}
}

void CREVASS::Cleanup(void)
{
	/* Clear Cameras */
	SAFE_DELETE_PTR(m_Camera);

	for (auto& p : AppContext->m_RItemsMap)
	{
		SAFE_DELETE_PTR(p.second);
	}

	for (auto& p : AppContext->m_RItemsVec)
		SAFE_DELETE_PTR(p);

	AppContext->m_RItemsVec.clear();
	AppContext->m_RItemsMap.clear();

	/* Release Commands */
	CommandCenter::GetApp()->Release();

	/* Release References */
	SceneManager::DestroyApp();
	CommandCenter::DestroyApp();
	MeshReference::DestroyApp();
	MaterialReference::DestroyApp();
	ApplicationContext::DestroyApp();
}

void CREVASS::Update(float deltaT)
{
	/*CommandCenter*/
	CommandCenter::GetApp()->Order(deltaT);

	g_pFramework->m_pNetwork->Recv();
	SceneManager::GetApp()->UpdateScene(deltaT);
	// PassCB
	m_Camera->UpdateViewMatrix();
	GraphicsContext::GetApp()->UpdateMainPassCB(*m_Camera, m_Lights[LIGHT_NAME_DIRECTIONAL].get());

	
}

void CREVASS::RenderScene(void)
{
	SceneManager::GetApp()->RenderScene();
}

void CREVASS::BuildCharacters()
{
	for (int i = 0; i < 5; ++i) {
		AppContext->CreateCharacter("husky", "husky"+std::to_string(i), "husky", BoneIndex::Husky);
		AppContext->CreateCharacter("Penguin_LOD0skin", "Penguin_LOD0skin" + std::to_string(i), "Penguin", BoneIndex::Penguin);
		AppContext->CreateCharacter("ArcticFox", "ArcticFox" + std::to_string(i), "ArcticFox", BoneIndex::Fox);
		AppContext->CreateCharacter("PolarBear", "PolarBear" + std::to_string(i), "PolarBear", BoneIndex::PolarBear);
		AppContext->CreateCharacter("Seal", "Seal" + std::to_string(i), "Seal", BoneIndex::Seal);
	}
}

void CREVASS::BuildStream() {
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/ice_cube_2.mesh", "icecube");    //fbx
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_top.mesh", "snow_top");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowman.mesh", "snowman");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowbox_dest.mesh", "icicle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_cube.mesh", "snowcube");

	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_fish_m.mesh", "fish");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_kayak_m.mesh", "kayak");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_kayakPaddle_m.mesh", "kayakpaddle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_rock00.mesh", "rock_0");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_rock01.mesh", "rock_1");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_tent01_m.mesh", "tent");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/mountains01_m.mesh", "mountain");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/igloo01_m.mesh", "igloo");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_sled_m.mesh", "sled");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/arctic_fishrack01_m.mesh", "fishrack");
}