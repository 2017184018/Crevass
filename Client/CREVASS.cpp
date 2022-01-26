#include "pch.h"
#include "CREVASS.h"
#include "CommandContext.h"
#include "GameObject.h"

#include "MeshReference.h"
#include "MaterialReference.h"

using namespace Core;




void CREVASS::Startup(void)
{
	m_Camera.SetPosition(0.0f, 2.0f, -15.0f);
	m_Camera.SetLens(0.25f * MathHelper::Pi, static_cast<float>(g_DisplayWidth) / g_DisplayHeight, 1.0f, 1000.0f);

	// Build Mesh & Material & Texture
	m_MeshRef = new MeshReference;
	m_MaterialRef = new MaterialReference;

	m_MeshRef->BuildSkullGeometry(g_Device.Get(), g_CommandList.Get());
	m_MeshRef->BuildGeoMeshes(g_Device.Get(), g_CommandList.Get());

	m_MaterialRef->BuildMaterials();

	// Build RenderItem
	BuildScene();

	GraphicsContext::GetApp()->passCount = 1;
	GraphicsContext::GetApp()->materialCount = m_MaterialRef->m_Materials.size();

	for (auto& p : m_RItemsMap)
	{
		GraphicsContext::GetApp()->BuildInstanceBuffer(p.second);
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
	//Map = object info
	//Vec = game object
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap, m_RItemsVec);
	GraphicsContext::GetApp()->UpdateMaterialBuffer(m_MaterialRef->m_Materials);
	GraphicsContext::GetApp()->UpdateMainPassCB(m_Camera);
}

void CREVASS::RenderScene(void)
{
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["skull"], m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(m_RItemsMap["sky"], m_RItemsVec);
}

void CREVASS::OnKeyboardInput(const float deltaT)
{
	if (GetAsyncKeyState('W') & 0x8000)
		m_Camera.Walk(20.0f * deltaT);

	if (GetAsyncKeyState('S') & 0x8000)
		m_Camera.Walk(-20.0f * deltaT);

	if (GetAsyncKeyState('A') & 0x8000)
		m_Camera.Strafe(-20.0f * deltaT);

	if (GetAsyncKeyState('D') & 0x8000)
		m_Camera.Strafe(20.0f * deltaT);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_Camera.RotateY(0.005);

	if (GetAsyncKeyState('E') & 0x8000)
		m_Camera.RotateY(-0.005);

	m_Camera.UpdateViewMatrix();
}

void CREVASS::BuildScene()
{
	//layer ,mesh type , id 
	GameObject* skyRitem = CreateObject<GameObject>(RenderLayer::ID_SKY, "sky", "sky0");
	skyRitem->Geo = m_MeshRef->m_GeometryMesh["geo"].get();
	//XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
	//skyRitem->Mat = m_MaterialRef->m_Materials["desertcube1024"].get();
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->m_MaterialIndex = 0;

	skyRitem->World = MathHelper::Identity4x4();
	skyRitem->TexTransform = MathHelper::Identity4x4();

	// Instaincing Obj
	GameObject* instancingObj = CreateObject<GameObject>(RenderLayer::ID_OPAQUE, "skull", "skull0");
	instancingObj->Geo = m_MeshRef->m_GeometryMesh["skull"].get();
	instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	instancingObj->IndexCount = instancingObj->Geo->DrawArgs["skull"].IndexCount;
	instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["skull"].StartIndexLocation;
	instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["skull"].BaseVertexLocation;

	instancingObj->m_MaterialIndex = 1;
	// Generate instance data.
	// 위치와 텍스처, 재질에만 자유로움
	// 매쉬는 단 한개만 가능
	//const int n = 1;
	//instancingObj->InstanceCount = 1;
	//instancingObj->Instances.resize(instancingObj->InstanceCount);

	//float width = 200.0f;
	//float height = 200.0f;
	//float depth = 200.0f;

	//float x = -0.5f * width;
	//float y = -0.5f * height;
	//float z = -0.5f * depth;
	//float dx = width / (n - 1);
	//float dy = height / (n - 1);
	//float dz = depth / (n - 1);

	//int index = 0;
	//// Position instanced along a 3D grid.
	//instancingObj->Instances[index].World = XMFLOAT4X4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0, 0, 0, 1.0f);

	//XMStoreFloat4x4(&instancingObj->Instances[index].TexTransform, XMMatrixScaling(2.0f, 2.0f, 1.0f));
	//instancingObj->Instances[index].MaterialIndex = 1;

}
