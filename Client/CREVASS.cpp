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
	m_MaterialRef->BuildMaterials();

	// Build RenderItem
	BuildScene();

	GraphicsContext::GetApp()->passCount = 1;
	GraphicsContext::GetApp()->InstanceCount = 125;
	GraphicsContext::GetApp()->materialCount = m_MaterialRef->m_Materials.size();
}

void CREVASS::Cleanup(void)
{
	for (auto& p : m_AllRItems)
		SAFE_DELETE_PTR(p);
	m_AllRItems.clear();

	SAFE_DELETE_PTR(m_MeshRef);
	SAFE_DELETE_PTR(m_MaterialRef);
}

void CREVASS::Update(float deltaT)
{
	OnKeyboardInput(deltaT);

	GraphicsContext::GetApp()->UpdateInstanceData(m_AllRItems);
	GraphicsContext::GetApp()->UpdateMaterialBuffer(m_MaterialRef->m_Materials);
	GraphicsContext::GetApp()->UpdateMainPassCB(m_Camera);
}

void CREVASS::RenderScene(void)
{
	GraphicsContext::GetApp()->DrawRenderItems(m_AllRItems);
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
	// Instaincing Obj
	GameObject* instancingObj = new GameObject;
	instancingObj->Initialize();
	instancingObj->Geo = m_MeshRef->m_GeometryMesh[MeshReference::MESHID::ID_SKULL].get();
	instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	instancingObj->IndexCount = instancingObj->Geo->DrawArgs[GeometryMesh::GEOID::ID_SKULL].IndexCount;
	instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs[GeometryMesh::GEOID::ID_SKULL].StartIndexLocation;
	instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs[GeometryMesh::GEOID::ID_SKULL].BaseVertexLocation;
	instancingObj->Bounds = instancingObj->Geo->DrawArgs[GeometryMesh::GEOID::ID_SKULL].Bounds;
	instancingObj->Mat = m_MaterialRef->m_Materials[MaterialReference::MATERIALID::ID_ICE].get();

	// Generate instance data.
	// 위치와 텍스처, 재질에만 자유로움
	// 매쉬는 단 한개만 가능
	const int n = 5;
	instancingObj->InstanceCount = n * n * n;
	instancingObj->Instances.resize(instancingObj->InstanceCount);

	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;

	float x = -0.5f * width;
	float y = -0.5f * height;
	float z = -0.5f * depth;
	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int index = k * n * n + i * n + j;
				// Position instanced along a 3D grid.
				instancingObj->Instances[index].World = XMFLOAT4X4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x + j * dx, y + i * dy, z + k * dz, 1.0f);

				XMStoreFloat4x4(&instancingObj->Instances[index].TexTransform, XMMatrixScaling(2.0f, 2.0f, 1.0f));
			//	instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY;
			//	instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY2;
			//	instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY3;
			//	instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY4;
			//	instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY5;
				instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_SKY6;

				//instancingObj->Instances[index].MaterialIndex = index % ObjectManager::GetApp()->m_MaterialRef->m_Materials.size();
			}
		}
	}

	m_AllRItems.push_back(std::move(instancingObj));
}
