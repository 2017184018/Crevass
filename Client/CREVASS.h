#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"
//#include "GameObject.h"
//#include "ObjectInfo.h"
#include "Waves.h"
//#include "BlurFilter.h"
#include "MainFramework.h"


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

	extern std::unique_ptr<Waves> mWaves;
	//extern std::unique_ptr<BlurFilter> mBlurFilter;
	//extern ID3D12Resource* BackBuffer;
	//extern ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

	extern MainFramework* g_pFramework;

	extern bool Inactive;
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
	void BuildCharacters();

	//bool IsShake[25];		//��鸮�� ��������
	//bool IsRight[25];		//���������� ��鸮����
	//UINT ShakeCnt[25];		//�¿�� �� �� ����������
	//UINT DestructionCnt[25];	//�� �� �ı� �ƴ���
	//void shake(GameObject*, int);

	//bool BlockCheck(int);		//�ν����� �������� �ƴ��� üũ

	//bool IsDown[25];		//�������� ������

	//UINT SnowmanIndex[2];

	//std::unique_ptr<Waves> mWaves;

	//UINT SnowmanLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };

	//bool IsFall = false;		//��������

	//UINT BlurCnt = 0;
public:
	Camera* m_Camera;

public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;
	SceneManager* m_SceneManager;

};

