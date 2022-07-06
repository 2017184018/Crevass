#pragma once
#include "Singleton.h"
#include "GeometryMesh.h"
#include "SkinnedModelInstance.h"
#include "Waves.h"

// ���� ������ �޽��� �� �ѹ��� �����Ǿ��Ѵ�.
// �޽��� �����͸� �����־ ������Ʈ���� �����͸� ������ ����.
// �޽� ������ ������ �����Ѵ�.
class MeshReference final : public TemplateSingleton<MeshReference>
{

public:
	explicit MeshReference();
	virtual ~MeshReference();

public:
	void BuildGeoMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);
	void BuildBoundingBoxMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string meshName, BoundingBox BB);
	void BuildStreamMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, const char* path, std::string meshName);
	void BuildSkinnedModel(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string meshName);
	void BuildSkinnedModelAnimation(std::string meshName, const std::string clipName);
	void BuildWaves(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, Waves* wave);
	void BuildParticle(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string particleName, int particleCount, DirectX::XMFLOAT2 particleSize,
		float VelX, float VelY, float VelZ,
		DirectX::XMFLOAT2 particleStartTime, DirectX::XMFLOAT2 particleLifeTime);

private:
	bool LoadSkeletonFile(SkinnedData& outSkinnedData, std::string path);

	bool LoadMeshFile(
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial,
		std::string path);
	bool LoadMeshFile(
		std::vector<CharacterVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial,
		std::string path);
	bool LoadAnimationFile(SkinnedData& outSkinnedData, std::string& path, const std::string clipName);
	
public:
	std::unordered_map<std::string, std::unique_ptr<GeometryMesh>>	m_GeometryMesh;
	std::map<std::string, std::unique_ptr<SkinnedModelInstance>> m_SkinnedModelInsts;

	// Mesh Material Reference�� ��ġ��
	//std::unordered_map<std::string, std::unique_ptr<Material>>	m_Materials;
	
	std::vector<std::string> mSkinnedTextureNames;
}; 

