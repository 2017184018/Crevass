#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"
#include "GameObject.h"
#include "ObjectInfo.h"


namespace Graphics
{
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
}

namespace Core
{
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
}

class MeshReference;
class MaterialReference;
class GameObject;
class CREVASS : public IGameApp, public TemplateSingleton<CREVASS>
{

public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;

	void OnKeyboardInput(const float deltaT);

	void shake(GameObject*, int);

private:
	void BuildScene();

	bool IsShake[25];		//흔들리는 상태인지
	bool IsRight[25];		//오른쪽으로 흔들리는지
	UINT ShakeCnt[25];		//좌우로 몇 번 움직였는지
	UINT DestructionCnt[25];	//몇 번 파괴 됐는지

public:
	template <class TObject>
	TObject* CreateObject(RenderLayer layer, std::string type, std::string instID)
	{
		GameObject* obj = new TObject(layer, type, instID);
		m_RItemsVec.push_back(obj);
		//중복이면 index++해서 info 수정
		//중복이 아니라면 info를 생성하여 m_RItemsMap에 add 
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
		//다이나믹 케스트를 왜했을까 
		return dynamic_cast<TObject*>(obj);

	}
private:
	Camera m_Camera;

public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;

public:
	std::map<std::string, ObjectInfo*> m_RItemsMap;
	std::vector<GameObject*> m_RItemsVec;


	std::vector<GameObject*>	m_AllRItems;
	std::vector<GameObject*>	m_RitemLayer[static_cast<int>(RenderLayer::ID_COUNT)];
	UINT						m_InstanceCount = 0;
};

