#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "ObjectInfo.h"

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
			cout << "Create Instance!" << endl;
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
			cout << "Error! None Type" << endl;
			return nullptr;
		}

		if (m_RItemsMap[type]->GetInstanceIndex(instID) == -1)
			return nullptr;
		return dynamic_cast<TObject*>(m_RItemsVec[m_RItemsMap[type]->GetInstanceIndex(instID)]);
	}

public:
	std::string FindAnimName(int animCode) const;
	
	void CreateSkycube(std::string skycubeName, std::string instID, std::string matName);
	void CreateBlocks();
	void CreateWave();
	void CreateCharacter(std::string meshName, std::string instID, std::string matName, int skinnedCBIndex /*Character ����(����)*/);

	void DisplayProps(std::string mapName);
	void HiddenBlocks();

	void DisplayCharacter(std::string mapName, Character* user, int spawnLocation, bool isVisible = true);
	void DisplayCharacter(std::string mapName, std::string userName, int spawnLocation, bool isVisible = true);
	void HiddenCharacter(Character* user);
	void HiddenCharacter(std::string userName);

	//void DisplayUI(std::string mapName);
	//void HiddenUI(std::string mapName);

public:
	//void CreateDebugBoundingBox(std::string boundsName, std::string boundsInstName);

public:
	std::map<std::string, ObjectInfo*> m_RItemsMap;
	std::vector<GameObject*> m_RItemsVec;

};
