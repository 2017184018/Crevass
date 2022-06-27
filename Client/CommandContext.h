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

	extern Microsoft::WRL::ComPtr<ID2D1DeviceContext2> g_D2dDeviceContext;
	extern Microsoft::WRL::ComPtr<IDWriteFactory> g_DWriteFactory;

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

struct UIInfo
{
	XMFLOAT2 pos;
	XMFLOAT2 size;
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
	void UpdateMainPassCB(Camera& camera, Light* light);
	void UpdateWave(Waves* wave, GameObject *waveobject);

	void UpdateSkinnedCBs(UINT skinnedCBIndex, SkinnedModelInstance* skinmodelInstance);

	void UpdateShadowPassCB();
	void UpdateShadowTransform(Light* light, DirectX::BoundingSphere sceneBounds);


	void DrawRenderItems(ObjectInfo* objInfo, const std::vector<GameObject*>& rItems);

	void SetResourceShadowPassCB();
	void ShadowTransitionResourceBarrier();

	void SetPipelineState(ID3D12PipelineState* PSO);
	void SetGraphicsRootSignature(ID3D12RootSignature* RootSignature);

	ID3D12PipelineState* GetPipelineState() { return m_CurPipelineState; };

	void OnBlurEffect(bool onoff);
	void OnResizeBlur();

public:
	void LoadFont(const wstring fontName, float fontSize);

	// Dont use this method often.
	// This is function that resets and recreates textformat.
	void SetTextSize(float fontSize);
	void SetTextSize(float fontSize, DWRITE_TEXT_ALIGNMENT textType, D2D1::ColorF color);
	void SetTextAlignMent(DWRITE_TEXT_ALIGNMENT textAlignment);
	void DrawD2DText(const wstring text, float posX, float posY, float rectSize);
	void DrawD2DText(const wstring text, float posX, float posY, float rectSizeX, float rectSizeY, bool aligmentLeading);

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
	wstring m_FontName;
	float	m_FontSize;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_TextBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_TextFormat;

private:
	// 임시 Light변수
	float mLightNearZ = 0.0f;
	float mLightFarZ = 0.0f;
	XMFLOAT3 mLightPosW;
	XMFLOAT4X4 mLightView = MathHelper::Identity4x4();
	XMFLOAT4X4 mLightProj = MathHelper::Identity4x4();
	XMFLOAT4X4 mShadowTransform = MathHelper::Identity4x4();

	float mLightRotationAngle = 0.0f;
	XMFLOAT3 mRotatedLightDirections[3];

};