#include "pch.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "Character.h"
#include "CharacterParts.h"
#include "MeshReference.h"
#include "MaterialReference.h"
#include "Particle.h"

#include <random>

random_device rd1;
default_random_engine dre2(rd1());
uniform_int_distribution<> uid5{ 0,8 }; //눈사람 위치
uniform_int_distribution<> uid4{ 0,3 }; //블록 덮개 회전
uniform_int_distribution<> uid6{ 15,25 }; //배경 캐릭터 크기
uniform_int_distribution<> uid7{ 0,100 }; // 회전
uniform_int_distribution<> uid8{ 0,4 }; //배경 캐릭터 위치
uniform_int_distribution<> uid9{ -620,620 }; //배경 캐릭터 반경

using namespace Core;

#define SCALE 1

std::string ApplicationContext::FindAnimName(int animCode) const
{
	std::string animName;

	// 애니메이션 상수 string -> 디파인값으로 바꿀예정
	switch (animCode)
	{
	case ANIM_IDLE:
		animName = "Idle";
		break;
	case ANIM_MOVE:
		animName = "Forward";
		break;
	case ANIM_ATTACK:
		animName = "Attack";
		break;
		//case ANIM_RUNNING:
		//	animName = "Running";
		//	break;
	case ANIM_JUMP:
		animName = "jump";
		break;
	case ANIM_FALL:
		animName = "Fall";
		break;
	default:
		cout << "None Anim Code" << endl;
		break;
	}

	return animName;
}

void ApplicationContext::CreateLobby() {
	GameObject* top = CreateObject<GameObject>("lobby", "lobby0");
	top->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
	top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	top->IndexCount = top->Geo->DrawArgs["grid"].IndexCount;
	top->StartIndexLocation = top->Geo->DrawArgs["grid"].StartIndexLocation;
	top->BaseVertexLocation = top->Geo->DrawArgs["grid"].BaseVertexLocation;
	top->m_IsVisible = true;
	top->m_MaterialIndex = 8;
	top->m_World = MathHelper::Identity4x4();
	top->m_World._11 = 200 * 1.43;
	top->m_World._22 = 150 * 1.43;
	top->m_World._33 = 1;
	//XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
	top->m_World._41 = 180;
	top->m_World._42 = 100;
	top->m_World._43 = 90;
	top->m_TexTransform = MathHelper::Identity4x4();
}

void ApplicationContext::CreateSkycube(std::string skycubeName, std::string instID, std::string matName)
{

	GameObject* skyRitem = CreateObject<GameObject>(skycubeName, instID);		//0
	skyRitem->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->m_MaterialIndex = MaterialReference::GetApp()->m_Materials[matName]->DiffuseSrvHeapIndex;
	skyRitem->m_IsVisible = true;
	//skyRitem->m_MaterialIndex = MeshReference::GetApp()->m_Materials[matName]->DiffuseSrvHeapIndex;
	skyRitem->m_World = MathHelper::Identity4x4();
	skyRitem->m_TexTransform = MathHelper::Identity4x4();
	skyRitem->Scale(5000, 5000, 5000);
}

void ApplicationContext::CreateDebugBoundingBox(std::string boundsName, std::string boundsInstName)
{
	/*GameObject* item = CreateObject<GameObject>(boundsName, boundsInstName);
	item->Geo = MeshReference::GetApp()->m_GeometryMesh[boundsName].get();
	item->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	item->IndexCount = item->Geo->DrawArgs[boundsName].IndexCount;
	item->StartIndexLocation = item->Geo->DrawArgs[boundsName].StartIndexLocation;
	item->BaseVertexLocation = item->Geo->DrawArgs[boundsName].BaseVertexLocation;
	item->m_IsVisible = false;
	item->m_MaterialIndex = 1;*/

	GameObject* quadRitem = CreateObject<GameObject>("quad", "quad");
	quadRitem->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
	quadRitem->IndexCount = quadRitem->Geo->DrawArgs["quad"].IndexCount;
	quadRitem->StartIndexLocation = quadRitem->Geo->DrawArgs["quad"].StartIndexLocation;
	quadRitem->BaseVertexLocation = quadRitem->Geo->DrawArgs["quad"].BaseVertexLocation;
	quadRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	quadRitem->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["bricks0"]->DiffuseSrvHeapIndex;
	quadRitem->m_World = MathHelper::Identity4x4();
	quadRitem->m_TexTransform = MathHelper::Identity4x4();
	quadRitem->m_IsVisible = true;

}

bool ApplicationContext::BlockCheck(int idx) {
	if (idx == 0 || idx == 2 || idx == 4 || idx == 10 || idx == 12 || idx == 14 || idx == 20 || idx == 22 || idx == 24)
		return false;
	return true;
}

void ApplicationContext::CreateBlocks()
{
	int distance = SCALE * 200;

	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = CreateObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["icecube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["icecube"].Bounds;
				instancingObj->m_IsVisible = true;
				instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ice"]->DiffuseSrvHeapIndex;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;

				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;
				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["icecube"].Bounds.Center, instancingObj->GetPosition());
			}
			else {
				instancingObj = CreateObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["snowcube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowcube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowcube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowcube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["snowcube"].Bounds;
				instancingObj->m_IsVisible = true;
				instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ice"]->DiffuseSrvHeapIndex;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;

				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;
				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["snowcube"].Bounds.Center, instancingObj->GetPosition());
			}

			GameObject* top = CreateObject<GameObject>("snow_top", "snow_top" + std::to_string(5 * i + j));
			top->Geo = MeshReference::GetApp()->m_GeometryMesh["snow_top"].get();
			top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			top->IndexCount = top->Geo->DrawArgs["snow_top"].IndexCount;
			top->StartIndexLocation = top->Geo->DrawArgs["snow_top"].StartIndexLocation;
			top->BaseVertexLocation = top->Geo->DrawArgs["snow_top"].BaseVertexLocation;
			top->m_IsVisible = true;
			top->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ice"]->DiffuseSrvHeapIndex;
			top->m_World = MathHelper::Identity4x4();
			top->m_World._11 = SCALE;
			top->m_World._22 = SCALE;
			top->m_World._33 = SCALE;
			XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationY(3.141592 * uid4(dre2)));
			top->m_World._41 = distance * i;
			top->m_World._42 = -30;
			top->m_World._43 = distance * j;
			top->m_TexTransform = MathHelper::Identity4x4();
		}
	}

	for (int i = 0; i < 5; ++i) {	//51~75
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = CreateObject<GameObject>("icicle", "icicle" + std::to_string(5 * i + j));
			instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["icicle"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icicle"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icicle"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icicle"].BaseVertexLocation;
			instancingObj->m_IsVisible = true;
			instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ice"]->DiffuseSrvHeapIndex;
			instancingObj->m_World = MathHelper::Identity4x4();
			instancingObj->m_World._11 = SCALE * 7.5 / 10.0;
			instancingObj->m_World._22 = SCALE;
			instancingObj->m_World._33 = SCALE * 7.5 / 10.0;
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * uid4(dre2)));
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._42 = -30;
			instancingObj->m_World._43 = distance * j;
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}
}

void ApplicationContext::Createigloos()
{
	int RandomLocation[2] = { -1,-1 };

	for (int i = 0; i < 2; ++i) {		//76, 77
		GameObject* instancingObj = CreateObject<GameObject>("igloo", "igloo" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["igloo"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowman"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowman"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowman"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["ice"]->DiffuseSrvHeapIndex;

		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["igloo"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["igloo"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["igloo"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;

		instancingObj->m_IsVisible = true;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = SCALE * 1 / 10.0;
		instancingObj->m_World._22 = SCALE * 1 / 10.0;
		instancingObj->m_World._33 = SCALE * 1 / 10.0;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}
}

void ApplicationContext::CreateWave()
{
	//78
	GameObject* Sea = CreateObject<GameObject>("Sea", "Sea0");
	Sea->Geo = MeshReference::GetApp()->m_GeometryMesh["wave"].get();
	Sea->IndexCount = Sea->Geo->DrawArgs["wave"].IndexCount;
	Sea->StartIndexLocation = Sea->Geo->DrawArgs["wave"].StartIndexLocation;
	Sea->BaseVertexLocation = Sea->Geo->DrawArgs["wave"].BaseVertexLocation;
	Sea->m_IsVisible = true;
	Sea->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	Sea->m_MaterialIndex = 3;
	Sea->m_World = MathHelper::Identity4x4();
	Sea->m_World._11 = 11;
	Sea->m_World._33 = 11;

	Sea->m_World._41 = SCALE * 400;
	Sea->m_World._42 = -SCALE * 100;
	Sea->m_World._43 = SCALE * 400;

	Sea->m_TexTransform = MathHelper::Identity4x4();
	Core::wave[0] = Sea;
}

void ApplicationContext::CreateBackground()
{
	float X[5] = { -950,400,1750,-950,1750 };		//배경 블록 위치
	float Z[5] = { 1800,1800,1800,400,400 };

	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("snowcube", "snowcube" + std::to_string(25 + i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["snowcube"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowcube"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowcube"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowcube"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["snowcube"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = 1;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 13;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;

		instancingObj->m_World._41 = X[i];
		instancingObj->m_World._42 = -640;
		instancingObj->m_World._43 = Z[i];
		instancingObj->m_TexTransform = MathHelper::Identity4x4();

		instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["snowcube"].Bounds.Center, instancingObj->GetPosition());
	}

	//84~133	캐릭터
	for (int i = 0; i < 50; ++i) {
		std::string meshName;
		std::string instID;
		int matidx = -1;
		auto bonidx = BoneIndex::Count;
		if (i / 10 == 0) {
			meshName = "husky";
			instID = "husky" + std::to_string(i + 100);
			matidx = 4;
			bonidx = BoneIndex::Husky;
		}
		else if (i / 10 == 1) {
			meshName = "Penguin_LOD0skin";
			instID = "Penguin_LOD0skin" + std::to_string(i + 100);
			matidx = 2;
			bonidx = BoneIndex::Penguin;
		}
		else if (i / 10 == 2) {
			meshName = "ArcticFox";
			instID = "ArcticFox" + std::to_string(i + 100);
			matidx = 10;
			bonidx = BoneIndex::Fox;
		}
		else if (i / 10 == 3) {
			meshName = "PolarBear";
			instID = "PolarBear" + std::to_string(i + 100);
			matidx = 11;
			bonidx = BoneIndex::PolarBear;
		}
		else if (i / 10 == 4) {
			meshName = "Seal";
			instID = "Seal" + std::to_string(i + 100);
			matidx = 12;
			bonidx = BoneIndex::Seal;
		}

		Character* chr = CreateObject<Character>(meshName, instID);
		chr->Geo = MeshReference::GetApp()->m_GeometryMesh[meshName].get();
		chr->IndexCount = chr->Geo->DrawArgs[meshName].IndexCount;
		chr->StartIndexLocation = chr->Geo->DrawArgs[meshName].StartIndexLocation;
		chr->BaseVertexLocation = chr->Geo->DrawArgs[meshName].BaseVertexLocation;
		chr->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		chr->m_Bounds = chr->Geo->DrawArgs[meshName].Bounds;
		chr->m_MaterialIndex = matidx;
		chr->m_SkinnedCBIndex = bonidx;
		chr->m_SkinnedModelInst = NULL; MeshReference::GetApp()->m_SkinnedModelInsts[meshName].get();
		if (i < 3)
			chr->m_IsVisible = true;
		else
			chr->m_IsVisible = false;
		// 임시 스폰위치 지정
		//chr->m_SpawnLoaction = skinnedCBIndex;
		int XPos, ZPos;
		int size = uid6(dre2);
		chr->Scale(size, size, size);
		int tmp = 0;
		float tmp2 = 0;
		do {
			tmp = uid8(dre2);
			XPos = X[tmp] + uid9(dre2);
			ZPos = Z[tmp] + uid9(dre2);
			tmp2 = sqrt(pow(XPos - X[tmp], 2) + pow(ZPos - Z[tmp], 2));
		} while (XPos < -600 || (XPos < -300 && ZPos>1100) || (XPos > -300 && XPos < 15) || (XPos > 15 && XPos < 150)
			|| (XPos > 150 && XPos < 350 && ZPos>1600) || (XPos > 350 && XPos < 660 && ZPos>1150) || (XPos > 660 && XPos < 1100 && ZPos>1350)
			|| (XPos > 1100 && XPos < 1570 && ZPos>800) || XPos>1600 || ZPos < 0);
		if (XPos <= 200) {	//항상 중심 바라보게
			chr->Rotate(0, 180 - ((200 - XPos) * 45 / 600 + (1100 - ZPos) * 45 / 1100), 0);
		}
		else {
			chr->Rotate(0, 180 + ((XPos - 200) * 45 / 600 + (1100 - ZPos) * 45 / 1100), 0);
		}
		chr->SetPosition(XPos, -size + 95 - 45 * tmp2 / (620.0 * sqrt(2)) + 10, ZPos);
	}

	//134~138	라이프
	for (int i = 0; i < 5; ++i) {
		GameObject* top = CreateObject<GameObject>("life", "life" + std::to_string(i));
		top->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
		top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		top->IndexCount = top->Geo->DrawArgs["grid"].IndexCount;
		top->StartIndexLocation = top->Geo->DrawArgs["grid"].StartIndexLocation;
		top->BaseVertexLocation = top->Geo->DrawArgs["grid"].BaseVertexLocation;
		top->m_IsVisible = true;
		top->m_MaterialIndex = 5;
		top->m_World = MathHelper::Identity4x4();
		float size = 7;
		top->m_World._11 = size;
		top->m_World._22 = size;
		top->m_World._33 = size;
		//XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
		top->m_World._41 = (size * 1.1) * i;
		top->m_World._42 = 300;
		top->m_World._43 = 400;
		top->m_TexTransform = MathHelper::Identity4x4();
	}

	//139~143	밧줄
	for (int i = 0; i < 5; ++i) {
		GameObject* top = CreateObject<GameObject>("lifeline", "lifeline" + std::to_string(i));
		top->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
		top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		top->IndexCount = top->Geo->DrawArgs["grid"].IndexCount;
		top->StartIndexLocation = top->Geo->DrawArgs["grid"].StartIndexLocation;
		top->BaseVertexLocation = top->Geo->DrawArgs["grid"].BaseVertexLocation;
		top->m_MaterialIndex = 7;
		top->m_IsVisible = true;
		top->m_World = MathHelper::Identity4x4();
		float size = 15;
		top->m_World._11 = size;
		top->m_World._22 = size;
		top->m_World._33 = size;
		//	XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
		top->m_World._41 = (size * 1.1) * i;
		top->m_World._42 = 300;
		top->m_World._43 = 400;
		top->m_TexTransform = MathHelper::Identity4x4();
	}

	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("mountain", "mountain" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["mountain"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["mountain"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["mountain"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["mountain"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["mountain"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 0.8;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;

		instancingObj->m_World._42 = 50;
		if (i < 2) {
			instancingObj->m_World._41 = X[0] + 600 * (i);
			instancingObj->m_World._43 = Z[0] - 400 + 400 * i;
		}
		else {
			instancingObj->m_World._41 = X[0] + 600 * (i + 1);
			instancingObj->m_World._43 = Z[0] - 0 - 400 * (i - 2);
		}
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("tent", "tent" + std::to_string(0));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["tent"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["tent"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["tent"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["tent"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["tent"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 0.7;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * 8 / 10));
		instancingObj->m_World._41 = X[1] - 450;
		instancingObj->m_World._42 = 50;
		instancingObj->m_World._43 = Z[1] - 400;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("kayak", "kayak" + std::to_string(0));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["kayak"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["kayak"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["kayak"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["kayak"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["kayak"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 0.7;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;
		instancingObj->m_World._41 = X[1] - 350;
		instancingObj->m_World._42 = 50;
		instancingObj->m_World._43 = Z[1] - 700;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("kayakpaddle", "kayakpaddle" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["kayakpaddle"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["kayakpaddle"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["kayakpaddle"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["kayakpaddle"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["kayakpaddle"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 0.7;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;
		instancingObj->m_World._41 = X[1] - 330 + 20 * (i + 1);
		instancingObj->m_World._42 = 50;
		instancingObj->m_World._43 = Z[1] - 680;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("rock_0", "rock_0" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["rock_0"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["rock_0"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["rock_0"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["rock_0"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["rock_0"].Bounds;
		instancingObj->m_IsVisible = true;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		float size = 2;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * uid7(dre2) / 100.0));
		instancingObj->m_World._41 = 400 + cos(i) * 1350;
		instancingObj->m_World._42 = 80;
		if (sin(i) < 0)
			instancingObj->m_World._43 = 400 - sin(i) * 1350;
		else
			instancingObj->m_World._43 = 400 + sin(i) * 1350;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("rock_1", "rock_1" + std::to_string(i));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["rock_1"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["rock_1"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["rock_1"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["rock_1"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["rock_1"].Bounds;
		instancingObj2->m_IsVisible = true;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		float size = 1.0;
		instancingObj2->m_World._11 = size;
		instancingObj2->m_World._22 = size;
		instancingObj2->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 0.5));
		if (i == 11) {
			instancingObj2->m_World._41 = X[1] + 100;
			instancingObj2->m_World._42 = 150;
			instancingObj2->m_World._43 = Z[1] - 500;
		}
		else {
			instancingObj2->m_World._41 = X[1] + 100 + cos(i * 3.141592 / 15 * 2) * 100;
			instancingObj2->m_World._42 = 65;
			instancingObj2->m_World._43 = Z[1] - 500 + sin(i * 3.141592 / 15 * 2) * 100;
		}
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("igloo", "igloo" + std::to_string(2));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["igloo"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["igloo"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["igloo"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["igloo"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["igloo"].Bounds;
		instancingObj2->m_IsVisible = true;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		float size = 0.6;
		instancingObj2->m_World._11 = size;
		instancingObj2->m_World._22 = size;
		instancingObj2->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592));
		instancingObj2->m_World._41 = X[1] + 100;
		instancingObj2->m_World._42 = 50;
		instancingObj2->m_World._43 = Z[1] - 500;
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("fish", "fish" + std::to_string(i));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["fish"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["fish"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["fish"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["fish"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["fish"].Bounds;
		instancingObj2->m_IsVisible = true;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		float size = 0.7;
		instancingObj2->m_World._11 = size;
		instancingObj2->m_World._22 = size;
		instancingObj2->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 11 / 10));
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationX(3.141592 * 2 / 10));
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationZ(3.141592 * -1 / 10));
		instancingObj2->m_World._41 = X[1] - 465 - i * 13;
		instancingObj2->m_World._42 = 130;
		instancingObj2->m_World._43 = Z[1] - 495 + i * 13;
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("sled", "sled" + std::to_string(0));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["sled"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["sled"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["sled"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["sled"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["sled"].Bounds;
		instancingObj2->m_IsVisible = true;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		float size = 1;
		instancingObj2->m_World._11 = size;
		instancingObj2->m_World._22 = size;
		instancingObj2->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 7 / 10));
		instancingObj2->m_World._41 = X[1] - 500;
		instancingObj2->m_World._42 = 30;
		instancingObj2->m_World._43 = Z[0] - 650;
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("fishrack", "fishrack" + std::to_string(0));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["fishrack"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["fishrack"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["fishrack"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["fishrack"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["fishrack"].Bounds;
		instancingObj2->m_IsVisible = true;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		float size = 1;
		instancingObj2->m_World._11 = size;
		instancingObj2->m_World._22 = size;
		instancingObj2->m_World._33 = size;
		XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 7 / 10));
		instancingObj2->m_World._41 = X[0] + 520;
		instancingObj2->m_World._42 = 30;
		instancingObj2->m_World._43 = 1170;
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = CreateObject<GameObject>("fish", "fish" + std::to_string(4 + i));
			instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["fish"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["fish"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["fish"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["fish"].BaseVertexLocation;
			instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["fish"].Bounds;
			instancingObj->m_IsVisible = true;
			instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
			instancingObj->m_World = MathHelper::Identity4x4();
			float size = 0.7;
			instancingObj->m_World._11 = size;
			instancingObj->m_World._22 = size;
			instancingObj->m_World._33 = size;
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * 11 / 10));
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(3.141592 * 1.5 / 10));
			instancingObj->m_World._41 = X[0] + 470 + 11 * i;
			instancingObj->m_World._42 = 180;
			instancingObj->m_World._43 = 1110 + 15 * i;
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}
}

void ApplicationContext::CreateSnowmans()
{
	for (int i = 0; i < 4; ++i) {		//209~212
		GameObject* instancingObj = CreateObject<GameObject>("snowman", "snowman" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["snowman"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowman"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowman"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowman"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = 1;
		instancingObj->m_IsVisible = true;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = SCALE * 3 / 5.0;
		instancingObj->m_World._22 = SCALE * 3 / 5.0;
		instancingObj->m_World._33 = SCALE * 3 / 5.0;
		instancingObj->m_TexTransform = MathHelper::Identity4x4();

	}
}

void ApplicationContext::CreateHail()
{
	//213~217
	for (int i = 0; i < 5; ++i) {
		GameObject* instancingObj = CreateObject<GameObject>("rock_1", "rock_1" + std::to_string(i + 15));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["rock_1"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["rock_1"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["rock_1"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["rock_1"].BaseVertexLocation;
		instancingObj->m_MaterialIndex = 11;
		instancingObj->m_IsVisible = true;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = 1.5;
		instancingObj->m_World._22 = 1.5;
		instancingObj->m_World._33 = 1.5;

		XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(3.141592 * -0.4));

		instancingObj->m_TexTransform = MathHelper::Identity4x4();

	}
}

void ApplicationContext::CreateWaterDrop()
{
	//218~221
	for (int i = 0; i < 4; ++i) {
		GameObject* top = CreateObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i));
		top->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
		top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		top->IndexCount = top->Geo->DrawArgs["grid"].IndexCount;
		top->StartIndexLocation = top->Geo->DrawArgs["grid"].StartIndexLocation;
		top->BaseVertexLocation = top->Geo->DrawArgs["grid"].BaseVertexLocation;
		top->m_IsVisible = true;
		top->m_MaterialIndex = 18;
		top->m_World = MathHelper::Identity4x4();
		top->m_World._11 = 150;
		top->m_World._22 = 150;
		top->m_World._33 = 1;
		XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * 30 / 180));
		XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationY(3.141592 * i / 2.0));

		top->m_TexTransform = MathHelper::Identity4x4();

	}
}

void ApplicationContext::CreateMinimap()
{
	{	//222
		GameObject* Sea = CreateObject<GameObject>("MinimapSea", "MinimapSea0");
		Sea->Geo = MeshReference::GetApp()->m_GeometryMesh["wave"].get();
		Sea->IndexCount = Sea->Geo->DrawArgs["wave"].IndexCount;
		Sea->StartIndexLocation = Sea->Geo->DrawArgs["wave"].StartIndexLocation;
		Sea->BaseVertexLocation = Sea->Geo->DrawArgs["wave"].BaseVertexLocation;
		Sea->m_IsVisible = true;
		Sea->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Sea->m_MaterialIndex = 3;
		Sea->m_World = MathHelper::Identity4x4();
		Sea->m_World._11 = 3;
		Sea->m_World._33 = 3;

		XMStoreFloat4x4(&Sea->m_World, XMLoadFloat4x4(&Sea->m_World) * XMMatrixRotationX(-3.141592 * (90 - acos(0.4 * sqrt(5))) / 180.0f));

		Sea->m_TexTransform = MathHelper::Identity4x4();
		Core::wave[1] = Sea;
	}
	float scale = SCALE * 2.0f / 11.0f;
	int distance = scale * 200;

	for (int i = 0; i < 5; ++i) {	//223~272
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = CreateObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["icecube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icecube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icecube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icecube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["icecube"].Bounds;
				instancingObj->m_IsVisible = true;
				instancingObj->m_MaterialIndex = 1;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = scale;
				instancingObj->m_World._22 = scale;
				instancingObj->m_World._33 = scale;

				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;

				XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(-3.141592 * (90 - acos(0.4 * sqrt(5))) / 180.0f));

				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["icecube"].Bounds.Center, instancingObj->GetPosition());
			}
			else {
				instancingObj = CreateObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["snowcube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["snowcube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["snowcube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["snowcube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["snowcube"].Bounds;
				instancingObj->m_IsVisible = true;
				instancingObj->m_MaterialIndex = 1;
				instancingObj->m_World = MathHelper::Identity4x4();
				instancingObj->m_World._11 = scale;
				instancingObj->m_World._22 = scale;
				instancingObj->m_World._33 = scale;

				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;

				XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(-3.141592 * (90 - acos(0.4 * sqrt(5))) / 180.0f));

				instancingObj->m_TexTransform = MathHelper::Identity4x4();

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["snowcube"].Bounds.Center, instancingObj->GetPosition());
			}
			MinimapBlockScale = XMFLOAT3(
				instancingObj->m_World._11,
				instancingObj->m_World._22,
				instancingObj->m_World._33
			);

			float d = 800.0f / 11.0f;
			MinimapCubePos[5 * i + j] = XMFLOAT3(
				instancingObj->m_World._41 - d,
				instancingObj->m_World._42 - d,
				instancingObj->m_World._43 - d);

			GameObject* top = CreateObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(5 * i + j));
			top->Geo = MeshReference::GetApp()->m_GeometryMesh["snow_top"].get();
			top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			top->IndexCount = top->Geo->DrawArgs["snow_top"].IndexCount;
			top->StartIndexLocation = top->Geo->DrawArgs["snow_top"].StartIndexLocation;
			top->BaseVertexLocation = top->Geo->DrawArgs["snow_top"].BaseVertexLocation;
			top->m_IsVisible = true;
			top->m_MaterialIndex = 1;
			top->m_World = MathHelper::Identity4x4();
			top->m_World._11 = scale;
			top->m_World._22 = scale;
			top->m_World._33 = scale;
			top->m_World._41 = distance * i;
			top->m_World._42 = -30;
			top->m_World._43 = distance * j;

			XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(-3.141592 * (90 - acos(0.4 * sqrt(5))) / 180.0f));

			top->m_TexTransform = MathHelper::Identity4x4();
		}
	}

	for (int i = 0; i < 5; ++i) {	//273~297
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = CreateObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(5 * i + j));
			instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["icicle"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["icicle"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["icicle"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["icicle"].BaseVertexLocation;
			instancingObj->m_IsVisible = true;
			instancingObj->m_MaterialIndex = 1;
			instancingObj->m_World = MathHelper::Identity4x4();
			instancingObj->m_World._11 = scale * 7.5 / 10.0;
			instancingObj->m_World._22 = scale;
			instancingObj->m_World._33 = scale * 7.5 / 10.0;
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._42 = -30;
			instancingObj->m_World._43 = distance * j;

			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(-3.141592 * (90 - acos(0.4 * sqrt(5))) / 180.0f));
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}

	{
		//298
		GameObject* icon = CreateObject<GameObject>("myicon", "myicon0");
		icon->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
		icon->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		icon->IndexCount = icon->Geo->DrawArgs["grid"].IndexCount;
		icon->StartIndexLocation = icon->Geo->DrawArgs["grid"].StartIndexLocation;
		icon->BaseVertexLocation = icon->Geo->DrawArgs["grid"].BaseVertexLocation;
		icon->m_IsVisible = true;
		icon->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["blueicon"]->MatCBIndex;
		icon->m_World = MathHelper::Identity4x4();
		icon->m_World._11 = 15;
		icon->m_World._22 = 15;
		icon->m_World._33 = 1;
		XMStoreFloat4x4(&icon->m_World, XMLoadFloat4x4(&icon->m_World) * XMMatrixRotationX(3.141592 * acos(0.4 * sqrt(5)) / 180.0f));

		icon->m_TexTransform = MathHelper::Identity4x4();
	}

	{
		//299~302
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = CreateObject<GameObject>("snowmanicon", "snowmanicon" + std::to_string(i));
			icon->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
			icon->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			icon->IndexCount = icon->Geo->DrawArgs["grid"].IndexCount;
			icon->StartIndexLocation = icon->Geo->DrawArgs["grid"].StartIndexLocation;
			icon->BaseVertexLocation = icon->Geo->DrawArgs["grid"].BaseVertexLocation;
			icon->m_IsVisible = true;
			icon->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["snowmanicon"]->MatCBIndex;
			icon->m_World = MathHelper::Identity4x4();
			icon->m_World._11 = 50;
			icon->m_World._22 = 50;
			icon->m_World._33 = 1;

			XMStoreFloat4x4(&icon->m_World, XMLoadFloat4x4(&icon->m_World) * XMMatrixRotationX(3.141592 * acos(0.4 * sqrt(5)) / 180.0f));

			icon->m_TexTransform = MathHelper::Identity4x4();
		}
	}
	{
		//303~304
		for (int i = 0; i < 2; ++i) {
			GameObject* icon = CreateObject<GameObject>("iglooicon", "iglooicon" + std::to_string(i));
			icon->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
			icon->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			icon->IndexCount = icon->Geo->DrawArgs["grid"].IndexCount;
			icon->StartIndexLocation = icon->Geo->DrawArgs["grid"].StartIndexLocation;
			icon->BaseVertexLocation = icon->Geo->DrawArgs["grid"].BaseVertexLocation;
			icon->m_IsVisible = true;
			icon->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["iglooicon"]->MatCBIndex;
			icon->m_World = MathHelper::Identity4x4();
			icon->m_World._11 = 20;
			icon->m_World._22 = 20;
			icon->m_World._33 = 1;

			XMStoreFloat4x4(&icon->m_World, XMLoadFloat4x4(&icon->m_World) * XMMatrixRotationX(3.141592 * acos(0.4 * sqrt(5)) / 180.0f));

			icon->m_TexTransform = MathHelper::Identity4x4();
		}
	}
	{
		//305~308
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = CreateObject<GameObject>("enermyicon", "enermyicon" + std::to_string(i));
			icon->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
			icon->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			icon->IndexCount = icon->Geo->DrawArgs["grid"].IndexCount;
			icon->StartIndexLocation = icon->Geo->DrawArgs["grid"].StartIndexLocation;
			icon->BaseVertexLocation = icon->Geo->DrawArgs["grid"].BaseVertexLocation;
			icon->m_IsVisible = true;
			icon->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redicon"]->MatCBIndex;
			icon->m_World = MathHelper::Identity4x4();
			icon->m_World._11 = 15;
			icon->m_World._22 = 15;
			icon->m_World._33 = 1;

			icon->m_World._41 = -1000;
			icon->m_World._42 = -1000;
			icon->m_World._43 = -1000;
			XMStoreFloat4x4(&icon->m_World, XMLoadFloat4x4(&icon->m_World) * XMMatrixRotationX(3.141592 * acos(0.4 * sqrt(5)) / 180.0f));

			icon->m_TexTransform = MathHelper::Identity4x4();
		}
	}
}

void ApplicationContext::CreateOutline() {
	{
		//309~313
		Character* husky = CreateObject<Character>("huskyOutline", "huskyOutline0");
		husky->Geo = MeshReference::GetApp()->m_GeometryMesh["husky"].get();
		husky->IndexCount = husky->Geo->DrawArgs["husky"].IndexCount;
		husky->StartIndexLocation = husky->Geo->DrawArgs["husky"].StartIndexLocation;
		husky->BaseVertexLocation = husky->Geo->DrawArgs["husky"].BaseVertexLocation;
		husky->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		husky->m_Bounds = husky->Geo->DrawArgs["husky"].Bounds;
		husky->m_IsVisible = true;
		husky->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redline"]->MatCBIndex;
		husky->m_SkinnedCBIndex = BoneIndex::Husky;
		husky->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get();
		husky->m_World = MathHelper::Identity4x4();

		Character* penguin = CreateObject<Character>("Penguin_LOD0skinOutline", "Penguin_LOD0skinOutline0");
		penguin->Geo = MeshReference::GetApp()->m_GeometryMesh["Penguin_LOD0skin"].get();
		penguin->IndexCount = penguin->Geo->DrawArgs["Penguin_LOD0skin"].IndexCount;
		penguin->StartIndexLocation = penguin->Geo->DrawArgs["Penguin_LOD0skin"].StartIndexLocation;
		penguin->BaseVertexLocation = penguin->Geo->DrawArgs["Penguin_LOD0skin"].BaseVertexLocation;
		penguin->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		penguin->m_Bounds = penguin->Geo->DrawArgs["Penguin_LOD0skin"].Bounds;
		penguin->m_IsVisible = true;
		penguin->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redline"]->MatCBIndex;
		penguin->m_SkinnedCBIndex = BoneIndex::Penguin;
		penguin->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["Penguin_LOD0skin"].get();
		penguin->m_World = MathHelper::Identity4x4();

		Character* arcticfox = CreateObject<Character>("ArcticFoxOutline", "ArcticFoxOutline0");
		arcticfox->Geo = MeshReference::GetApp()->m_GeometryMesh["ArcticFox"].get();
		arcticfox->IndexCount = arcticfox->Geo->DrawArgs["ArcticFox"].IndexCount;
		arcticfox->StartIndexLocation = arcticfox->Geo->DrawArgs["ArcticFox"].StartIndexLocation;
		arcticfox->BaseVertexLocation = arcticfox->Geo->DrawArgs["ArcticFox"].BaseVertexLocation;
		arcticfox->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		arcticfox->m_Bounds = arcticfox->Geo->DrawArgs["ArcticFox"].Bounds;
		arcticfox->m_IsVisible = true;
		arcticfox->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redline"]->MatCBIndex;
		arcticfox->m_SkinnedCBIndex = BoneIndex::Fox;
		arcticfox->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["ArcticFox"].get();
		arcticfox->m_World = MathHelper::Identity4x4();

		Character* polarbear = CreateObject<Character>("PolarBearOutline", "PolarBearOutline0");
		polarbear->Geo = MeshReference::GetApp()->m_GeometryMesh["PolarBear"].get();
		polarbear->IndexCount = polarbear->Geo->DrawArgs["PolarBear"].IndexCount;
		polarbear->StartIndexLocation = polarbear->Geo->DrawArgs["PolarBear"].StartIndexLocation;
		polarbear->BaseVertexLocation = polarbear->Geo->DrawArgs["PolarBear"].BaseVertexLocation;
		polarbear->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		polarbear->m_Bounds = polarbear->Geo->DrawArgs["PolarBear"].Bounds;
		polarbear->m_IsVisible = true;
		polarbear->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redline"]->MatCBIndex;
		polarbear->m_SkinnedCBIndex = BoneIndex::PolarBear;
		polarbear->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["PolarBear"].get();
		polarbear->m_World = MathHelper::Identity4x4();

		Character* seal = CreateObject<Character>("SealOutline", "SealOutline0");
		seal->Geo = MeshReference::GetApp()->m_GeometryMesh["Seal"].get();
		seal->IndexCount = seal->Geo->DrawArgs["Seal"].IndexCount;
		seal->StartIndexLocation = seal->Geo->DrawArgs["Seal"].StartIndexLocation;
		seal->BaseVertexLocation = seal->Geo->DrawArgs["Seal"].BaseVertexLocation;
		seal->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		seal->m_Bounds = seal->Geo->DrawArgs["Seal"].Bounds;
		seal->m_IsVisible = true;
		seal->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["redline"]->MatCBIndex;
		seal->m_SkinnedCBIndex = BoneIndex::Seal;
		seal->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["Seal"].get();
		seal->m_World = MathHelper::Identity4x4();

	}
}

void ApplicationContext::CreateUI2D(std::string ui2dLayer, std::string ui2dName, int matIndex, float posX, float posY, float sizeX, float sizeY)
{
	GameObject* item = CreateObject<GameObject>(ui2dLayer, ui2dName);
	item->Geo = MeshReference::GetApp()->m_GeometryMesh["geo"].get();
	item->IndexCount = item->Geo->DrawArgs["grid"].IndexCount;
	item->StartIndexLocation = item->Geo->DrawArgs["grid"].StartIndexLocation;
	item->BaseVertexLocation = item->Geo->DrawArgs["grid"].BaseVertexLocation;
	item->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	item->m_Bounds = item->Geo->DrawArgs["grid"].Bounds;
	item->m_MaterialIndex = matIndex;
	item->m_IsVisible = true;

	item->m_positionRatio = { (posX - (sizeX / 20.f)) / 800.f, (posY - (sizeY / 20.f)) / 600.f };
	item->m_sizeRatio = { sizeX / 800.f, sizeY / sizeX };

	item->Scale(sizeX, sizeY, 1);
	item->SetPosition(posX, posY, 1.f);
}

void ApplicationContext::CreateParticle(std::string particleName, std::string instID, std::string matName)
{
	Particle* particle = CreateObject<Particle>(particleName, instID);
	particle->Geo = MeshReference::GetApp()->m_GeometryMesh[particleName].get();
	particle->IndexCount = particle->Geo->DrawArgs[particleName].IndexCount;
	particle->StartIndexLocation = particle->Geo->DrawArgs[particleName].StartIndexLocation;
	particle->BaseVertexLocation = particle->Geo->DrawArgs[particleName].BaseVertexLocation;
	particle->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	particle->m_Bounds = particle->Geo->DrawArgs[particleName].Bounds;
	particle->SetParticleNameCount(particleName);
	particle->m_MaterialIndex = MaterialReference::GetApp()->m_Materials[matName]->DiffuseSrvHeapIndex;
	//particle->m_MaterialIndex = 28;
	particle->m_IsVisible = true;
}

void ApplicationContext::CreateCharacter(std::string meshName, std::string instID, std::string matName, int skinnedCBIndex)
{
	Character* chr = CreateObject<Character>(meshName, instID);
	chr->Geo = MeshReference::GetApp()->m_GeometryMesh[meshName].get();
	chr->IndexCount = chr->Geo->DrawArgs[meshName].IndexCount;
	chr->StartIndexLocation = chr->Geo->DrawArgs[meshName].StartIndexLocation;
	chr->BaseVertexLocation = chr->Geo->DrawArgs[meshName].BaseVertexLocation;
	chr->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	chr->m_Bounds = chr->Geo->DrawArgs[meshName].Bounds;
	chr->m_HitBox.Extents = XMFLOAT3(10.0, 30.0, 12.0);
	chr->m_MaterialIndex = MaterialReference::GetApp()->m_Materials[matName]->DiffuseSrvHeapIndex;
	chr->m_SkinnedCBIndex = skinnedCBIndex;
	chr->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts[meshName].get();
	chr->m_IsVisible = false;
	// 임시 스폰위치 지정
	//chr->m_SpawnLoaction = skinnedCBIndex;
	if (meshName == "Seal")
		chr->Scale(15, 15, 15);
	else
		chr->Scale(20, 20, 20);
	chr->SetPosition(0, 30, 0);

}

void ApplicationContext::HiddenBlocks()
{

	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {

			if (BlockCheck(5 * i + j)) {
				GameObject* obj = FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j));
				if (!obj)
					return;
				ZeroMemory(&obj->m_World, sizeof(obj->m_World));
				ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
			}
			else {
				GameObject* obj = FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j));
				if (!obj)
					return;
				ZeroMemory(&obj->m_World, sizeof(obj->m_World));
				ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
			}
			GameObject* obj = FindObject<GameObject>("snow_top", "snow_top" + std::to_string(5 * i + j));
			if (!obj)
				return;
			ZeroMemory(&obj->m_World, sizeof(obj->m_World));
			ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));

			GameObject* obj0 = FindObject<GameObject>("icicle", "icicle" + std::to_string(5 * i + j));
			if (!obj0)
				return;
			ZeroMemory(&obj0->m_World, sizeof(obj0->m_World));
			ZeroMemory(&obj0->m_TexTransform, sizeof(obj0->m_TexTransform));
		}
	}

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icecube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snow_top"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["icicle"], m_RItemsVec);

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (BlockCheck(5 * i + j)) {
				GameObject* obj = FindObject<GameObject>("icecube", "icecube" + std::to_string(5 * i + j));
				obj->m_IsVisible = false;
			}
			else {
				GameObject* obj = FindObject<GameObject>("snowcube", "snowcube" + std::to_string(5 * i + j));
				obj->m_IsVisible = false;
			}
			GameObject* obj = FindObject<GameObject>("snow_top", "snow_top" + std::to_string(5 * i + j));
			obj->m_IsVisible = false;
			GameObject* obj0 = FindObject<GameObject>("icicle", "icicle" + std::to_string(5 * i + j));
			obj0->m_IsVisible = false;
		}
	}

}

void ApplicationContext::HiddenCharacter(std::string userName)
{
	Character* user = FindObject<Character>(userName, userName + std::to_string(0));
	if (!user) return;

	user->m_MyCamera = nullptr;
	user->m_IsVisible = false;
	user->m_SkinnedModelInst->ChangeSkinnedAnimation("Idle");

	ZeroMemory(&user->m_World, sizeof(user->m_World));
	ZeroMemory(&user->m_TexTransform, sizeof(user->m_TexTransform));

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap[userName], AppContext->m_RItemsVec);
}

void ApplicationContext::DisplayUI(std::string mapName)
{
}

void ApplicationContext::HiddenUI(std::string uiName)
{
	GameObject* obj = FindObject<GameObject>(uiName, uiName);
	ZeroMemory(&obj->m_World, sizeof(obj->m_World));
	ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
	obj->m_IsVisible = false;
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap[uiName], AppContext->m_RItemsVec);
}

void ApplicationContext::SetUI2DPosition(std::string ui2dName, float posX, float posY)
{
	GameObject* item = FindObject<GameObject>("UI_2D", ui2dName);
	item->SetPosition(posX / Core::g_DisplayWidth, posY / Core::g_DisplayHeight, 1);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap[ui2dName], AppContext->m_RItemsVec);
}

void ApplicationContext::DisplayParticle(std::string particleName, std::string instID, DirectX::XMFLOAT3 pos)
{
	Particle* ptc = FindObject<Particle>(particleName, instID);
	if (!ptc) { return; }
	ptc->m_IsVisible = true;
	ptc->m_World = MathHelper::Identity4x4();
	ptc->m_TexTransform = MathHelper::Identity4x4();
	ptc->SetPosition(pos);
	ptc->PlayParticle();
	// rotate
}

void ApplicationContext::HiddenParticle(std::string particleName, std::string instID)
{
	Particle* ptc = FindObject<Particle>(particleName, instID);
	if (!ptc) return;

	ZeroMemory(&ptc->m_World, sizeof(ptc->m_World));
	ZeroMemory(&ptc->m_TexTransform, sizeof(ptc->m_TexTransform));
	ptc->m_IsVisible = false;
	ptc->StopParticle();

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap[particleName], m_RItemsVec);
}
