#pragma once
#include "Singleton.h"
#include "Camera.h"
#include "UploadBuffer.h"
#include "ShadowMap.h"
#include "BlurFilter.h"

class GameObject;
class GameTimer;

namespace Core
{
	extern GameTimer* g_GameTimer;

	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_CommandQueue;
	extern Microsoft::WRL::ComPtr<ID3D12CommandAllocator> g_DirectCmdListAlloc;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
	extern DXGI_FORMAT g_BackBufferFormat;
	extern DXGI_FORMAT g_DepthStencilFormat;

	extern bool      g_4xMsaaState;			// 4X MSAA enabled
	extern UINT      g_4xMsaaQuality;      // quality level of 4X MSAA

	extern std::unique_ptr<BlurFilter> mBlurFilter;
	extern ID3D12Resource* BackBuffer;
	extern ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

}

class GraphicsRenderer : public TemplateSingleton<GraphicsRenderer>
{
public:
	void Initialize();
	void Shutdown();

public:
	void RenderGraphics();

	void ExecuteBlurEffects();
	void ExecuteResizeBlur();
private:
	void LoadTextures();
	void BuildDescriptorHeaps();
	void BuildShaderAndInputLayout();
	void BuildRootSignatures();
	void BuildPipelineStateObjects();

	void BuildPostProcessRootSignature();

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();

public:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RenderRS;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> m_Shaders;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_Instancing_InputLayout;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_Billboard_InputLayout;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_Skinned_InputLayout;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_UI_InputLayout;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_BB_InputLayout;

public:
	/* GpuResource */
	std::unique_ptr<ShadowMap> mShadowMap;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SrvDescriptorHeap = nullptr;

	UINT mSkyTexHeapIndex = 0;
	UINT mShadowMapHeapIndex = 0;
	UINT mBlurHeapIndex = 0;
public:
	bool	m_SwitchBlur = false;
};

