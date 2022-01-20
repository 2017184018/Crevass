#pragma once
#include "Component.h"
#include "GeometryMesh.h"

// ���� ������ �޽��� �� �ѹ��� �����Ǿ��Ѵ�.
// �޽��� �����͸� �����־ ������Ʈ���� �����͸� ������ ����.
// �޽� ������ ������ �����Ѵ�.
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

