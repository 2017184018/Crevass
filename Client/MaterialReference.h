#pragma once
#include "Component.h"

class MaterialReference : public Component
{
public:
	enum MATERIALID { ID_BRICKS, ID_STONE, ID_TILE, ID_CRATE, ID_ICE, ID_GRASS, ID_SKULLMAT, ID_SKY };

public:
	explicit MaterialReference();
	virtual ~MaterialReference();

public:
	virtual HRESULT InitializeComponent() override;
	virtual void UpdateComponent(const float& fTimeDelta) override;
	virtual void ReleaseComponent() override;

public:
	void BuildMaterials();

public:
	std::unordered_map<int, std::unique_ptr<Material>>	m_Materials;
};

