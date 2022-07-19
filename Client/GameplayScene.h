#pragma once
#include "Scene.h"
#include "BlurFilter.h"
#include "Waves.h"
#include "GameObject.h"
#include "MainFramework.h"
#include "Network.h"

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
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;

	extern std::unique_ptr<BlurFilter> mBlurFilter;
	extern ID3D12Resource* BackBuffer;
	extern ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

	extern std::unique_ptr<Waves> mWaves;
	extern GameObject* wave[2];
	extern UINT iglooIndex[2];
	extern UINT SnowmanIndex[2];
	extern MainFramework* g_pFramework;
	extern D3D12_VIEWPORT mMinimapViewport;
	extern D3D12_RECT mMinimapScissorRect;
	extern D3D12_VIEWPORT mScreenViewport;
	extern D3D12_RECT mScissorRect;
	extern XMFLOAT3 MinimapCubePos[25];
	extern XMFLOAT3 MinimapBlockScale;

	extern int m_PlayerID;
}

class Character;
class GameplayScene : public Scene
{
public:
	virtual void Initialize() override;
	virtual void OnResize() override;

public:
	virtual bool Enter() override;
	virtual void Exit() override;

	virtual void Update(const float& fDeltaTime) override;
	virtual void Render() override;
	float distance(XMFLOAT3 a, XMFLOAT3 b);
	virtual void RenderUI() override;
	/* ID: battleID */
	std::map<int, Character*> m_Users;

//	bool IsShake[25];
	//bool IsRight[25];		//오른쪽으로 흔들리는지
	//UINT ShakeCnt[25];		//좌우로 몇 번 움직였는지
	UINT DestructionCnt[25];	//몇 번 파괴 됐는지

	//bool IsDown[25];		//내려가는 중인지


	UINT iglooLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };

	bool IsFall[3] = { 0, };		// 블러링, 높은 물결

	bool BlockIn = false;		//블록 한번만 밟게
	int tmp1 = -1;
	int tmp2 = -1;

	void Fall(int num);
	UINT Player_Lifecnt[5] = {0,1,2,3,4};

	int FallX, FallZ;
	float Gravity = 0.1;

	int m_Timer;
};

