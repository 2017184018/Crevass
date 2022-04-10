#pragma once

class SkinnedModelInstance;
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

	virtual void Scale(float x, float y, float z);
	virtual void SetPosition(float posX, float posY, float posZ);
	virtual void SetPosition(DirectX::XMFLOAT3 xmPos);

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
	// ���� ��ȯ ��� ���� �ε���
	// ��Ű�׿� ���� �׸񿡸� ���δ�
	UINT m_SkinnedCBIndex = -1;

};

