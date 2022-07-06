#include "pch.h"
#include "MeshReference.h"
#include "GeometryGenerator.h"

MeshReference::MeshReference()
{
}

MeshReference::~MeshReference()
{
}

void MeshReference::BuildGeoMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList)
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(1.0f, 1.0f, 128, 128);
	GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);
	GeometryGenerator::MeshData quad = geoGen.CreateQuad(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();
	UINT quadVertexOffset = cylinderVertexOffset + (UINT)cylinder.Vertices.size();

	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();
	UINT quadIndexOffset = cylinderIndexOffset + (UINT)cylinder.Indices32.size();

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	SubmeshGeometry quadSubmesh;
	quadSubmesh.IndexCount = (UINT)quad.Indices32.size();
	quadSubmesh.StartIndexLocation = quadIndexOffset;
	quadSubmesh.BaseVertexLocation = quadVertexOffset;

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size()+
		quad.Vertices.size();;

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].TexC = grid.Vertices[i].TexC;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].TexC = sphere.Vertices[i].TexC;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].TexC = cylinder.Vertices[i].TexC;
	}
	for (int i = 0; i < quad.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = quad.Vertices[i].Position;
		vertices[k].Normal = quad.Vertices[i].Normal;
		vertices[k].TexC = quad.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(quad.GetIndices16()), std::end(quad.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<GeometryMesh>();

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;
	geo->DrawArgs["quad"] = quadSubmesh;

	m_GeometryMesh["geo"] = std::move(geo);
}

void MeshReference::BuildBoundingBoxMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string meshName, BoundingBox BB)
{
	/*BoundingBox B2;
	B2.Extents = XMFLOAT3(10.0, 30.0, 12.0);*/

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(BB, 3);
	UINT boxVertexOffset = 0;

	UINT boxIndexOffset = 0;

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	auto totalVertexCount =
		box.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<GeometryMesh>();

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs[meshName + "BB"] = boxSubmesh;

	m_GeometryMesh[meshName + "BB"] = std::move(geo);
}

void MeshReference::BuildWaves(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, Waves* wave)
{
	std::vector<std::uint16_t> indices(3 * wave->TriangleCount()); // 3 indices per face
	assert(wave->VertexCount() < 0x0000ffff);

	// Iterate over each quad.
	int m = wave->RowCount();
	int n = wave->ColumnCount();
	int k = 0;

	float width = 1;
	float depth = 1;

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	for (int i = 0; i < m - 1; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			indices[k] = i * n + j;
			indices[k + 1] = i * n + j + 1;
			indices[k + 2] = (i + 1) * n + j;

			indices[k + 3] = (i + 1) * n + j;
			indices[k + 4] = i * n + j + 1;
			indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}

	UINT vbByteSize = (*wave).VertexCount() * sizeof(Vertex);
	UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<GeometryMesh>();

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	geo->DrawArgs["wave"] = submesh;

	m_GeometryMesh["wave"] = std::move(geo);
}

void MeshReference::BuildParticle(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string particleName, int particleCount, DirectX::XMFLOAT2 particleSize)
{
	// 입력받아야 할 자료들
// world, particle name
// particle Param: pos, size,,,, 
// build particle verticies
	std::vector<ParticleVertex> particleVertices;
	particleVertices.resize(particleCount);

	for (int i = 0; i < particleCount; ++i)
	{

		particleVertices[i].pos = XMFLOAT3(0.f,0.f,0.f);
;		particleVertices[i].size = particleSize;
		particleVertices[i].velocity = XMFLOAT3(MathHelper::RandF(-50.0f, 50.0f), MathHelper::RandF(200.0f, 200.0f), MathHelper::RandF(-50.0f, 50.0f));
		particleVertices[i].Starttime = MathHelper::RandF(0.0f, 6.0f);
		particleVertices[i].Lifetime = MathHelper::RandF(3.0f, 5.0f);

		//m_ParticleVertices[i].size ,,,
	}

	// build paritcle indicies
	std::vector<std::uint16_t> indices;
	indices.resize(particleCount);

	for (int i = 0; i < particleCount; ++i)
	{
		indices[i] = i;
	}

	const UINT vbByteSize = (UINT)particleVertices.size() * sizeof(ParticleVertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<GeometryMesh>();
	geo->Name = particleName;

	// 파티클 메시(정점, 인덱스 버퍼) 리소스 할당
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), particleVertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, particleVertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(ParticleVertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	geo->DrawArgs[particleName] = submesh;

	m_GeometryMesh[particleName] = std::move(geo);

	// 멤버변수 메모리 해제
	particleVertices.clear();
	indices.clear();
}


void MeshReference::BuildStreamMeshes(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, const char* path, std::string meshName)
{
	//string path2 = "Models\\" + meshName;

	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<Material> materials;

	LoadMeshFile(vertices, indices, &materials, path);

	const UINT vbByteSize = vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = indices.size() * sizeof(std::int32_t);

	auto geo = std::make_unique<GeometryMesh>();
	geo->Name = meshName;

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	BoundingBox bounds;


	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (auto& p : vertices)
	{
		XMVECTOR P = XMLoadFloat3(&p.Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin));

	//ofstream out{ "bb.txt",ios::app };
	//out << meshName << " " << bounds.Extents.x<<" " << bounds.Extents.y << " " << bounds.Extents.z << endl;

	SubmeshGeometry submesh;
	submesh.IndexCount = indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	submesh.Bounds = bounds;

	geo->DrawArgs[meshName] = submesh;
	m_GeometryMesh[meshName] = std::move(geo);

}

void MeshReference::BuildSkinnedModel(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList, std::string meshName)
{
	string path = "Models\\Character\\" + meshName;

	std::vector<CharacterVertex> CVertex;
	std::vector<std::uint32_t> indices;
	std::vector<Material> materials;
	std::unique_ptr<SkinnedData> skinnedInfo = make_unique<SkinnedData>();

	LoadMeshFile(CVertex, indices, &materials, path);
	LoadSkeletonFile(*skinnedInfo, path);

	// Geo CreateDefaultBuffer
	std::unique_ptr<SkinnedModelInstance> skinnedModelInst = std::make_unique<SkinnedModelInstance>();
	skinnedModelInst->SkinnedInfo = std::move(skinnedInfo);
	skinnedModelInst->FinalTransforms.resize(skinnedModelInst->SkinnedInfo->BoneCount());
	skinnedModelInst->ClipName = "Idle";
	skinnedModelInst->TimePos = 0.0f;
	m_SkinnedModelInsts[meshName] = std::move(skinnedModelInst);

	const UINT vbByteSize = (UINT)CVertex.size() * sizeof(CharacterVertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint32_t);

	auto geo = std::make_unique<GeometryMesh>();
	geo->Name = meshName;

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), CVertex.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, CVertex.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(pDevice,
		pCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(CharacterVertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	// Bounds
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (auto& p : CVertex)
	{
		XMVECTOR P = XMLoadFloat3(&p.Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);

	}

	BoundingBox bounds;
	//XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax) * 20);
	//XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin) * 20);
	XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin));

	//ofstream out{ "bb.txt",ios::app };
	//out << meshName << " " << bounds.Extents.x << " " << bounds.Extents.z << " " << bounds.Extents.y << endl;

	SubmeshGeometry submesh;
	submesh.IndexCount = indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	submesh.Bounds = bounds;
	geo->DrawArgs[meshName] = submesh;

	m_GeometryMesh[meshName] = std::move(geo);
}

void MeshReference::BuildSkinnedModelAnimation(std::string meshName, const std::string clipName)
{
	if (!m_SkinnedModelInsts.count(meshName))
	{
		cout << "LoadModelAnimation: None MeshName" << endl;
	}
	else
	{
		string path = "Models\\Character\\" + meshName;
		LoadAnimationFile(*m_SkinnedModelInsts[meshName]->SkinnedInfo, path, clipName);
	}
}

bool MeshReference::LoadMeshFile(std::vector<Vertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>* outMaterial, std::string path)
{
	//path += ".mesh";
	std::ifstream fileIn(path);

	uint32_t vertexSize, indexSize;
	uint32_t materialSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> vertexSize;
		fileIn >> ignore >> indexSize;
		fileIn >> ignore >> materialSize;

		if (vertexSize == 0 || indexSize == 0)
			return false;

		// Material Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < materialSize; ++i)
		{
			Material tempMaterial;

			fileIn >> ignore >> tempMaterial.Name;
			fileIn >> ignore >> tempMaterial.Ambient.x >> tempMaterial.Ambient.y >> tempMaterial.Ambient.z;
			fileIn >> ignore >> tempMaterial.DiffuseAlbedo.x >> tempMaterial.DiffuseAlbedo.y >> tempMaterial.DiffuseAlbedo.z >> tempMaterial.DiffuseAlbedo.w;
			fileIn >> ignore >> tempMaterial.FresnelR0.x >> tempMaterial.FresnelR0.y >> tempMaterial.FresnelR0.z;
			fileIn >> ignore >> tempMaterial.Specular.x >> tempMaterial.Specular.y >> tempMaterial.Specular.z;
			fileIn >> ignore >> tempMaterial.Emissive.x >> tempMaterial.Emissive.y >> tempMaterial.Emissive.z;
			fileIn >> ignore >> tempMaterial.Roughness;
			fileIn >> ignore;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileIn >> tempMaterial.MatTransform.m[i][j];
				}
			}
			(*outMaterial).push_back(tempMaterial);
		}

		// Vertex Data
		for (uint32_t i = 0; i < vertexSize; ++i)
		{
			Vertex vertex;
			fileIn >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
			fileIn >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
			fileIn >> ignore >> vertex.TexC.x >> vertex.TexC.y;
			fileIn >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
			fileIn >> ignore >> vertex.Binormal.x >> vertex.Binormal.y >> vertex.Binormal.z;

			// push_back
			outVertexVector.push_back(vertex);
		}

		// Index Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < indexSize; ++i)
		{
			uint32_t index;
			fileIn >> index;
			outIndexVector.push_back(index);
		}

		return true;
	}

	return false;
}


bool MeshReference::LoadMeshFile(std::vector<CharacterVertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, std::vector<Material>* outMaterial, std::string path)
{
	path += ".cmesh";
	std::ifstream fileIn(path);

	uint32_t vertexSize, indexSize;
	uint32_t materialSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> vertexSize;
		fileIn >> ignore >> indexSize;
		fileIn >> ignore >> materialSize;

		if (vertexSize == 0 || indexSize == 0)
			return false;

		if (outMaterial != nullptr)
		{
			// Material Data
			fileIn >> ignore;
			for (uint32_t i = 0; i < materialSize; ++i)
			{
				Material tempMaterial;

				fileIn >> ignore >> tempMaterial.Name;
				fileIn >> ignore >> tempMaterial.Ambient.x >> tempMaterial.Ambient.y >> tempMaterial.Ambient.z;
				fileIn >> ignore >> tempMaterial.DiffuseAlbedo.x >> tempMaterial.DiffuseAlbedo.y >> tempMaterial.DiffuseAlbedo.z >> tempMaterial.DiffuseAlbedo.w;
				fileIn >> ignore >> tempMaterial.FresnelR0.x >> tempMaterial.FresnelR0.y >> tempMaterial.FresnelR0.z;
				fileIn >> ignore >> static_cast<float>(tempMaterial.Specular.x) >> tempMaterial.Specular.y >> tempMaterial.Specular.z;
				fileIn >> ignore >> tempMaterial.Emissive.x >> tempMaterial.Emissive.y >> tempMaterial.Emissive.z;
				fileIn >> ignore >> tempMaterial.Roughness;
				fileIn >> ignore;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						fileIn >> tempMaterial.MatTransform.m[i][j];
					}
				}
				(*outMaterial).push_back(tempMaterial);
			}
		}

		// Vertex Data
		for (uint32_t i = 0; i < vertexSize; ++i)
		{
			CharacterVertex vertex;
			int temp[4];
			fileIn >> ignore >> vertex.Pos.x >> vertex.Pos.y >> vertex.Pos.z;
			fileIn >> ignore >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
			fileIn >> ignore >> vertex.TexC.x >> vertex.TexC.y;
			fileIn >> ignore >> vertex.Tangent.x >> vertex.Tangent.y >> vertex.Tangent.z;
			fileIn >> ignore >> vertex.Binormal.x >> vertex.Binormal.y >> vertex.Binormal.z;
			fileIn >> ignore >> vertex.BoneWeights.x >> vertex.BoneWeights.y >> vertex.BoneWeights.z;
			fileIn >> ignore >> temp[0] >> temp[1] >> temp[2] >> temp[3];

			for (int j = 0; j < 4; ++j)
			{
				vertex.BoneIndices[j] = temp[j];
			}
			// push_back
			outVertexVector.push_back(vertex);
		}

		// Index Data
		fileIn >> ignore;
		for (uint32_t i = 0; i < indexSize; ++i)
		{
			uint32_t index;
			fileIn >> index;
			outIndexVector.push_back(index);
		}

		return true;
	}

	return false;
}

bool MeshReference::LoadSkeletonFile(SkinnedData& outSkinnedData, std::string path)
{
	path += ".skeleton";
	std::ifstream fileIn(path);

	uint32_t boneSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> boneSize;

		if (boneSize == 0)
			return false;

		// Bone Data
		// Bone Hierarchy
		fileIn >> ignore;
		std::vector<int> boneHierarchy;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			int tempBoneHierarchy;
			fileIn >> tempBoneHierarchy;
			boneHierarchy.push_back(tempBoneHierarchy);
		}

		fileIn >> ignore;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			std::string tempBoneName;
			fileIn >> tempBoneName;
			outSkinnedData.SetBoneName(tempBoneName);
		}
		// Bone Offset
		fileIn >> ignore;
		std::vector<DirectX::XMFLOAT4X4> boneOffsets;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			DirectX::XMFLOAT4X4 tempBoneOffset;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fileIn >> tempBoneOffset.m[i][j];
				}
			}
			boneOffsets.push_back(tempBoneOffset);
		}
		// Bone Submesh Offset
		fileIn >> ignore;
		std::vector<int> boneSubmeshOffset;
		for (uint32_t i = 0; i < boneSize; ++i)
		{
			int tempBoneSubmeshOffset;
			fileIn >> tempBoneSubmeshOffset;
			outSkinnedData.SetSubmeshOffset(tempBoneSubmeshOffset);
		}

		outSkinnedData.Set(
			boneHierarchy,
			boneOffsets);

		return true;
	}

	return false;
}

bool MeshReference::LoadAnimationFile(SkinnedData& outSkinnedData, std::string& path, const std::string clipName)
{
	path = path + "_" + clipName + ".anim";
	std::ifstream fileIn(path);

	AnimationClip animation;
	uint32_t boneAnimationSize, keyframeSize;

	std::string ignore;
	if (fileIn)
	{
		fileIn >> ignore >> boneAnimationSize;
		fileIn >> ignore >> keyframeSize;

		for (uint32_t i = 0; i < boneAnimationSize; ++i)
		{
			BoneAnimation boneAnim;
			for (uint32_t j = 0; j < keyframeSize; ++j)
			{
				Keyframe key;
				fileIn >> key.TimePos;
				fileIn >> key.Translation.x >> key.Translation.y >> key.Translation.z;
				fileIn >> key.Scale.x >> key.Scale.y >> key.Scale.z;
				fileIn >> key.RotationQuat.x >> key.RotationQuat.y >> key.RotationQuat.z >> key.RotationQuat.w;
				boneAnim.Keyframes.push_back(key);
			}
			animation.BoneAnimations.push_back(boneAnim);
		}

		outSkinnedData.SetAnimation(animation, clipName);
		return true;
	}

	return false;
}
