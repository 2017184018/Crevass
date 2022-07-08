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

	auto waterdropMat = m_Materials["waterdrop"].get();

	tu = waterdropMat->MatTransform(3, 0);
	tv = waterdropMat->MatTransform(3, 1);

	tu += 0.05f * t;
	tv -= 0.05f * t;

	if (tu >= 1.0f)
		tu -= 1.0f;

	if (tv <= 0.0f)
		tv += 1.0f;

	waterdropMat->MatTransform(3, 0) = tu;
	waterdropMat->MatTransform(3, 1) = tv;
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
	rope->Roughness = 1.0f;

	auto lobby = std::make_unique<Material>();
	lobby->MatCBIndex = 8;
	lobby->DiffuseSrvHeapIndex = 8;
	lobby->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby->Roughness = 1.0f;

	auto arctic = std::make_unique<Material>();
	arctic->MatCBIndex = 9;
	arctic->DiffuseSrvHeapIndex = 9;
	arctic->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	arctic->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	arctic->Roughness = 1.0f;

	auto ArcticFox = std::make_unique<Material>();
	ArcticFox->MatCBIndex = 10;
	ArcticFox->DiffuseSrvHeapIndex = 10;
	ArcticFox->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ArcticFox->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	ArcticFox->Roughness = 1.0f;

	auto PolarBear = std::make_unique<Material>();
	PolarBear->MatCBIndex = 11;
	PolarBear->DiffuseSrvHeapIndex = 11;
	PolarBear->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	PolarBear->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	PolarBear->Roughness = 1.0f;

	auto Seal = std::make_unique<Material>();
	Seal->MatCBIndex = 12;
	Seal->DiffuseSrvHeapIndex = 12;
	Seal->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Seal->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	Seal->Roughness = 1.0f;

	auto lobby1 = std::make_unique<Material>();
	lobby1->MatCBIndex = 13;
	lobby1->DiffuseSrvHeapIndex = 13;
	lobby1->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby1->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby1->Roughness = 1.0f;

	auto lobby2 = std::make_unique<Material>();
	lobby2->MatCBIndex = 14;
	lobby2->DiffuseSrvHeapIndex = 14;
	lobby2->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby2->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby2->Roughness = 1.0f;

	auto lobby3 = std::make_unique<Material>();
	lobby3->MatCBIndex = 15;
	lobby3->DiffuseSrvHeapIndex = 15;
	lobby3->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby3->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby3->Roughness = 1.0f;

	auto lobby4 = std::make_unique<Material>();
	lobby4->MatCBIndex = 16;
	lobby4->DiffuseSrvHeapIndex = 16;
	lobby4->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby4->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby4->Roughness = 1.0f;

	auto lobby5 = std::make_unique<Material>();
	lobby5->MatCBIndex = 17;
	lobby5->DiffuseSrvHeapIndex = 17;
	lobby5->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lobby5->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	lobby5->Roughness = 1.0f;

	auto waterdrop = std::make_unique<Material>();
	waterdrop->MatCBIndex = 18;
	waterdrop->DiffuseSrvHeapIndex = 18;
	waterdrop->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	waterdrop->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	waterdrop->Roughness = 1.0f;

	auto TranslucentArcticFox = std::make_unique<Material>();
	TranslucentArcticFox->MatCBIndex = 19;
	TranslucentArcticFox->DiffuseSrvHeapIndex = 10;
	TranslucentArcticFox->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	TranslucentArcticFox->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	TranslucentArcticFox->Roughness = 1.0f;

	auto huskyimage1 = std::make_unique<Material>();
	huskyimage1->MatCBIndex = 20;
	huskyimage1->DiffuseSrvHeapIndex = 4;
	huskyimage1->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
	huskyimage1->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	huskyimage1->Roughness = 1.0f;

	auto huskyimage2 = std::make_unique<Material>();
	huskyimage2->MatCBIndex = 21;
	huskyimage2->DiffuseSrvHeapIndex = 4;
	huskyimage2->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	huskyimage2->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	huskyimage2->Roughness = 1.0f;

	auto huskyimage3 = std::make_unique<Material>();
	huskyimage3->MatCBIndex = 22;
	huskyimage3->DiffuseSrvHeapIndex = 4;
	huskyimage3->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	huskyimage3->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	huskyimage3->Roughness = 1.0f;

	auto snowmanicon = std::make_unique<Material>();
	snowmanicon->MatCBIndex = 23;
	snowmanicon->DiffuseSrvHeapIndex = 19;
	snowmanicon->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	snowmanicon->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	snowmanicon->Roughness = 1.0f;

	auto iglooicon = std::make_unique<Material>();
	iglooicon->MatCBIndex = 24;
	iglooicon->DiffuseSrvHeapIndex = 20;
	iglooicon->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	iglooicon->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	iglooicon->Roughness = 1.0f;

	auto blueicon = std::make_unique<Material>();
	blueicon->MatCBIndex = 25;
	blueicon->DiffuseSrvHeapIndex = 21;
	blueicon->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	blueicon->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	blueicon->Roughness = 1.0f;

	auto redicon = std::make_unique<Material>();
	redicon->MatCBIndex = 26;
	redicon->DiffuseSrvHeapIndex = 22;
	redicon->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	redicon->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	redicon->Roughness = 1.0f;

	auto redline = std::make_unique<Material>();
	redline->MatCBIndex = 27;
	redline->DiffuseSrvHeapIndex = 12;
	redline->DiffuseAlbedo = XMFLOAT4(30.0f, 0.0f, 0.0f, 1.0f);
	redline->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	redline->Roughness = 1.0f;

	auto blueline = std::make_unique<Material>();
	blueline->MatCBIndex = 28;
	blueline->DiffuseSrvHeapIndex = 12;
	blueline->DiffuseAlbedo = XMFLOAT4(0.0f, 0.0f, 30.0f, 1.0f);
	blueline->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	blueline->Roughness = 1.0f;

	m_Materials["snowcube1024"] = std::move(sky);
	m_Materials["ice"] = std::move(ice);
	m_Materials["Penguin"] = std::move(Penguin);
	m_Materials["water"] = std::move(water);
	m_Materials["husky"] = std::move(husky);
	m_Materials["heart"] = std::move(heart);
	m_Materials["heartline"] = std::move(heartline);
	m_Materials["rope"] = std::move(rope);
	m_Materials["lobby"] = std::move(lobby);
	m_Materials["arctic"] = std::move(arctic);
	m_Materials["ArcticFox"] = std::move(ArcticFox);
	m_Materials["PolarBear"] = std::move(PolarBear);
	m_Materials["Seal"] = std::move(Seal);
	m_Materials["lobby1"] = std::move(lobby1);
	m_Materials["lobby2"] = std::move(lobby2);
	m_Materials["lobby3"] = std::move(lobby3);
	m_Materials["lobby4"] = std::move(lobby4);
	m_Materials["lobby5"] = std::move(lobby5);
	m_Materials["waterdrop"] = std::move(waterdrop);
	m_Materials["TranslucentArcticFox"] = std::move(TranslucentArcticFox);
	m_Materials["huskyimage1"] = std::move(huskyimage1);
	m_Materials["huskyimage2"] = std::move(huskyimage2);
	m_Materials["huskyimage3"] = std::move(huskyimage3);
	m_Materials["snowmanicon"] = std::move(snowmanicon);
	m_Materials["iglooicon"] = std::move(iglooicon);
	m_Materials["blueicon"] = std::move(blueicon);
	m_Materials["redicon"] = std::move(redicon);
	m_Materials["redline"] = std::move(redline);
	m_Materials["blueline"] = std::move(blueline);
}
