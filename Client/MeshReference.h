#pragma once
#include "Component.h"
#include "GeometryMesh.h"

// 같은 종류의 메쉬는 딱 한번만 생성되야한다.
// 메쉬들 포인터만 갖고있어서 오브젝트에서 포인터만 가져다 쓴다.
// 메쉬 제작은 씬에서 제작한다.
class MeshReference final 
{

public:
	explicit MeshReference();
	virtual ~MeshReference();

public:
	void BuildGeoMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);
	void BuildSkullGeometry(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);

public:
	std::unordered_map<std::string, std::unique_ptr<GeometryMesh>>	m_GeometryMesh;
};

