#include "pch.h"
#include "LogoScene.h"

#include "MeshReference.h"
#include "MaterialReference.h"

#include "GameObject.h"

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

bool LogoScene::InitializeScene()
{
	//BuildRenderItems();
	//BuildSkyCube();

	return true;
}

void LogoScene::UpdateScene(const float& fTimeDelta)
{
}

void LogoScene::ReleaseScene()
{
	cout << "Release LogoScene" << endl;
}

//void LogoScene::BuildRenderItems()
//{
//	// Instaincing Obj
//	GameObject* instancingObj = new InstancingObject;
//	instancingObj->Initialize();
//	instancingObj->SetMesh(MeshReference::MESHID::ID_SKULL, GeometryMesh::GEOID::ID_SKULL);
//	instancingObj->SetMaterial(MaterialReference::MATERIALID::ID_ICE);
//
//	// Generate instance data.
//	// 위치와 텍스처, 재질에만 자유로움
//	// 매쉬는 단 한개만 가능
//	const int n = 5;
//	instancingObj->InstanceCount = n * n * n;
//	instancingObj->Instances.resize(instancingObj->InstanceCount);
//
//	float width = 200.0f;
//	float height = 200.0f;
//	float depth = 200.0f;
//
//	float x = -0.5f * width;
//	float y = -0.5f * height;
//	float z = -0.5f * depth;
//	float dx = width / (n - 1);
//	float dy = height / (n - 1);
//	float dz = depth / (n - 1);
//	for (int k = 0; k < n; ++k)
//	{
//		for (int i = 0; i < n; ++i)
//		{
//			for (int j = 0; j < n; ++j)
//			{
//				int index = k * n * n + i * n + j;
//				// Position instanced along a 3D grid.
//				instancingObj->Instances[index].World = XMFLOAT4X4(
//					1.0f, 0.0f, 0.0f, 0.0f,
//					0.0f, 1.0f, 0.0f, 0.0f,
//					0.0f, 0.0f, 1.0f, 0.0f,
//					x + j * dx, y + i * dy, z + k * dz, 1.0f);
//
//				XMStoreFloat4x4(&instancingObj->Instances[index].TexTransform, XMMatrixScaling(2.0f, 2.0f, 1.0f));
//					instancingObj->Instances[index].MaterialIndex = MaterialReference::MATERIALID::ID_GRASS;
//
//				//instancingObj->Instances[index].MaterialIndex = index % ObjectManager::GetApp()->m_MaterialRef->m_Materials.size();
//			}
//		}
//	}
//
//	ObjectManager::GetApp()->m_AllRItems.push_back(std::move(instancingObj));
//
//	// All the render items are opaque.
//	for (auto& e : ObjectManager::GetApp()->m_AllRItems)
//		ObjectManager::GetApp()->m_RitemLayer[static_cast<int>(ObjectManager::RENDER_LAYER::ID_OPAQUE)].push_back(e);
//}
//
//void LogoScene::BuildSkyCube()
//{
//	// SkyCube
//	GameObject* skyCubeObj = new SkyCube;
//	skyCubeObj->Initialize();
//	skyCubeObj->SetMesh(MeshReference::MESHID::ID_GEO, GeometryMesh::GEOID::ID_SPHERE);
//	skyCubeObj->SetMaterial(MaterialReference::MATERIALID::ID_SKY);
//
//	ObjectManager::GetApp()->m_RitemLayer[static_cast<int>(ObjectManager::RENDER_LAYER::ID_SKYCUBE)].push_back(skyCubeObj);
//	ObjectManager::GetApp()->m_AllRItems.push_back(std::move(skyCubeObj));
//}

