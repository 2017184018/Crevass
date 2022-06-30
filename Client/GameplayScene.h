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
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> 	g_DebugPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_UIPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_BB;

	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> HorBlur;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> VerBlur;

	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_ShadowOpaquePSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedShadowOpaquePSO;

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
	extern GameObject* wave;
	extern UINT iglooIndex[2];
	extern UINT SnowmanIndex[2];
	extern MainFramework* g_pFramework;
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
	virtual void RenderUI() override;
	/* ID: battleID */
	std::map<int, Character*> m_Users;
	int m_PlayerID;

//	bool IsShake[25];
	//bool IsRight[25];		//���������� ��鸮����
	//UINT ShakeCnt[25];		//�¿�� �� �� ����������
	UINT DestructionCnt[25];	//�� �� �ı� �ƴ���

	//bool IsDown[25];		//�������� ������


	UINT iglooLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };

	bool IsFall[3] = { 0, };		// ����, ���� ����

	bool BlockIn = false;		//��� �ѹ��� ���
	int tmp1 = -1;
	int tmp2 = -1;

	void Fall(int num);
	UINT Lifecnt = 5;

	int FallX, FallZ;
	float Gravity = 0.1;

	int m_Timer;
};

