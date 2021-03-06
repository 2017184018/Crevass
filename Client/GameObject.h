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

public:
	explicit GameObject(std::string type, std::string id);
	virtual ~GameObject();

	std::string GetType() const { return m_Type; }
	UINT GetIndex() const { return m_Index; }

	virtual void Scale(float x, float y, float z);
	virtual void SetPosition(float posX, float posY, float posZ);
	virtual void SetPosition(DirectX::XMFLOAT3 xmPos);

	virtual void SetRight(const DirectX::XMFLOAT3& Right);
	virtual void SetUp(const DirectX::XMFLOAT3& Up);
	virtual void SetLook(const DirectX::XMFLOAT3& Look);

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetLook() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetRight() const;

	virtual void MoveStrafe(float fDistance = 1.0f);
	virtual void MoveUp(float fDistance = 1.0f);
	virtual void MoveForward(float fDistance = 1.0f);

	virtual void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);
	virtual void Move(const XMFLOAT3& xmf3Shift, bool bVelocity = false);

	virtual void Rotate(const DirectX::XMFLOAT3& axis, float angle);
	virtual void Rotate(const DirectX::XMFLOAT4& quaternion);
	virtual void Rotate(float pitch, float yaw, float roll);
//public:
//	virtual void	Initialize() {};
//	virtual void	Update() {};
//	virtual void	Release() {};

public:
	bool m_IsVisible = false;

	DirectX::XMFLOAT4X4 m_World;
	DirectX::XMFLOAT4X4 m_TexTransform;

//public:
//	std::vector<ShaderResource::InstanceData> Instances;
//	UINT InstanceCount;
	

public:
	//Material* Mat;

	GeometryMesh* Geo;
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType;
	UINT IndexCount;
	UINT StartIndexLocation;
	int BaseVertexLocation;
	UINT m_MaterialIndex;
	BoundingBox m_Bounds;
public:
	// Animation
	SkinnedModelInstance* m_SkinnedModelInst;


	// 뼈대 변환 상수 버퍼 인덱스
	// 스키닝용 렌더 항목에만 쓰인다
	UINT m_SkinnedCBIndex = -1;

};

