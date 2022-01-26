#pragma once

class ObjectInfo
{
private:
	
	std::map<std::string, UINT> m_InstanceKeymap;

public:
	ObjectInfo(std::string instID, UINT instIndex);
	~ObjectInfo() = default;

	void AddInstance(std::string instID, UINT instIndex);
	UINT GetInstanceIndex(std::string instID);

	std::map<std::string, UINT>& GetinstanceKeymap() { return m_InstanceKeymap; };

public:
	//geo е╦ют
	std::string m_Type;

	UINT m_InstanceCount;
	UINT m_LifeCount;
};

