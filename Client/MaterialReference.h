#pragma once

class MaterialReference final
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

