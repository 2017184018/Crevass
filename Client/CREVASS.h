#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"
//#include "GameObject.h"
//#include "ObjectInfo.h"
#include "Waves.h"
//#include "BlurFilter.h"
//#include "MainFramework.h"


//namespace Graphics
//{
//	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedPSO;
//	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
//	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_BB;
//
//	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> HorBlur;
//	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> VerBlur;
//}

namespace Core
{
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;

	//extern std::unique_ptr<BlurFilter> mBlurFilter;
	//extern ID3D12Resource* BackBuffer;
	//extern ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

	//extern MainFramework* g_pFramework;
}

class SceneManager;
class MeshReference;
class MaterialReference;
class Character;
class CREVASS : public IGameApp, public TemplateSingleton<CREVASS>
{

public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;


private:
	void BuildScene();

	//bool IsShake[25];		//흔들리는 상태인지
	//bool IsRight[25];		//오른쪽으로 흔들리는지
	//UINT ShakeCnt[25];		//좌우로 몇 번 움직였는지
	//UINT DestructionCnt[25];	//몇 번 파괴 됐는지
	//void shake(GameObject*, int);

	//bool BlockCheck(int);		//부숴지는 블록인지 아닌지 체크

	//bool IsDown[25];		//내려가는 중인지

	//UINT SnowmanIndex[2];

	std::unique_ptr<Waves> mWaves;

	//UINT SnowmanLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };

	//bool IsFall = false;		//빠졌는지

	//UINT BlurCnt = 0;
public:
	Camera* m_Camera;

public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;
	SceneManager* m_SceneManager;

};

