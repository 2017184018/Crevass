#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
#include <fbxsdk.h>

struct CtrlPoint
{
	DirectX::XMFLOAT3 mPosition;
//	std::vector<BoneIndexAndWeight> mBoneInfo;
	//std::string mBoneName;

	//CtrlPoint()
	//{
	//	mBoneInfo.reserve(4);
	//}

	//void SortBlendingInfoByWeight()
	//{
	//	std::sort(mBoneInfo.begin(), mBoneInfo.end());
	//}

};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;

	bool operator==(const Vertex& other) const
	{
		if (Pos.x != other.Pos.x || Pos.y != other.Pos.y || Pos.z != other.Pos.z)
			return false;

		if (Normal.x != other.Normal.x || Normal.y != other.Normal.y || Normal.z != other.Normal.z)
			return false;

		if (TexC.x != other.TexC.x || TexC.y != other.TexC.y)
			return false;

		return true;
	}
};

namespace std {
	template <>
	struct hash<DirectX::XMFLOAT2>
	{
		std::size_t operator()(const DirectX::XMFLOAT2& k) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<float>()(k.x)
				^ (hash<float>()(k.y) << 1)) >> 1);
		}
	};

	template <>
	struct hash<DirectX::XMFLOAT3>
	{
		std::size_t operator()(const DirectX::XMFLOAT3& k) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<float>()(k.x)
				^ (hash<float>()(k.y) << 1)) >> 1)
				^ (hash<float>()(k.z) << 1);
		}
	};

	template <>
	struct hash<Vertex>
	{
		std::size_t operator()(const Vertex& k) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<DirectX::XMFLOAT3>()(k.Pos)
				^ (hash<DirectX::XMFLOAT3>()(k.Normal) << 1)) >> 1)
				^ (hash<DirectX::XMFLOAT2>()(k.TexC) << 1);
		}
	};
}
struct Material
{
	// Unique material name for lookup.
	std::string Name;
	// Material constant buffer data used for shading.
	DirectX::XMFLOAT3 Ambient = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	DirectX::XMFLOAT3 Specular = { 0.01f, 0.01f, 0.01f };
	DirectX::XMFLOAT3 Emissive = { 0.01f, 0.01f, 0.01f };
	float Roughness = .25f;
	DirectX::XMFLOAT4X4 MatTransform = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

};


class FBXLoader {
public:
	FBXLoader();
	~FBXLoader();

	void LoadFBX(
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>& outMaterial,
		std::string fileName
		);

	void  LoadNode(FbxNode* node, std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector);

	bool LoadMesh(
		std::string fileName,
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector,
		std::vector<Material>* outMaterial = nullptr);

	void GetControlPoints(fbxsdk::FbxNode* pFbxRootNode);

	void GetVerticesAndIndice(
		fbxsdk::FbxMesh* pMesh,
		std::vector<Vertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector);

	void GetMaterials(fbxsdk::FbxNode* pNode, std::vector<Material>& outMaterial);

//	void GetMaterialAttribute(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& outMaterial);

//	void GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, Material& Mat);

	void ExportMesh(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>& outMaterial, std::string fileName);

private:
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;
};