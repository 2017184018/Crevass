#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "ObjectInfo.h"
#include "MainFramework.h"
#include "Network.h"

namespace Core {
	extern GameObject* wave[2];
	extern UINT iglooIndex[2];
	extern MainFramework* g_pFramework;
	extern	 XMFLOAT3 MinimapCubePos[25];
	extern XMFLOAT3 MinimapBlockScale;

}

class Character;
class ApplicationContext : public TemplateSingleton<ApplicationContext>
{
public:
	template <class TObject>
	TObject* CreateObject(std::string type, std::string instID)
	{
		// �� �ؾ��Ұ͵�
		// �ڽ��� Index�� ������
		GameObject* obj = new TObject(type, instID);
		m_RItemsVec.push_back(obj);

		// Map ����
		// Map �ߺ�üũ
		if (m_RItemsMap.find(type) != m_RItemsMap.end())
		{
			// Instancemap ����
			m_RItemsMap[type]->AddInstance(instID, obj->GetIndex());
		}
		else
		{
			// ���ο� Ÿ��(�޽�)�� �߰���
			// -> ObjInfo ����
			ObjectInfo* objInfo = new ObjectInfo(instID, obj->GetIndex());
			objInfo->m_Type = type;
			m_RItemsMap[type] = objInfo;
		}

		return dynamic_cast<TObject*>(obj);
	}

	template <class TObject>
	TObject* FindObject(std::string type, std::string instID)
	{
		if (!m_RItemsMap.count(type)) {
			return nullptr;
		}

		if (m_RItemsMap[type]->GetInstanceIndex(instID) == -1)
			return nullptr;
		return dynamic_cast<TObject*>(m_RItemsVec[m_RItemsMap[type]->GetInstanceIndex(instID)]);
	}

public:
	std::string FindAnimName(int animCode) const;

	void CreateLobby();

	void CreateSkycube(std::string skycubeName, std::string instID, std::string matName);
	void CreateDebugBoundingBox(std::string boundsName, std::string boundsInstName);
	void CreateBlocks();
	void Createigloos();
	void CreateSnowmans();
	void CreateWave();
	void CreateCharacter(std::string meshName, std::string instID, std::string matName, int skinnedCBIndex /*Character ����(����)*/);
	void CreateBackground();
	void CreateHail();
	void CreateWaterDrop();
	void CreateMinimap();
	void CreateOutline();
	void CreateUI2D(std::string ui2dLayer, std::string ui2dName, int matIndex);
	void CreateParticle(std::string particleName, std::string instID, std::string matName, bool isLoop);
	void CreatelobbyBlocks();
	void CreatelobbyBackground();


	void DisplayOutline();
	void DisplayMinimap();
	void DisplaySnowmans();
	void DisplayWaterDrop();
	void DisplayHail();
	void Displayigloos();
	void DisplayBlocks();
	void DisplayBackground();
	void HiddenOutline();
	void HiddenMinimap();
	void HiddenSnowmans();
	void HiddenWaterDrop();
	void HiddenHail();
	void Hiddenigloos();
	void HiddenBlocks();
	void HiddenBackground();

	 
	void DisplaylobbyBackground();
	void DisplaylobbyBlocks();
	void HiddenlobbyBackground();
	void HiddenlobbyBlocks();

	void DisplayCharacter(Character* user, XMFLOAT3 pos, bool isVisible);
	void DisplayCharacterSeal(Character* user, XMFLOAT3 pos, bool isVisible);
	void DisplayCharacter(std::string mapName, std::string userName, int spawnLocation, bool isVisible = true);
	void HiddenCharacter(Character* user);
	void HiddenCharacter(std::string userName, std::string insname);

	void DisplayUI(std::string ui2dLayer, std::string ui2dName, float posX, float posY, float sizeX, float sizeY);
	void DisplayUI3D(std::string ui2dLayer, std::string ui2dName, float posX, float posY, float sizeX, float sizeY);
	void HiddenUI(std::string uiName, std::string instname);

	void SetUI2DPosition(std::string ui2dName, float posX, float posY);

	void DisplayParticle(std::string particleName, std::string instID, DirectX::XMFLOAT3 pos);
	void HiddenParticle(std::string particleName, std::string instID);

	bool BlockCheck(int idx);
public:
	//void CreateDebugBoundingBox(std::string boundsName, std::string boundsInstName);

	UINT iglooLocaArray[9] = { 0,2,4,10,12,14,20,22,24 };
public:
	std::map<std::string, ObjectInfo*> m_RItemsMap;
	std::vector<GameObject*> m_RItemsVec;

};
