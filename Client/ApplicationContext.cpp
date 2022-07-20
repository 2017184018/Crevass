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
			meshName = "Penguin";
			instID = "Penguin" + std::to_string(i + 100);
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
		if (i % 5 < 5)
			chr->m_IsVisible = true;
		else
			chr->m_IsVisible = false;
		int XPos, ZPos;
		int size = uid6(dre2);
		chr->Scale(20, 20, 20);
		int tmp = 0;
		float tmp2 = 0;
	
		chr->Rotate(0, 90, 0);
		chr->SetPosition(-1000,-1000,-1000);
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
		husky->m_IsVisible = false;
		husky->m_MaterialIndex = 1; 
		husky->m_SkinnedCBIndex = BoneIndex::Husky;
		husky->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get();
		husky->m_World = MathHelper::Identity4x4();

		Character* penguin = CreateObject<Character>("PenguinOutline", "PenguinOutline0");
		penguin->Geo = MeshReference::GetApp()->m_GeometryMesh["Penguin"].get();
		penguin->IndexCount = penguin->Geo->DrawArgs["Penguin"].IndexCount;
		penguin->StartIndexLocation = penguin->Geo->DrawArgs["Penguin"].StartIndexLocation;
		penguin->BaseVertexLocation = penguin->Geo->DrawArgs["Penguin"].BaseVertexLocation;
		penguin->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		penguin->m_Bounds = penguin->Geo->DrawArgs["Penguin"].Bounds;
		penguin->m_IsVisible = false;
		penguin->m_MaterialIndex = 1; 
		penguin->m_SkinnedCBIndex = BoneIndex::Penguin;
		penguin->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts["Penguin"].get();
		penguin->m_World = MathHelper::Identity4x4();

		Character* arcticfox = CreateObject<Character>("ArcticFoxOutline", "ArcticFoxOutline0");
		arcticfox->Geo = MeshReference::GetApp()->m_GeometryMesh["ArcticFox"].get();
		arcticfox->IndexCount = arcticfox->Geo->DrawArgs["ArcticFox"].IndexCount;
		arcticfox->StartIndexLocation = arcticfox->Geo->DrawArgs["ArcticFox"].StartIndexLocation;
		arcticfox->BaseVertexLocation = arcticfox->Geo->DrawArgs["ArcticFox"].BaseVertexLocation;
		arcticfox->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		arcticfox->m_Bounds = arcticfox->Geo->DrawArgs["ArcticFox"].Bounds;
		arcticfox->m_IsVisible = false;
		arcticfox->m_MaterialIndex = 1; 
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
		polarbear->m_IsVisible = false;
		polarbear->m_MaterialIndex = 1; 
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
		seal->m_IsVisible = false;
		seal->m_MaterialIndex = 1; 
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

void ApplicationContext::CreatelobbyBlocks()
{
	int distance = SCALE * 200;

	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = CreateObject<GameObject>("lobby_icecube", "lobby_icecube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_icecube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_icecube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_icecube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_icecube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_icecube"].Bounds;
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

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["lobby_icecube"].Bounds.Center, instancingObj->GetPosition());
			}
			else {
				instancingObj = CreateObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(5 * i + j));
				instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_snowcube"].get();
				instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_snowcube"].IndexCount;
				instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_snowcube"].StartIndexLocation;
				instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_snowcube"].BaseVertexLocation;
				instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_snowcube"].Bounds;
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

				instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["lobby_snowcube"].Bounds.Center, instancingObj->GetPosition());
			}

			GameObject* top = CreateObject<GameObject>("lobby_snow_top", "lobby_snow_top" + std::to_string(5 * i + j));
			top->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_snow_top"].get();
			top->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			top->IndexCount = top->Geo->DrawArgs["lobby_snow_top"].IndexCount;
			top->StartIndexLocation = top->Geo->DrawArgs["lobby_snow_top"].StartIndexLocation;
			top->BaseVertexLocation = top->Geo->DrawArgs["lobby_snow_top"].BaseVertexLocation;
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
}

void ApplicationContext::CreatelobbyBackground()
{
	
	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(25 + i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_snowcube"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_snowcube"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_snowcube"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_snowcube"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_snowcube"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = 1;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
		instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["lobby_snowcube"].Bounds.Center, instancingObj->GetPosition());
	}



	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_mountain", "lobby_mountain" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_mountain"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_mountain"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_mountain"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_mountain"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_mountain"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();

		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_tent", "lobby_tent" + std::to_string(0));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_tent"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_tent"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_tent"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_tent"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_tent"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_kayak", "lobby_kayak" + std::to_string(0));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_kayak"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_kayak"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_kayak"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_kayak"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_kayak"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_kayakpaddle", "lobby_kayakpaddle" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_kayakpaddle"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_kayakpaddle"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_kayakpaddle"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_kayakpaddle"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_kayakpaddle"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = CreateObject<GameObject>("lobby_rock_0", "lobby_rock_0" + std::to_string(i));
		instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_rock_0"].get();
		instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_rock_0"].IndexCount;
		instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_rock_0"].StartIndexLocation;
		instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_rock_0"].BaseVertexLocation;
		instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_rock_0"].Bounds;
		instancingObj->m_IsVisible = false;
		instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_TexTransform = MathHelper::Identity4x4();
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("lobby_rock_1", "lobby_rock_1" + std::to_string(i));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_rock_1"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["lobby_rock_1"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["lobby_rock_1"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["lobby_rock_1"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["lobby_rock_1"].Bounds;
		instancingObj2->m_IsVisible = false;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("lobby_igloo", "lobby_igloo" + std::to_string(2));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_igloo"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["lobby_igloo"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["lobby_igloo"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["lobby_igloo"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["lobby_igloo"].Bounds;
		instancingObj2->m_IsVisible = false;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(i));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_fish"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["lobby_fish"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["lobby_fish"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["lobby_fish"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["lobby_fish"].Bounds;
		instancingObj2->m_IsVisible = false;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("lobby_sled", "lobby_sled" + std::to_string(0));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_sled"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["lobby_sled"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["lobby_sled"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["lobby_sled"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["lobby_sled"].Bounds;
		instancingObj2->m_IsVisible = false;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = CreateObject<GameObject>("lobby_fishrack", "lobby_fishrack" + std::to_string(0));
		instancingObj2->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_fishrack"].get();
		instancingObj2->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		instancingObj2->IndexCount = instancingObj2->Geo->DrawArgs["lobby_fishrack"].IndexCount;
		instancingObj2->StartIndexLocation = instancingObj2->Geo->DrawArgs["lobby_fishrack"].StartIndexLocation;
		instancingObj2->BaseVertexLocation = instancingObj2->Geo->DrawArgs["lobby_fishrack"].BaseVertexLocation;
		instancingObj2->m_Bounds = instancingObj2->Geo->DrawArgs["lobby_fishrack"].Bounds;
		instancingObj2->m_IsVisible = false;
		instancingObj2->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
		instancingObj2->m_World = MathHelper::Identity4x4();
		instancingObj2->m_TexTransform = MathHelper::Identity4x4();

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = CreateObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(4 + i));
			instancingObj->Geo = MeshReference::GetApp()->m_GeometryMesh["lobby_fish"].get();
			instancingObj->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			instancingObj->IndexCount = instancingObj->Geo->DrawArgs["lobby_fish"].IndexCount;
			instancingObj->StartIndexLocation = instancingObj->Geo->DrawArgs["lobby_fish"].StartIndexLocation;
			instancingObj->BaseVertexLocation = instancingObj->Geo->DrawArgs["lobby_fish"].BaseVertexLocation;
			instancingObj->m_Bounds = instancingObj->Geo->DrawArgs["lobby_fish"].Bounds;
			instancingObj->m_IsVisible = false;
			instancingObj->m_MaterialIndex = MaterialReference::GetApp()->m_Materials["arctic"]->DiffuseSrvHeapIndex;
			instancingObj->m_World = MathHelper::Identity4x4();
			instancingObj->m_TexTransform = MathHelper::Identity4x4();
		}
	}
}

void ApplicationContext::HiddenOutline()
{
	Character* husky = CreateObject<Character>("huskyOutline", "huskyOutline0");
	if (!husky)
		return;
	ZeroMemory(&husky->m_World, sizeof(husky->m_World));
	ZeroMemory(&husky->m_TexTransform, sizeof(husky->m_TexTransform));

	Character* penguin = CreateObject<Character>("PenguinOutline", "PenguinOutline0");
	if (!penguin)
		return;
	ZeroMemory(&penguin->m_World, sizeof(penguin->m_World));
	ZeroMemory(&penguin->m_TexTransform, sizeof(penguin->m_TexTransform));

	Character* arcticfox = CreateObject<Character>("ArcticFoxOutline", "ArcticFoxOutline0");
	if (!arcticfox)
		return;
	ZeroMemory(&arcticfox->m_World, sizeof(arcticfox->m_World));
	ZeroMemory(&arcticfox->m_TexTransform, sizeof(arcticfox->m_TexTransform));

	Character* polarbear = CreateObject<Character>("PolarBearOutline", "PolarBearOutline0");
	if (!polarbear)
		return;
	ZeroMemory(&polarbear->m_World, sizeof(polarbear->m_World));
	ZeroMemory(&polarbear->m_TexTransform, sizeof(polarbear->m_TexTransform));

	Character* seal = CreateObject<Character>("SealOutline", "SealOutline0");
	if (!seal)
		return;
	ZeroMemory(&seal->m_World, sizeof(seal->m_World));
	ZeroMemory(&seal->m_TexTransform, sizeof(seal->m_TexTransform));

	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["huskyOutline"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["PenguinOutline"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["ArcticFoxOutline"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["PolarBearOutline"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["SealOutline"], m_RItemsVec);

	husky->m_IsVisible = false;

	penguin->m_IsVisible = false;

	arcticfox->m_IsVisible = false;

	polarbear->m_IsVisible = false;

	seal->m_IsVisible = false;
}

void ApplicationContext::HiddenMinimap()
{
	{	//222
		GameObject* Sea = FindObject<GameObject>("MinimapSea", "MinimapSea0");
		if (!Sea)
			return;
		ZeroMemory(&Sea->m_World, sizeof(Sea->m_World));
		ZeroMemory(&Sea->m_TexTransform, sizeof(Sea->m_TexTransform));
	}

	for (int i = 0; i < 5; ++i) {	//223~272
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(5 * i + j));
				if (!instancingObj)
					return;
				ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
				ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
			}
			else {
				instancingObj = FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(5 * i + j));
				if (!instancingObj)
					return;
				ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
				ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
			}
			
			GameObject* top = FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(5 * i + j));
			if (!top)
				return;
			ZeroMemory(&top->m_World, sizeof(top->m_World));
			ZeroMemory(&top->m_TexTransform, sizeof(top->m_TexTransform));
		}
	}

	for (int i = 0; i < 5; ++i) {	//273~297
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(5 * i + j));
			if (!instancingObj)
				return;
			ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
			ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
		}
	}

	{
		//298
		GameObject* icon = FindObject<GameObject>("myicon", "myicon0");
		if (!icon)
			return;
		ZeroMemory(&icon->m_World, sizeof(icon->m_World));
		ZeroMemory(&icon->m_TexTransform, sizeof(icon->m_TexTransform));
	}

	{
		//299~302
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = FindObject<GameObject>("snowmanicon", "snowmanicon" + std::to_string(i));
			if (!icon)
				return;
			ZeroMemory(&icon->m_World, sizeof(icon->m_World));
			ZeroMemory(&icon->m_TexTransform, sizeof(icon->m_TexTransform));
		}
	}
	{
		//303~304
		for (int i = 0; i < 2; ++i) {
			GameObject* icon = FindObject<GameObject>("iglooicon", "iglooicon" + std::to_string(i));
			if (!icon)
				return;
			ZeroMemory(&icon->m_World, sizeof(icon->m_World));
			ZeroMemory(&icon->m_TexTransform, sizeof(icon->m_TexTransform));
		}
	}
	{
		//305~308
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = FindObject<GameObject>("enermyicon", "enermyicon" + std::to_string(i));
			if (!icon)
				return;
			ZeroMemory(&icon->m_World, sizeof(icon->m_World));
			ZeroMemory(&icon->m_TexTransform, sizeof(icon->m_TexTransform));
		}
	}

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["MinimapSea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapicecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapsnowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapicicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Minimapsnow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["myicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowmanicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["iglooicon"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["enermyicon"], AppContext->m_RItemsVec);

	{	//222
		GameObject* Sea = FindObject<GameObject>("MinimapSea", "MinimapSea0");
		if (!Sea)
			return;
		ZeroMemory(&Sea->m_World, sizeof(Sea->m_World));
		ZeroMemory(&Sea->m_TexTransform, sizeof(Sea->m_TexTransform));
	}

	for (int i = 0; i < 5; ++i) {	//223~272
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = FindObject<GameObject>("Minimapicecube", "Minimapicecube" + std::to_string(5 * i + j));
				instancingObj->m_IsVisible = false;
			}
			else {
				instancingObj = FindObject<GameObject>("Minimapsnowcube", "Minimapsnowcube" + std::to_string(5 * i + j));
				instancingObj->m_IsVisible = false;
			}

			GameObject* top = FindObject<GameObject>("Minimapsnow_top", "Minimapsnow_top" + std::to_string(5 * i + j));
			top->m_IsVisible = false;
		}
	}

	for (int i = 0; i < 5; ++i) {	//273~297
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj = FindObject<GameObject>("Minimapicicle", "Minimapicicle" + std::to_string(5 * i + j));
			instancingObj->m_IsVisible = false;
		}
	}

	{
		//298
		GameObject* icon = FindObject<GameObject>("myicon", "myicon0");
		icon->m_IsVisible = false;
	}

	{
		//299~302
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = FindObject<GameObject>("snowmanicon", "snowmanicon" + std::to_string(i));
			icon->m_IsVisible = false;
		}
	}
	{
		//303~304
		for (int i = 0; i < 2; ++i) {
			GameObject* icon = FindObject<GameObject>("iglooicon", "iglooicon" + std::to_string(i));
			icon->m_IsVisible = false;
		}
	}
	{
		//305~308
		for (int i = 0; i < 4; ++i) {
			GameObject* icon = FindObject<GameObject>("enermyicon", "enermyicon" + std::to_string(i));
			icon->m_IsVisible = false;
		}
	}

}

void ApplicationContext::HiddenSnowmans()
{
	for (int i = 0; i < 4; ++i) {		//209~212
		GameObject* instancingObj = FindObject<GameObject>("snowman", "snowman" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowman"], AppContext->m_RItemsVec);

	for (int i = 0; i < 4; ++i) {		//209~212
		GameObject* instancingObj = FindObject<GameObject>("snowman", "snowman" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}

}


void ApplicationContext::HiddenWaterDrop()
{
	for (int i = 0; i < 4; ++i) {
		GameObject* obj = FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i));
		if (!obj)
			return;
		ZeroMemory(&obj->m_World, sizeof(obj->m_World));
		ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
	}
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["waterdrop"], AppContext->m_RItemsVec);
	for (int i = 0; i < 4; ++i) {
		GameObject* obj = FindObject<GameObject>("waterdrop", "waterdrop" + std::to_string(i));
		obj->m_IsVisible = false;
	}
}

void ApplicationContext::HiddenHail()
{
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("rock_1", "rock_1" + std::to_string(i + 15));
		if (!obj)
			return;
		ZeroMemory(&obj->m_World, sizeof(obj->m_World));
		ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));

	}
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("rock_1", "rock_1" + std::to_string(i + 15));
		obj->m_IsVisible = false;

	}
}

void ApplicationContext::Hiddenigloos()
{
	for (int i = 0; i < 2; ++i) {		//76, 77
		GameObject* obj = FindObject<GameObject>("igloo", "igloo" + std::to_string(i));
		if (!obj)
			return;
		ZeroMemory(&obj->m_World, sizeof(obj->m_World));
		ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));

	}

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["igloo"], AppContext->m_RItemsVec);

	for (int i = 0; i < 2; ++i) {		//76, 77
		GameObject* obj = FindObject<GameObject>("igloo", "igloo" + std::to_string(i));
		obj->m_IsVisible = false;
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
//	XMStoreFloat4x4(&chr->m_World, XMLoadFloat4x4(&chr->m_World) * XMMatrixRotationY(3.141592));
	
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

void ApplicationContext::HiddenBackground()
{
	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("snowcube", "snowcube" + std::to_string(25 + i));
		if (!obj)
			return;
		ZeroMemory(&obj->m_World, sizeof(obj->m_World));
		ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
	}

	//134~138	라이프
	for (int i = 0; i < 5; ++i) {
		GameObject* top = FindObject<GameObject>("life", "life" + std::to_string(i));
		if (!top)
			return;
		ZeroMemory(&top->m_World, sizeof(top->m_World));
		ZeroMemory(&top->m_TexTransform, sizeof(top->m_TexTransform));
	}

	//139~143	밧줄
	for (int i = 0; i < 5; ++i) {
		GameObject* top = FindObject<GameObject>("lifeline", "lifeline" + std::to_string(i));
		if (!top)
			return;
		ZeroMemory(&top->m_World, sizeof(top->m_World));
		ZeroMemory(&top->m_TexTransform, sizeof(top->m_TexTransform));
	}

	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj = FindObject<GameObject>("mountain", "mountain" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("tent", "tent" + std::to_string(0));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("kayak", "kayak" + std::to_string(0));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("kayakpaddle", "kayakpaddle" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("rock_0", "rock_0" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("rock_1", "rock_1" + std::to_string(i));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("igloo", "igloo" + std::to_string(2));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("fish", "fish" + std::to_string(i));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("sled", "sled" + std::to_string(0));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("fishrack", "fishrack" + std::to_string(0));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("fish", "fish" + std::to_string(4 + i));
			if (!instancingObj)
				return;
			ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
			ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
		}
	}

	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["life"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lifeline"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["mountain"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["tent"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["kayak"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["kayakpaddle"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["rock_0"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["rock_1"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["igloo"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["sled"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["fishrack"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["fish"], m_RItemsVec);

	

	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("snowcube", "snowcube" + std::to_string(25 + i));
		obj->m_IsVisible = false;
	}

	//134~138	라이프
	for (int i = 0; i < 5; ++i) {
		GameObject* top = FindObject<GameObject>("life", "life" + std::to_string(i));
		top->m_IsVisible = false;
	}

	//139~143	밧줄
	for (int i = 0; i < 5; ++i) {
		GameObject* top = FindObject<GameObject>("lifeline", "lifeline" + std::to_string(i));
		top->m_IsVisible = false;
	}

	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj = FindObject<GameObject>("mountain", "mountain" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("tent", "tent" + std::to_string(0));
		instancingObj->m_IsVisible = false;
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("kayak", "kayak" + std::to_string(0));
		instancingObj->m_IsVisible = false;
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("kayakpaddle", "kayakpaddle" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("rock_0", "rock_0" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("rock_1", "rock_1" + std::to_string(i));
		instancingObj2->m_IsVisible = false;
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("igloo", "igloo" + std::to_string(2));
		instancingObj2->m_IsVisible = false;
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("fish", "fish" + std::to_string(i));
		instancingObj2->m_IsVisible = false;
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("sled", "sled" + std::to_string(0));
		instancingObj2->m_IsVisible = false;
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("fishrack", "fishrack" + std::to_string(0));
		if (!instancingObj2)
			return;
		instancingObj2->m_IsVisible = false;

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("fish", "fish" + std::to_string(4 + i));
			instancingObj->m_IsVisible = false;
		}
	}

}

void ApplicationContext::DisplaylobbyBackground()
{
	float X[5] = { -950,400,1750,-950,1750 };		//배경 블록 위치
	float Z[5] = { 1800,1800,1800,400,400 };

	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(25 + i));
		instancingObj->InitializeTransform();
		instancingObj->m_IsVisible = true;
		float size = 13;
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;
		instancingObj->m_World._41 = X[i];
		instancingObj->m_World._42 = -640;
		instancingObj->m_World._43 = Z[i];



		//144~147	산
		for (int i = 0; i < 4; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_mountain", "lobby_mountain" + std::to_string(i));
			instancingObj->InitializeTransform();
			instancingObj->m_IsVisible = true;
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
		}

		//148	텐트
		{
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_tent", "lobby_tent" + std::to_string(0));
			instancingObj->InitializeTransform();
			instancingObj->m_IsVisible = true;
			float size = 0.7;
			instancingObj->m_World._11 = size;
			instancingObj->m_World._22 = size;
			instancingObj->m_World._33 = size;
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * 8 / 10));
			instancingObj->m_World._41 = X[1] - 450;
			instancingObj->m_World._42 = 50;
			instancingObj->m_World._43 = Z[1] - 400;
		}

		//149 카약
		{
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_kayak", "lobby_kayak" + std::to_string(0));
			instancingObj->InitializeTransform();
			instancingObj->m_IsVisible = true;
			float size = 0.7;
			instancingObj->m_World._11 = size;
			instancingObj->m_World._22 = size;
			instancingObj->m_World._33 = size;
			instancingObj->m_World._41 = X[1] - 350;
			instancingObj->m_World._42 = 50;
			instancingObj->m_World._43 = Z[1] - 700;
		}

		//150~151	패들
		for (int i = 0; i < 2; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_kayakpaddle", "lobby_kayakpaddle" + std::to_string(i));
			instancingObj->InitializeTransform();
			instancingObj->m_IsVisible = true;
			float size = 0.7;
			instancingObj->m_World._11 = size;
			instancingObj->m_World._22 = size;
			instancingObj->m_World._33 = size;
			instancingObj->m_World._41 = X[1] - 330 + 20 * (i + 1);
			instancingObj->m_World._42 = 50;
			instancingObj->m_World._43 = Z[1] - 680;
		}

		//152~191	돌
		for (int i = 0; i < 25; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_rock_0", "lobby_rock_0" + std::to_string(i));
			instancingObj->InitializeTransform();
			instancingObj->m_IsVisible = true;
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
		}
		for (int i = 0; i < 15; ++i) {
			GameObject* instancingObj2;
			instancingObj2 = FindObject<GameObject>("lobby_rock_1", "lobby_rock_1" + std::to_string(i));
			instancingObj2->InitializeTransform();
			instancingObj2->m_IsVisible = true;
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
		}

		//192	이글루
		{
			GameObject* instancingObj2;
			instancingObj2 = FindObject<GameObject>("lobby_igloo", "lobby_igloo" + std::to_string(2));
			instancingObj2->InitializeTransform();
			instancingObj2->m_IsVisible = true;
			float size = 0.6;
			instancingObj2->m_World._11 = size;
			instancingObj2->m_World._22 = size;
			instancingObj2->m_World._33 = size;
			XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592));
			instancingObj2->m_World._41 = X[1] + 100;
			instancingObj2->m_World._42 = 50;
			instancingObj2->m_World._43 = Z[1] - 500;
		}

		//193~196	텐트 생선
		for (int i = 0; i < 4; ++i) {
			GameObject* instancingObj2;
			instancingObj2 = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(i));
			instancingObj2->InitializeTransform();
			instancingObj2->m_IsVisible = true;
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
		}

		//197 썰매
		{
			GameObject* instancingObj2;
			instancingObj2 = FindObject<GameObject>("lobby_sled", "lobby_sled" + std::to_string(0));
			instancingObj2->InitializeTransform();
			instancingObj2->m_IsVisible = true;
			float size = 1;
			instancingObj2->m_World._11 = size;
			instancingObj2->m_World._22 = size;
			instancingObj2->m_World._33 = size;
			XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 7 / 10));
			instancingObj2->m_World._41 = X[1] - 500;
			instancingObj2->m_World._42 = 30;
			instancingObj2->m_World._43 = Z[0] - 650;
		}

		//198~208 물고기 걸이, 물고기
		{
			GameObject* instancingObj2;
			instancingObj2 = FindObject<GameObject>("lobby_fishrack", "lobby_fishrack" + std::to_string(0));
			instancingObj2->InitializeTransform();
			instancingObj2->m_IsVisible = true;
			float size = 1;
			instancingObj2->m_World._11 = size;
			instancingObj2->m_World._22 = size;
			instancingObj2->m_World._33 = size;
			XMStoreFloat4x4(&instancingObj2->m_World, XMLoadFloat4x4(&instancingObj2->m_World) * XMMatrixRotationY(3.141592 * 7 / 10));
			instancingObj2->m_World._41 = X[0] + 520;
			instancingObj2->m_World._42 = 30;
			instancingObj2->m_World._43 = 1170;

			for (int i = 0; i < 10; ++i) {
				GameObject* instancingObj;
				instancingObj = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(4 + i));
				instancingObj->InitializeTransform();
				instancingObj->m_IsVisible = true;
				float size = 0.7;
				instancingObj->m_World._11 = size;
				instancingObj->m_World._22 = size;
				instancingObj->m_World._33 = size;
				XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.141592 * 11 / 10));
				XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationX(3.141592 * 1.5 / 10));
				instancingObj->m_World._41 = X[0] + 470 + 11 * i;
				instancingObj->m_World._42 = 180;
				instancingObj->m_World._43 = 1110 + 15 * i;
			}
		}
	}
}

void ApplicationContext::DisplaylobbyBlocks()
{
	int distance = SCALE * 200;

	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {
			GameObject* instancingObj;
			if (BlockCheck(5 * i + j)) {
				instancingObj = FindObject<GameObject>("lobby_icecube", "lobby_icecube" + std::to_string(5 * i + j));
				instancingObj->InitializeTransform();
				instancingObj->m_IsVisible = true;
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;
				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;

			}
			else {
				instancingObj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(5 * i + j));
				instancingObj->InitializeTransform();
				instancingObj->m_IsVisible = true;
				instancingObj->m_World._11 = SCALE;
				instancingObj->m_World._22 = SCALE;
				instancingObj->m_World._33 = SCALE;

				instancingObj->m_World._41 = distance * i;
				instancingObj->m_World._42 = -30;
				instancingObj->m_World._43 = distance * j;
			}

			GameObject* top = FindObject<GameObject>("lobby_snow_top", "lobby_snow_top" + std::to_string(5 * i + j));
			top->InitializeTransform();
			top->m_IsVisible = true;
			top->m_World._11 = SCALE;
			top->m_World._22 = SCALE;
			top->m_World._33 = SCALE;
			XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationY(3.141592 * uid4(dre2)));
			top->m_World._41 = distance * i;
			top->m_World._42 = -30;
			top->m_World._43 = distance * j;
		}
	}
}

void ApplicationContext::HiddenlobbyBackground()
{
	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(25 + i));
		if (!obj)
			return;
		ZeroMemory(&obj->m_World, sizeof(obj->m_World));
		ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
	}


	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj = FindObject<GameObject>("lobby_mountain", "lobby_mountain" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_tent", "lobby_tent" + std::to_string(0));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_kayak", "lobby_kayak" + std::to_string(0));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_kayakpaddle", "lobby_kayakpaddle" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_rock_0", "lobby_rock_0" + std::to_string(i));
		if (!instancingObj)
			return;
		ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
		ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_rock_1", "lobby_rock_1" + std::to_string(i));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_igloo", "lobby_igloo" + std::to_string(2));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(i));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_sled", "lobby_sled" + std::to_string(0));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_fishrack", "lobby_fishrack" + std::to_string(0));
		if (!instancingObj2)
			return;
		ZeroMemory(&instancingObj2->m_World, sizeof(instancingObj2->m_World));
		ZeroMemory(&instancingObj2->m_TexTransform, sizeof(instancingObj2->m_TexTransform));

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(4 + i));
			if (!instancingObj)
				return;
			ZeroMemory(&instancingObj->m_World, sizeof(instancingObj->m_World));
			ZeroMemory(&instancingObj->m_TexTransform, sizeof(instancingObj->m_TexTransform));
		}
	}

	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_mountain"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_tent"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_kayak"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_kayakpaddle"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_rock_0"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_rock_1"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_igloo"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_sled"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_fishrack"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_fish"], m_RItemsVec);



	//79~83	바닥
	for (int i = 0; i < 5; ++i) {
		GameObject* obj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(25 + i));
		obj->m_IsVisible = false;
	}



	//144~147	산
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj = FindObject<GameObject>("lobby_mountain", "lobby_mountain" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}

	//148	텐트
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_tent", "lobby_tent" + std::to_string(0));
		instancingObj->m_IsVisible = false;
	}

	//149 카약
	{
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_kayak", "lobby_kayak" + std::to_string(0));
		instancingObj->m_IsVisible = false;
	}

	//150~151	패들
	for (int i = 0; i < 2; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_kayakpaddle", "lobby_kayakpaddle" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}

	//152~191	돌
	for (int i = 0; i < 25; ++i) {
		GameObject* instancingObj;
		instancingObj = FindObject<GameObject>("lobby_rock_0", "lobby_rock_0" + std::to_string(i));
		instancingObj->m_IsVisible = false;
	}
	for (int i = 0; i < 15; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_rock_1", "lobby_rock_1" + std::to_string(i));
		instancingObj2->m_IsVisible = false;
	}

	//192	이글루
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_igloo", "lobby_igloo" + std::to_string(2));
		instancingObj2->m_IsVisible = false;
	}

	//193~196	텐트 생선
	for (int i = 0; i < 4; ++i) {
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(i));
		instancingObj2->m_IsVisible = false;
	}

	//197 썰매
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_sled", "lobby_sled" + std::to_string(0));
		instancingObj2->m_IsVisible = false;
	}

	//198~208 물고기 걸이, 물고기
	{
		GameObject* instancingObj2;
		instancingObj2 = FindObject<GameObject>("lobby_fishrack", "lobby_fishrack" + std::to_string(0));
		if (!instancingObj2)
			return;
		instancingObj2->m_IsVisible = false;

		for (int i = 0; i < 10; ++i) {
			GameObject* instancingObj;
			instancingObj = FindObject<GameObject>("lobby_fish", "lobby_fish" + std::to_string(4 + i));
			instancingObj->m_IsVisible = false;
		}
	}
}

void ApplicationContext::HiddenlobbyBlocks()
{
	for (int i = 0; i < 5; ++i) {//1~50
		for (int j = 0; j < 5; ++j) {

			if (BlockCheck(5 * i + j)) {
				GameObject* obj = FindObject<GameObject>("lobby_icecube", "lobby_icecube" + std::to_string(5 * i + j));
				if (!obj)
					return;
				ZeroMemory(&obj->m_World, sizeof(obj->m_World));
				ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
			}
			else {
				GameObject* obj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(5 * i + j));
				if (!obj)
					return;
				ZeroMemory(&obj->m_World, sizeof(obj->m_World));
				ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
			}
			GameObject* obj = FindObject<GameObject>("lobby_snow_top", "lobby_snow_top" + std::to_string(5 * i + j));
			if (!obj)
				return;
			ZeroMemory(&obj->m_World, sizeof(obj->m_World));
			ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));

		}
	}

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_icecube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_snowcube"], m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap["lobby_snow_top"], m_RItemsVec);

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (BlockCheck(5 * i + j)) {
				GameObject* obj = FindObject<GameObject>("lobby_icecube", "lobby_icecube" + std::to_string(5 * i + j));
				obj->m_IsVisible = false;
			}
			else {
				GameObject* obj = FindObject<GameObject>("lobby_snowcube", "lobby_snowcube" + std::to_string(5 * i + j));
				obj->m_IsVisible = false;
			}
			GameObject* obj = FindObject<GameObject>("lobby_snow_top", "lobby_snow_top" + std::to_string(5 * i + j));
			obj->m_IsVisible = false;
		}
	}
}

void ApplicationContext::DisplayCharacter( Character* user, XMFLOAT3 pos,bool isVisible)
{
	
	if (!user) { cout << "return0000000000000000000000000" << endl;  return; };

			user->InitializeTransform();
			user->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
			user->SetAnimationPlayerState(Character::PlayerState::STATE_IDLE);
			user->m_IsVisible = isVisible;
			user->Scale(20.f, 20.f, 20.f);
			user->SetPosition(pos);
		
			
}

void ApplicationContext::HiddenCharacter(std::string userName, std::string insname)
{
	Character* user = FindObject<Character>(userName, insname);
	if (!user) { cout << userName << "return" << endl; return; }

	user->m_MyCamera = nullptr;
	user->SetAnimationPlayerState(Character::PlayerState::STATE_IDLE);
	user->SetAnimationKeyState(Character::PlayerState::STATE_IDLE);
	user->m_PlayerController.release();

	ZeroMemory(&user->m_World, sizeof(user->m_World));
	ZeroMemory(&user->m_TexTransform, sizeof(user->m_TexTransform));
    user->ReleaseTransform();

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap[userName], AppContext->m_RItemsVec);
	user->m_IsVisible = false;
}

void ApplicationContext::DisplayUI(std::string mapName)
{
}

void ApplicationContext::HiddenUI(std::string uiName, std::string instname)
{
	GameObject* obj = FindObject<GameObject>(uiName, instname);
	if (!obj) {
		cout << "return" << endl;
		return;
	}
	ZeroMemory(&obj->m_World, sizeof(obj->m_World));
	ZeroMemory(&obj->m_TexTransform, sizeof(obj->m_TexTransform));
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap[uiName], AppContext->m_RItemsVec);
	obj->m_IsVisible = false;


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
	if (!ptc) { cout << "return1111111" << endl;  return; }
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
	
	ptc->StopParticle();

	// Update InstanceData
	GraphicsContext::GetApp()->UpdateInstanceData(m_RItemsMap[particleName], m_RItemsVec);
	ptc->m_IsVisible = false;

}
