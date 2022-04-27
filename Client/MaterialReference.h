#pragma once
#include "Singleton.h"
class MaterialReference final : public TemplateSingleton<MaterialReference>
{
public:
	explicit MaterialReference();
	virtual ~MaterialReference();

	void Update(float t);
public:
	void BuildMaterials();

public:
	std::unordered_map<std::string, std::unique_ptr<Material>>	m_Materials;
};

