#pragma once

class GeometryMesh;
class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

public:
	virtual void	Initialize() {};
	virtual void	Update() {};
	virtual void	Release() {};

public:
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 TexTransform;

public:
	std::vector<ShaderResource::InstanceData> Instances;
	UINT InstanceCount;
	DirectX::BoundingBox Bounds;

public:
	Material* Mat;

	GeometryMesh* Geo;
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType;
	UINT IndexCount;
	UINT StartIndexLocation;
	int BaseVertexLocation;
};

