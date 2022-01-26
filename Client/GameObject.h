#pragma once

class GeometryMesh;
class GameObject
{

protected:
	
	//�޽� Ÿ��
	std::string m_Type;
	//������Ʈ id �̸��̶� �����ص���
	std::string m_ID;
	//���° ������Ʈ 
	UINT m_Index;
	//pso���� ����
	RenderLayer m_Layer;

public:
	explicit GameObject(RenderLayer layer, std::string type, std::string id);
	virtual ~GameObject();

	std::string GetType() const { return m_Type; }
	UINT GetIndex() const { return m_Index; }
//public:
//	virtual void	Initialize() {};
//	virtual void	Update() {};
//	virtual void	Release() {};

public:
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 TexTransform;

//public:
//	std::vector<ShaderResource::InstanceData> Instances;
//	UINT InstanceCount;
//	DirectX::BoundingBox Bounds;

public:
	//Material* Mat;

	GeometryMesh* Geo;
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType;
	UINT IndexCount;
	UINT StartIndexLocation;
	int BaseVertexLocation;

	UINT m_MaterialIndex;
};

