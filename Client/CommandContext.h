#pragma once
#include "Singleton.h"
#include "UploadBuffer.h"
#include "Waves.h"


class SkinnedModelInstance;
class GameObject;
class ObjectInfo;
class Camera;
class GameTimer;

namespace Core
{
	extern GameTimer* g_GameTimer;

	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
}

class CommandContext abstract
{
protected:
	void Reset(void);

public:
	virtual void Release() = 0;

protected:
	ID3D12RootSignature* m_CurGraphicsRootSignature;
	ID3D12PipelineState* m_CurPipelineState;
	ID3D12RootSignature* m_CurComputeRootSignature;
};

class GraphicsContext : public CommandContext, public TemplateSingleton<GraphicsContext>
{
public:
	void Initialize();
	virtual void Release() override;

public:
	void BuildInstanceBuffer(ObjectInfo* objInfo);

	void UpdateInstanceData(ObjectInfo* objInfo, std::vector<GameObject*>& rItems);
	void UpdateInstanceDatas(std::vector<ObjectInfo*>& objInfos, std::vector<GameObject*>& rItems);
	
	void UpdateMaterialBuffer(std::unordered_map<std::string, std::unique_ptr<Material>>& materials);
	void UpdateMainPassCB(Camera& camera);
	void UpdateWave(Waves* wave, GameObject *waveobject);

	void UpdateSkinnedCBs(UINT skinnedCBIndex, SkinnedModelInstance* skinmodelInstance);

	void UpdateShadowPassCB();
	void UpdateShadowTransform();


	void DrawRenderItems(ObjectInfo* objInfo, const std::vector<GameObject*>& rItems);

	void SetResourceShadowPassCB();
	void ShadowTransitionResourceBarrier();

	void SetPipelineState(ID3D12PipelineState* PSO);
	void SetGraphicsRootSignature(ID3D12RootSignature* RootSignature);

	ID3D12PipelineState* GetPipelineState() { return m_CurPipelineState; };

public:
	ShaderResource::PassConstants mMainPassCB;
	ShaderResource::PassConstants mShadowPassCB;

	std::unique_ptr<UploadBuffer<ShaderResource::PassConstants>>	PassCB = nullptr;
	std::map<string, std::unique_ptr<UploadBuffer<ShaderResource::InstanceData>>> m_InstanceBuffers;
	std::array< std::unique_ptr<UploadBuffer<ShaderResource::SkinnedConstants>>, BoneIndex::Count> m_SkinnedCBs;
	std::unique_ptr<UploadBuffer<ShaderResource::MaterialData>>		MaterialBuffer = nullptr;

	UINT passCount; UINT materialCount; UINT skinnedObjectCount;
	std::unique_ptr<UploadBuffer<Vertex>> WavesVB = nullptr;
	UINT VertexCount;

private:
	DirectX::BoundingSphere mSceneBounds;

	// ???? Light????
	float mLightNearZ = 0.0f;
	float mLightFarZ = 0.0f;
	XMFLOAT3 mLightPosW;
	XMFLOAT4X4 mLightView = MathHelper::Identity4x4();
	XMFLOAT4X4 mLightProj = MathHelper::Identity4x4();
	XMFLOAT4X4 mShadowTransform = MathHelper::Identity4x4();

	float mLightRotationAngle = 0.0f;
	XMFLOAT3 mBaseLightDirections[3] = {
		XMFLOAT3(0.57735f, -0.57735f, 0.57735f),
		XMFLOAT3(-0.57735f, -0.57735f, 0.57735f),
		XMFLOAT3(0.0f, -0.707f, -0.707f)
	};
	XMFLOAT3 mRotatedLightDirections[3];

};