#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjectInfo.h"
#include "Waves.h"
#include "BlurFilter.h"


namespace Graphics
{
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_BB;

	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> HorBlur;
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> VerBlur;
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
}

class MeshReference;
class MaterialReference;
class Character;
class GameObject;
class CREVASS : public IGameApp, public TemplateSingleton<CREVASS>
{

public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;

	void OnKeyboardInput(const float deltaT);


	GameObject* wave;

private:
	void BuildScene();

	bool IsShake[25];		//��鸮�� ��������
	bool IsRight[25];		//���������� ��鸮����
	UINT ShakeCnt[25];		//�¿�� �� �� ����������
	UINT DestructionCnt[25];	//�� �� �ı� �ƴ���
	void shake(GameObject*, int);

	bool BlockCheck(int);		//�ν����� ������� �ƴ��� üũ

	bool IsDown[25];		//�������� ������

	UINT SnowmanIndex[2];

	std::unique_ptr<Waves> mWaves;

	UINT SnowmanLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };

	bool IsFall = false;		//��������

	UINT BlurCnt = 0;
public:
	template <class TObject>
	TObject* CreateObject(RenderLayer layer, std::string type, std::string instID)
	{
		GameObject* obj = new TObject(layer, type, instID);
		m_RItemsVec.push_back(obj);
		//�ߺ��̸� index++�ؼ� info ����
		//�ߺ��� �ƴ϶�� info�� �����Ͽ� m_RItemsMap�� add 
		if (m_RItemsMap.find(type) != m_RItemsMap.end()) {
			cout << "create instance" << endl;
			//type , obinfo   objinfo = ((string)instID, (int)instindex)

			m_RItemsMap[type]->AddInstance(instID, obj->GetIndex());

		}
		else {
			ObjectInfo* objInfo = new ObjectInfo(instID, obj->GetIndex());
			objInfo->m_Type = type;
			m_RItemsMap[type] = objInfo;

		}
		//���̳��� �ɽ�Ʈ�� �������� 
		return dynamic_cast<TObject*>(obj);

	}
	template <class TObject>
	TObject* FindObject(std::string type, std::string instID)
	{
		if (!m_RItemsMap.count(type)) {
			cout << "Error! None Type" << endl;
			return nullptr;
		}

		if (m_RItemsMap[type]->GetInstanceIndex(instID) == -1)
			return nullptr;
		return dynamic_cast<TObject*>(m_RItemsVec[m_RItemsMap[type]->GetInstanceIndex(instID)]);
	}

public:
	Camera* m_Camera;

public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;

public:
	std::map<std::string, ObjectInfo*> m_RItemsMap;
	std::vector<GameObject*> m_RItemsVec;

	std::vector<GameObject*>	m_AllRItems;
	std::vector<GameObject*>	m_RitemLayer[static_cast<int>(RenderLayer::ID_COUNT)];
	UINT						m_InstanceCount = 0;
	
private:
	/* ID: battleID */
	std::map<int, Character*> m_Users;
	int m_PlayerID;
};

