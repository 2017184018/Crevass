#pragma once
#include "Component.h"
#include "GeometryMesh.h"

// ���� ������ �޽��� �� �ѹ��� �����Ǿ��Ѵ�.
// �޽��� �����͸� �����־ ������Ʈ���� �����͸� ������ ����.
// �޽� ������ ������ �����Ѵ�.
class MeshReference final : public Component
{
public:
	enum MESHID { ID_GEO, ID_SKULL, ID_CAR, ID_HELLICOPTER, ID_TERRAIN };

public:
	explicit MeshReference();
	virtual ~MeshReference();

public:
	virtual HRESULT InitializeComponent() override;
	virtual void	UpdateComponent(const float& fTimeDelta) override;
	virtual void	ReleaseComponent() override;

public:
	void BuildGeoMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);
	void BuildSkullGeometry(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);

public:
	std::unordered_map<int, std::unique_ptr<GeometryMesh>>	m_GeometryMesh;
};

