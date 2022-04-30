#include "pch.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "Character.h"
#include "CharacterParts.h"
#include "MeshReference.h"

#include <random>



random_device rd1;
default_random_engine dre2(rd1());
uniform_int_distribution<> uid5{ 0,8 }; //눈사람 위치
uniform_int_distribution<> uid4{ 0,3 }; //블록 덮개 회전
uniform_int_distribution<> uid6{ 15,25 }; //배경 캐릭터 크기
uniform_int_distribution<> uid7{ -25,25 }; //배경 캐릭터 회전
uniform_int_distribution<> uid8{ 0,4 }; //배경 캐릭터 위치
uniform_int_distribution<> uid9{ -430,430 }; //배경 캐릭터 반경

using namespace Core;

#define SCALE 0.5

std::string ApplicationContext::FindAnimName(int animCode) const
{
	std::string animName;

	// 애니메이션 상수 string -> 디파인값으로 바꿀예정
	switch (animCode)
	{
	case ANIM_IDLE:
		animName = "Idle";
		break;
	case ANIM_FORWARD:
		animName = "Forward";
		break;
	case ANIM_ATTACK:
		animName = "Attack";
		break;
	case ANIM_RUNNING:
		animName = "Running";
		break;
	case ANIM_JUMP:
		animName = "jump";
		break;
	default:
		cout << "None Anim Code" << endl;
		break;
	}

	return animName;
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
	item->m_IsVisible = true;
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
			top->m_World._42 = 0;
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
			instancingObj->m_World._11 = 0.5;
			instancingObj->m_World._22 = 0.4;
			instancingObj->m_World._33 = 0.5;
			instancingObj->m_World._41 = distance * i;
			instancingObj->m_World._42 = 0;
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
		instancingObj->m_World = MathHelper::Identity4x4();
		instancingObj->m_World._11 = SCALE - 0.2f;
		instancingObj->m_World._22 = SCALE - 0.2f;
		instancingObj->m_World._33 = SCALE - 0.2f;
		RandomLocation[i] = uid5(dre2);
		while (i == 1 && RandomLocation[0] == RandomLocation[1]) {
			RandomLocation[i] = uid5(dre2);
		}
		SnowmanIndex[i] = SnowmanLocaArray[RandomLocation[i]];
		if (SnowmanIndex[i] % 4) {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 5 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = SnowmanIndex[i] / 5 * distance - 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = SnowmanIndex[i] % 5 * distance + 15.0f;
		}
		else {
			XMStoreFloat4x4(&instancingObj->m_World, XMLoadFloat4x4(&instancingObj->m_World) * XMMatrixRotationY(3.14 * 7 / 6));
			int distance = SCALE * 200;
			instancingObj->m_World._41 = SnowmanIndex[i] / 5 * distance + 15.0f;
			instancingObj->m_World._42 = 10;
			instancingObj->m_World._43 = SnowmanIndex[i] % 5 * distance; +15.0f;
		}
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

	Sea->m_World._11 = 7;
	Sea->m_World._33 = 7;

	Sea->m_World._41 = SCALE * 400;
	Sea->m_World._42 = -SCALE * 100;
	Sea->m_World._43 = SCALE * 400;

	Sea->m_TexTransform = MathHelper::Identity4x4();
	Core::wave = Sea;
}

void ApplicationContext::CreateBackground()
{
	float X[5] = { -650,200,1050,-650,1050 };		//배경 블록 위치
	float Z[5] = { 1050,1050,1050,200,200 };
	float size = 8.5;

	//79~83
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
		instancingObj->m_World._11 = size;
		instancingObj->m_World._22 = size;
		instancingObj->m_World._33 = size;

		instancingObj->m_World._41 = X[i];
		instancingObj->m_World._42 = -400;
		instancingObj->m_World._43 = Z[i];
		instancingObj->m_TexTransform = MathHelper::Identity4x4();

		instancingObj->m_Bounds.Center = MathHelper::Add(instancingObj->Geo->DrawArgs["snowcube"].Bounds.Center, instancingObj->GetPosition());
	}

	//84~133
	for (int i = 0; i < 50; ++i) {
		std::string meshName = "husky";
		std::string instID = "husky" + std::to_string(i + 1);
		Character* chr = CreateObject<Character>(meshName, instID);
		chr->Geo = MeshReference::GetApp()->m_GeometryMesh[meshName].get();
		chr->IndexCount = chr->Geo->DrawArgs[meshName].IndexCount;
		chr->StartIndexLocation = chr->Geo->DrawArgs[meshName].StartIndexLocation;
		chr->BaseVertexLocation = chr->Geo->DrawArgs[meshName].BaseVertexLocation;
		chr->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		chr->m_Bounds = chr->Geo->DrawArgs["husky"].Bounds;
		chr->m_MaterialIndex = 4;
		chr->m_SkinnedCBIndex = BoneIndex::Husky;
		chr->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts[meshName].get();
		chr->m_IsVisible = true;
		// 임시 스폰위치 지정
		//chr->m_SpawnLoaction = skinnedCBIndex;
		int XPos, ZPos;
		int tmp = uid6(dre2);
		chr->Scale(tmp, tmp, tmp);
		do {
			tmp = uid8(dre2);
			XPos = X[tmp] + uid9(dre2);
			ZPos = Z[tmp] + uid9(dre2);
		} while (XPos < -400 || XPos>800 || ZPos > 1100 || ZPos < 0);

		if (XPos <= 200) {	//항상 중심 바라보게
			chr->Rotate(0, 180 - ((200 - XPos) * 45 / 600 + (1100 - ZPos) * 45 / 1100), 0);
		}
		else {
			chr->Rotate(0, 180 + ((XPos - 200) * 45 / 600 + (1100 - ZPos) * 45 / 1100), 0);
		}
		chr->SetPosition(XPos, 60, ZPos);
	}

	//134~138
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
		top->m_World._11 = 50;
		top->m_World._22 = 50;
		top->m_World._33 = 50;
		XMStoreFloat4x4(&top->m_World, XMLoadFloat4x4(&top->m_World) * XMMatrixRotationX(3.141592 * -0.5));
		top->m_World._41 = 55 * i;
		top->m_World._42 = 300;
		top->m_World._43 = 400;
		top->m_TexTransform = MathHelper::Identity4x4();
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
	chr->m_MaterialIndex = 4;
	chr->m_SkinnedCBIndex = skinnedCBIndex;
	chr->m_SkinnedModelInst = MeshReference::GetApp()->m_SkinnedModelInsts[meshName].get();
	chr->m_IsVisible = false;
	// 임시 스폰위치 지정
	//chr->m_SpawnLoaction = skinnedCBIndex;
	chr->Scale(20, 20, 20);
	chr->SetPosition(250, 30, 0);

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
