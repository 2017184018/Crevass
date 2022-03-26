#pragma once
#include "Component.h"
#include "GeometryMesh.h"
#include "Waves.h"

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
	void BuildStreamMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, const char* path, std::string meshName);
	void BuildWaves(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, Waves *wave);
public:
	std::unordered_map<std::string, std::unique_ptr<GeometryMesh>>	m_GeometryMesh;
};

