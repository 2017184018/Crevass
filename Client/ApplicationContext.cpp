#include "pch.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "Character.h"
#include "CharacterParts.h"
#include "MeshReference.h"
#include "MaterialReference.h"

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
	top->m_World._11 = 200*1.43;
	top->m_World._22 = 1;
	top->m_World._33 = 150 * 1.43;
	XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
	top->m_World._41 = 180;
	top->m_World._42 = 90;
	top->m_World._43 = 100;
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
	skyRitem->m_MaterialIndex = 0;
	skyRitem->m_IsVisible = true;
	//skyRitem->m_MaterialIndex = MeshReference::GetApp()->m_Materials[matName]->DiffuseSrvHeapIndex;
	skyRitem->m_World = MathHelper::Identity4x4();
	skyRitem->m_TexTransform = MathHelper::Identity4x4();
	skyRitem->Scale(3000, 3000, 3000);
}

void ApplicationContext::CreateDebugBoundingBox(std::string boundsName, std::string boundsInstName)
{
	GameObject* item = CreateObject<GameObject>(boundsName, boundsInstName);
	item->Geo = MeshReference::GetApp()->m_GeometryMesh[boundsName].get();
	item->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	item->IndexCount = item->Geo->DrawArgs[boundsName].IndexCount;
	item->StartIndexLocation = item->Geo->DrawArgs[boundsName].StartIndexLocation;
	item->BaseVertexLocation = item->Geo->DrawArgs[boundsName].BaseVertexLocation;
	item->m_IsVisible = false;
	item->m_MaterialIndex = 1;
}

bool BlockCheck(int idx) {
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
				instancingObj->m_MaterialIndex = 1;
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
				instancingObj->m_MaterialIndex = 1;
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
			top->m_MaterialIndex = 1;
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
			instancingObj->m_MaterialIndex = 1;
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

void ApplicationContext::CreateSnowmans()
{
	int RandomLocation[2] = { -1,-1 };

	for (int i = 0; i < 2; ++i) {		//76, 77
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

		/*if (SnowmanIndex[i] % 4) {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 5 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 =  SnowmanIndex[i] / 5 * distance - 15.0f;
			instancingObj->m_World._42 = 20;
			instancingObj->m_World._43 =  SnowmanIndex[i] % 5 * distance + 15.0f;
		}
		else {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 7 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 =  SnowmanIndex[i] / 5 * distance + 15.0f;
			instancingObj->m_World._42 = 20;
			instancingObj->m_World._43 =  SnowmanIndex[i] % 5 * distance; +15.0f;
		}*/
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
	Core::wave = Sea;
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
			//meshName = "husky";
			//instID = "husky" + std::to_string(i + 100);
			//matidx = 4;
			//bonidx = BoneIndex::Husky;
			meshName = "ArcticFox";
			instID = "ArcticFox" + std::to_string(i + 100);
			matidx = 10;
			bonidx = BoneIndex::Fox;
		}
		else if (i / 10 == 1) {
			//meshName = "Penguin_LOD0skin";
			//instID = "Penguin_LOD0skin" + std::to_string(i + 100);
			//matidx = 2;
			//bonidx = BoneIndex::Penguin;
			meshName = "Seal";
			instID = "Seal" + std::to_string(i + 100);
			matidx = 12;
			bonidx = BoneIndex::Seal;
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
		chr->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts[meshName].get();
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
		XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
		top->m_World._41 = (size * 1.1) * i;
		top->m_World._42 = 300;
		top->m_World._43 = 400;
		top->m_TexTransform = MathHelper::Identity4x4();
	}

	//139~144	밧줄
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
		XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
		top->m_World._41 = (size * 1.1) * i;
		top->m_World._42 = 300;
		top->m_World._43 = 400;
		top->m_TexTransform = MathHelper::Identity4x4();
	}

	//145~148	산
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

	//149	텐트
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

	//150 카약
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

	//151~152	패들
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

	//153~192	돌
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

	//193	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("igloo", "igloo" + std::to_string(0));
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

	//194~197	텐트 생선
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

	//198 썰매
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

	//199~209 물고기 걸이, 물고기
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
