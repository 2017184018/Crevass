#pragma once
#include "Singleton.h"
#include "UploadBuffer.h"

class GameObject;
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
	void UpdateInstanceData(std::vector<GameObject*>& rItems);
	void UpdateMaterialBuffer(std::unordered_map<int, std::unique_ptr<Material>>& materials);
	void UpdateMainPassCB(Camera& camera);
	void DrawRenderItems(const std::vector<GameObject*>& ritems);

	void SetPipelineState(ID3D12PipelineState* PSO);
	void SetGraphicsRootSignature(ID3D12RootSignature* RootSignature);

	ID3D12PipelineState* GetPipelineState() { return m_CurPipelineState; };

public:
	ShaderResource::PassConstants mMainPassCB;
	std::unique_ptr<UploadBuffer<ShaderResource::PassConstants>>	PassCB = nullptr;
	/* map<string, UploadBuffer-InstanceData>*/
	std::unique_ptr<UploadBuffer<ShaderResource::InstanceData>>		InstanceBuffer = nullptr;
	std::unique_ptr<UploadBuffer<ShaderResource::MaterialData>>		MaterialBuffer = nullptr;

	UINT passCount; UINT objectCount; UINT InstanceCount; UINT materialCount;
};