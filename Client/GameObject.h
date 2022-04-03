#pragma once

class SkinnedModelInstance;
class GeometryMesh;
class GameObject
{

protected:
	
	//메쉬 타입
	std::string m_Type;
	//오브젝트 id 이름이라 생각해도됨
	std::string m_ID;
	//몇번째 오브젝트 
	UINT m_Index;
	//pso따라 구분
	RenderLayer m_Layer;

public:
	explicit GameObject(RenderLayer layer, std::string type, std::string id);
	virtual ~GameObject();

	std::string GetType() const { return m_Type; }
	UINT GetIndex() const { return m_Index; }

	virtual void Scale(float x, float y, float z);
	virtual void SetPosition(float posX, float posY, float posZ);

	virtual void Rotate(const DirectX::XMFLOAT3& axis, float angle);
	virtual void Rotate(const DirectX::XMFLOAT4& quaternion);
	virtual void Rotate(float pitch, float yaw, float roll);
//public:
//	virtual void	Initialize() {};
//	virtual void	Update() {};
//	virtual void	Release() {};

public:
	DirectX::XMFLOAT4X4 m_World;
	DirectX::XMFLOAT4X4 m_TexTransform;

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

public:
	// Animation
	SkinnedModelInstance* m_SkinnedModelInst;
	// 뼈대 변환 상수 버퍼 인덱스
	// 스키닝용 렌더 항목에만 쓰인다
	UINT m_SkinnedCBIndex = -1;

};

