#include "pch.h"
#include "MaterialReference.h"

MaterialReference::MaterialReference()
{
}

MaterialReference::~MaterialReference()
{
}



void MaterialReference::BuildMaterials()
{
	
	auto sky = std::make_unique<Material>();
	sky->MatCBIndex = 0;
	sky->DiffuseSrvHeapIndex = 0;
	sky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky->Roughness = 1.0f;

	auto ice = std::make_unique<Material>();
	ice->MatCBIndex = 1;
	ice->DiffuseSrvHeapIndex = 1;
	ice->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ice->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	ice->Roughness = 1.0f;

	m_Materials["snowcube1024"] = std::move(sky);
	m_Materials["ice"] = std::move(ice);
}
