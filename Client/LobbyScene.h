#pragma once
#include "Scene.h"
#include "Waves.h"
#include "GameObject.h"
#include "MainFramework.h"
namespace Graphics
{
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_OpaquePSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_OutlinePSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> 	g_DebugPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_UIPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_BB;

	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> HorBlur;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> VerBlur;

	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_ShadowOpaquePSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedShadowOpaquePSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_ParticlePSO;


}

namespace Core
{
	extern std::unique_ptr<Waves> mWaves;
	extern GameObject* wave[2];
	extern int m_PlayerID;
	extern MainFramework* g_pFramework;
}

class LobbyScene : public Scene
{
public:
	virtual void Initialize() override;
	virtual void OnResize() override;
public:
	virtual bool Enter() override;
	virtual void Exit() override;

	virtual void Update(const float& fDeltaTime) override;
	virtual void Render() override;
	virtual void RenderUI() override;

	void Shake3DUI();

public:
	BOOL m_3DUIdir[2] = {0,0};
};

