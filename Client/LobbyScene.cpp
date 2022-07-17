#include "pch.h"
#include "LobbyScene.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "MaterialReference.h"
#include "MeshReference.h"

#include "Character.h"
#include "CharacterParts.h"

#include "CREVASS.h"
#include "Network.h"

void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
	//	AppContext->CreateLobby();
	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreatelobbyBlocks();
	AppContext->CreateWave();
	AppContext->CreatelobbyBackground();

}


void LobbyScene::OnResize()
{
}

bool LobbyScene::Enter()
{
	cout << "Lobby Scene" << endl;
	/* Create SceneBounds for Shadow */
	m_SceneBounds.Center = XMFLOAT3(500.11, 0, 500);
	m_SceneBounds.Radius = sqrtf(2000.f * 2000.f + 2000.f * 2000.f);

	/* Light Setting */
	CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL]->Direction = { 0.57735f, -0.57735f, 0.57735f };

	// 카메라 뷰행렬 초기화
	CREVASS::GetApp()->m_Camera->CameraInitialize(SceneType::Lobby);
	Core::g_pFramework->m_pNetwork->Recv();
	Core::m_PlayerID = Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientID;
	return false;
}

void LobbyScene::Exit()
{
	cout << "LobbyScene===========================================" << endl << endl;
}

void LobbyScene::Update(const float& fDeltaTime)
{
	m_SceneController->Update(fDeltaTime);
	//GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);


	int i = MathHelper::Rand(4, Core::mWaves->RowCount() - 5);
	int j = MathHelper::Rand(4, Core::mWaves->ColumnCount() - 5);

	float r = MathHelper::RandF(0.2f, 0.5f);
	Core::mWaves->Disturb(i, j, r);

	Core::mWaves->Update(fDeltaTime);

	for (int i = 0; i < Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i) {
		switch (CREVASS::GetApp()->currchar) {
		case 0:
			AppContext->FindObject<Character>("husky", "husky"+std::to_string(100+i))->SetPosition(200*i, 30, 0);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);
			break;
		case 1:
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);
			break;
		case 2:
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);
			break;
		case 3:
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);
			break;
		case 4:
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
			break;
		}
	}

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_icecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_snow_top"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["lobby_fishrack"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Penguin, MeshReference::GetApp()->m_SkinnedModelInsts["Penguin"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Husky, MeshReference::GetApp()->m_SkinnedModelInsts["husky"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Fox, MeshReference::GetApp()->m_SkinnedModelInsts["ArcticFox"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::PolarBear, MeshReference::GetApp()->m_SkinnedModelInsts["PolarBear"].get());
	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Seal, MeshReference::GetApp()->m_SkinnedModelInsts["Seal"].get());

	AppContext->FindObject<Character>("Penguin", "Penguin0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("husky", "husky0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("ArcticFox", "ArcticFox0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("PolarBear", "PolarBear0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("Seal", "Seal0")->Update(fDeltaTime);

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);
	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);
}

void LobbyScene::Render()
{
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_fishrack"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);

	//sky
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);

	//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
}

void LobbyScene::RenderUI()
{
}
