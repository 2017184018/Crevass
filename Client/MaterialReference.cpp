#include "pch.h"
#include "MaterialReference.h"

MaterialReference::MaterialReference()
{
}

MaterialReference::~MaterialReference()
{
	this->ReleaseComponent();
}

HRESULT MaterialReference::InitializeComponent()
{
	return S_OK;
}

void MaterialReference::UpdateComponent(const float& fTimeDelta)
{
}

void MaterialReference::ReleaseComponent()
{
}

void MaterialReference::BuildMaterials()
{
	auto bricks0 = std::make_unique<Material>();
	bricks0->MatCBIndex = MaterialReference::MATERIALID::ID_BRICKS;
	bricks0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_BRICKS;
	bricks0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	bricks0->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	bricks0->Roughness = 0.1f;

	auto stone0 = std::make_unique<Material>();
	stone0->MatCBIndex = MaterialReference::MATERIALID::ID_STONE;
	stone0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_STONE;
	stone0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	stone0->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	stone0->Roughness = 0.3f;

	auto tile0 = std::make_unique<Material>();
	tile0->MatCBIndex = MaterialReference::MATERIALID::ID_TILE;
	tile0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_TILE;
	tile0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	tile0->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	tile0->Roughness = 0.3f;

	auto crate0 = std::make_unique<Material>();
	crate0->MatCBIndex = MaterialReference::MATERIALID::ID_CRATE;
	crate0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_CRATE;
	crate0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	crate0->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	crate0->Roughness = 0.2f;

	auto ice0 = std::make_unique<Material>();
	ice0->MatCBIndex = MaterialReference::MATERIALID::ID_ICE;
	ice0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_ICE;
	ice0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ice0->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	ice0->Roughness = 0.0f;

	auto grass0 = std::make_unique<Material>();
	grass0->MatCBIndex = MaterialReference::MATERIALID::ID_GRASS;
	grass0->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_GRASS;
	grass0->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	grass0->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	grass0->Roughness = 0.2f;

	auto skullMat = std::make_unique<Material>();
	skullMat->MatCBIndex = MaterialReference::MATERIALID::ID_SKULLMAT;
	skullMat->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKULLMAT;
	skullMat->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	skullMat->FresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	skullMat->Roughness = 0.5f;

	auto sky = std::make_unique<Material>();
	sky->MatCBIndex = MaterialReference::MATERIALID::ID_SKY;
	sky->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY;
	sky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky->Roughness = 1.0f;

	auto sky2 = std::make_unique<Material>();
	sky2->MatCBIndex = MaterialReference::MATERIALID::ID_SKY2;
	sky2->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY2;
	sky2->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky2->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky2->Roughness = 1.0f;

	auto sky3 = std::make_unique<Material>();
	sky3->MatCBIndex = MaterialReference::MATERIALID::ID_SKY3;
	sky3->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY3;
	sky3->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky3->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky3->Roughness = 1.0f;

	auto sky4 = std::make_unique<Material>();
	sky4->MatCBIndex = MaterialReference::MATERIALID::ID_SKY4;
	sky4->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY4;
	sky4->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky4->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky4->Roughness = 1.0f;

	auto sky5 = std::make_unique<Material>();
	sky5->MatCBIndex = MaterialReference::MATERIALID::ID_SKY5;
	sky5->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY5;
	sky5->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky5->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky5->Roughness = 1.0f;

	auto sky6 = std::make_unique<Material>();
	sky6->MatCBIndex = MaterialReference::MATERIALID::ID_SKY6;
	sky6->DiffuseSrvHeapIndex = MaterialReference::MATERIALID::ID_SKY6;
	sky6->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky6->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky6->Roughness = 1.0f;

	m_Materials[MATERIALID::ID_BRICKS] = std::move(bricks0);
	m_Materials[MATERIALID::ID_STONE] = std::move(stone0);
	m_Materials[MATERIALID::ID_TILE] = std::move(tile0);
	m_Materials[MATERIALID::ID_CRATE] = std::move(crate0);
	m_Materials[MATERIALID::ID_ICE] = std::move(ice0);
	m_Materials[MATERIALID::ID_GRASS] = std::move(grass0);
	m_Materials[MATERIALID::ID_SKULLMAT] = std::move(skullMat);
	m_Materials[MATERIALID::ID_SKY] = std::move(sky);
	m_Materials[MATERIALID::ID_SKY2] = std::move(sky2);
	m_Materials[MATERIALID::ID_SKY3] = std::move(sky3);
	m_Materials[MATERIALID::ID_SKY4] = std::move(sky4);
	m_Materials[MATERIALID::ID_SKY5] = std::move(sky5);
	m_Materials[MATERIALID::ID_SKY6] = std::move(sky6);
}
