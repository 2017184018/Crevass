#include "pch.h"
#include "MaterialReference.h"

MaterialReference::MaterialReference()
{
}

MaterialReference::~MaterialReference()
{
}

void MaterialReference::Update(float t) {
	auto waterMat = m_Materials["water"].get();

	float& tu = waterMat->MatTransform(3, 0);
	float& tv = waterMat->MatTransform(3, 1);

	tu += 0.04f * t;
	tv += 0.04f * t;

	if (tu >= 1.0f)
		tu -= 1.0f;

	if (tv >= 1.0f)
		tv -= 1.0f;

	waterMat->MatTransform(3, 0) = tu;
	waterMat->MatTransform(3, 1) = tv;
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

	auto Penguin = std::make_unique<Material>();
	Penguin->MatCBIndex = 2;
	Penguin->DiffuseSrvHeapIndex = 2;
	Penguin->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Penguin->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	Penguin->Roughness = 1.0f;

	auto water = std::make_unique<Material>();
	water->MatCBIndex = 3;
	water->DiffuseSrvHeapIndex = 3;
	water->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	water->FresnelR0 = XMFLOAT3(0.2f, 0.2f, 0.2f);
	water->Roughness = 0.0f;

	auto husky = std::make_unique<Material>();
	husky->MatCBIndex = 4;
	husky->DiffuseSrvHeapIndex = 4;
	husky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	husky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	husky->Roughness = 1.0f;

	auto heart = std::make_unique<Material>();
	heart->MatCBIndex = 5;
	heart->DiffuseSrvHeapIndex = 5;
	heart->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	heart->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	heart->Roughness = 1.0f;

	auto heartline = std::make_unique<Material>();
	heartline->MatCBIndex = 6;
	heartline->DiffuseSrvHeapIndex = 6;
	heartline->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	heartline->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	heartline->Roughness = 1.0f;

	auto rope = std::make_unique<Material>();
	rope->MatCBIndex = 7;
	rope->DiffuseSrvHeapIndex = 7;
	rope->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	rope->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);

	heartline->Roughness = 1.0f;
	m_Materials["snowcube1024"] = std::move(sky);
	m_Materials["ice"] = std::move(ice);
	m_Materials["Penguin"] = std::move(Penguin);
	m_Materials["water"] = std::move(water);
	m_Materials["husky"] = std::move(husky);
	m_Materials["heart"] = std::move(heart);
	m_Materials["heartline"] = std::move(heartline);
	m_Materials["rope"] = std::move(rope);
	
}
