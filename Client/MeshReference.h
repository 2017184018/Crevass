#pragma once
#include "Singleton.h"
#include "GeometryMesh.h"
#include "SkinnedModelInstance.h"
#include "Waves.h"

// 같은 종류의 메쉬는 딱 한번만 생성되야한다.
// 메쉬들 포인터만 갖고있어서 오브젝트에서 포인터만 가져다 쓴다.
// 메쉬 제작은 씬에서 제작한다.
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

	// Mesh Material Reference를 합치자
	//std::unordered_map<std::string, std::unique_ptr<Material>>	m_Materials;
	
	std::vector<std::string> mSkinnedTextureNames;
}; 

