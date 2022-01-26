#include "pch.h"
#include "ObjectInfo.h"

ObjectInfo::ObjectInfo(std::string instID, UINT instIndex) :
	m_InstanceCount(0),
	m_LifeCount(0)
{
	AddInstance(instID, instIndex);
}

void ObjectInfo::AddInstance(std::string instID, UINT instIndex)
{
	if (m_InstanceKeymap.find(instID) != m_InstanceKeymap.end()) {
		cout << "해당 ID 인스턴스가 존재함" << endl;
	}
	else
	{
		m_InstanceKeymap[instID] = instIndex;

		m_InstanceCount += 1;
		m_LifeCount += 1;
	}
}

UINT ObjectInfo::GetInstanceIndex(std::string instID)
{
	if (!m_InstanceKeymap.count(instID)) {
		cout << "없는 key입니다." << endl;
		return -1;
	}

	return m_InstanceKeymap[instID];
}

