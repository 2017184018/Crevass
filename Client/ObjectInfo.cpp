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
		return -1;
	}

	return m_InstanceKeymap[instID];
}

