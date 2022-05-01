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

//#include "Network.h"
//#include "MainFramework.h"

#include <random>
using namespace Core;

//random_device rd;
//default_random_engine dre(rd());
//uniform_int_distribution<> uid{ 0,8 }; //����� ��ġ
//uniform_int_distribution<> uid2{ 0,3 }; //���� ���� ȸ��
//uniform_int_distribution<> uid3{ 0,24 }; //���� ����


void CREVASS::Startup(void)
{
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

	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/ice_cube_2.mesh", "icecube");    //fbx
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_top.mesh", "snow_top");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowman.mesh", "snowman");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snowbox_dest.mesh", "icicle");
	m_MeshRef->BuildStreamMeshes(g_Device.Get(), g_CommandList.Get(), "./Models/snow_cube.mesh", "snowcube");

	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());
	m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "icecube", m_MeshRef->m_GeometryMesh["icecube"].get()->DrawArgs["icecube"].Bounds);

	//husky
	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "husky");
	m_MeshRef->BuildBoundingBoxMeshes(g_Device.Get(), g_CommandList.Get(), "husky", m_MeshRef->m_GeometryMesh["husky"].get()->DrawArgs["husky"].Bounds);
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Walk");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("husky", "Attack");

	m_MeshRef->BuildSkinnedModel(g_Device.Get(), g_CommandList.Get(), "Penguin_LOD0skin");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Run");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Idle");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Walk");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Jump");
	m_MeshRef->BuildSkinnedModelAnimation("Penguin_LOD0skin", "Attack");

	mWaves = std::make_unique<Waves>(128, 128, 1.0f, 0.03f, 4.0f, 0.2f);

	m_MeshRef->BuildWaves(g_Device.Get(), g_CommandList.Get(), mWaves.get());

	m_MaterialRef->BuildMaterials();

	// Build RenderItem


	SceneManager::GetApp()->InitializeScenes();
	BuildCharacters();
	SceneManager::GetApp()->EnterScene(SceneType::Lobby);

	GraphicsContext::GetApp()->VertexCount = mWaves->VertexCount();
	GraphicsContext::GetApp()->passCount = 1;
	GraphicsContext::GetApp()->skinnedObjectCount = BoneIndex::Count;
	GraphicsContext::GetApp()->materialCount = m_MaterialRef->m_Materials.size();


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

	SceneManager::GetApp()->UpdateScene(deltaT);
	m_Camera->UpdateViewMatrix();

	GraphicsContext::GetApp()->UpdateMainPassCB(*m_Camera);

}

void CREVASS::RenderScene(void)
{
	SceneManager::GetApp()->RenderScene();
}

void CREVASS::BuildCharacters()
{
	AppContext->CreateCharacter("husky", "husky0", "None", BoneIndex::Husky);
	AppContext->CreateCharacter("Penguin_LOD0skin", "Penguin_LOD0skin0", "None", BoneIndex::Penguin);
}