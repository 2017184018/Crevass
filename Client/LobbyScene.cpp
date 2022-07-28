#include "pch.h"
#include "LobbyScene.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "MaterialReference.h"
#include "MeshReference.h"

#include "Particle.h"
#include "Character.h"
#include "CharacterParts.h"
#include "SoundManager.h"

#include "CREVASS.h"
#include "Network.h"

using namespace Core;
void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
	//	AppContext->CreateLobby();
	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreatelobbyBlocks();
	AppContext->CreateWave();
	AppContext->CreatelobbyBackground();

	AppContext->CreateUI2D("UI_SelectCharater", "UI_SelectCharater", 27);

	AppContext->CreateUI2D("UI_Up", "UI_Up", 28);
	AppContext->CreateUI2D("UI_Down", "UI_Down", 29);

	AppContext->CreateParticle("snowParticle", "snowParticle", "Particle_snow", true);
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
	AppContext->DisplaylobbyBackground();
	AppContext->DisplaylobbyBlocks();

	AppContext->DisplayUI("UI_SelectCharater", "UI_SelectCharater",  0.f, 230.f, 500, 100);


	// 카메라 뷰행렬 초기화
	CREVASS::GetApp()->m_Camera->CameraInitialize(SceneType::Lobby);
	Core::g_pFramework->m_pNetwork->Recv();
	Core::m_PlayerID = Core::g_pFramework->m_pNetwork->m_pGameInfo->m_ClientID;

	AppContext->DisplayUI("UI_Up", "UI_Up", 130.f * m_PlayerID - 270, 10.f, 30, 30);
	AppContext->DisplayUI("UI_Down", "UI_Down",  130.f * m_PlayerID - 270, -90.f, 30, 30);

	//for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	//{
	//	AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->m_IsVisible = true;
	//	AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->m_IsVisible = true;
	//	AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->m_IsVisible = true;
	//	AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->m_IsVisible = true;
	//	AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->m_IsVisible = true;

	//	
	//}
	//눈 파티클 시작
	AppContext->DisplayParticle("snowParticle", "snowParticle", XMFLOAT3(500, 500, 800));

	SoundManager::GetApp()->PlayBGM(L"LobbyBGM.mp3", 0.8f);
	return false;
}

void LobbyScene::Exit()
{
	SoundManager::GetApp()->StopAll();
	AppContext->HiddenlobbyBackground();
	AppContext->HiddenlobbyBlocks();

	for (int i = 0; i < g_pFramework->m_pNetwork->m_pGameInfo->m_ClientsNum; ++i)
	{
		AppContext->HiddenCharacter("Penguin", "Penguin" + std::to_string(110 + i));
		AppContext->HiddenCharacter("husky", "husky" + std::to_string(100 + i));
		AppContext->HiddenCharacter("ArcticFox", "ArcticFox" + std::to_string(120 + i));
		AppContext->HiddenCharacter("PolarBear", "PolarBear" + std::to_string(130 + i));
		AppContext->HiddenCharacter("Seal", "Seal" + std::to_string(140 + i));

	}
	AppContext->HiddenParticle("snowParticle", "snowParticle");

	AppContext->HiddenUI("UI_SelectCharater", "UI_SelectCharater");
	AppContext->HiddenUI("UI_Up", "UI_Up");
	AppContext->HiddenUI("UI_Down", "UI_Down");
	if (m_SceneController)
		dynamic_cast<LobbyController*>(m_SceneController)->is_ready = false;
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
		switch (Core::g_pFramework->m_pNetwork->GetChooseCharacter(i)) {

		case 0:
			AppContext->DisplayCharacter(AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i)), XMFLOAT3(200.f * i, 30.f, 0.f), true);
			AppContext->HiddenCharacter("Penguin", "Penguin" + std::to_string(110 + i));
			AppContext->HiddenCharacter("ArcticFox", "ArcticFox" + std::to_string(120 + i));
			AppContext->HiddenCharacter("PolarBear", "PolarBear" + std::to_string(130 + i));
			AppContext->HiddenCharacter("Seal", "Seal" + std::to_string(140 + i));

			/*	AppContext->FindObject<Character>("husky", "husky"+std::to_string(100+i))->SetPosition(200*i, 30, 0);
				AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);*/
			break;
		case 1:
			AppContext->DisplayCharacter(AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i)), XMFLOAT3(200.f * i, 30.f, 0.f), true);
			AppContext->HiddenCharacter("husky", "husky" + std::to_string(100 + i));
			AppContext->HiddenCharacter("ArcticFox", "ArcticFox" + std::to_string(120 + i));
			AppContext->HiddenCharacter("PolarBear", "PolarBear" + std::to_string(130 + i));
			AppContext->HiddenCharacter("Seal", "Seal" + std::to_string(140 + i));

			/*	AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(200 * i, 30, 0);
				AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);*/
			break;
		case 2:
			AppContext->DisplayCharacter(AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i)), XMFLOAT3(200.f * i, 30.f, 0.f), true);
			AppContext->HiddenCharacter("husky", "husky" + std::to_string(100 + i));
			AppContext->HiddenCharacter("Penguin", "Penguin" + std::to_string(110 + i));
			AppContext->HiddenCharacter("PolarBear", "PolarBear" + std::to_string(130 + i));
			AppContext->HiddenCharacter("Seal", "Seal" + std::to_string(140 + i));

			/*AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);*/
			break;
		case 3:
			AppContext->DisplayCharacter(AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i)), XMFLOAT3(200.f * i, 30.f, 0.f), true);
			AppContext->HiddenCharacter("husky", "husky" + std::to_string(100 + i));
			AppContext->HiddenCharacter("Penguin", "Penguin" + std::to_string(110 + i));
			AppContext->HiddenCharacter("ArcticFox", "ArcticFox" + std::to_string(120 + i));
			AppContext->HiddenCharacter("Seal", "Seal" + std::to_string(140 + i));

			/*AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(200 * i, 30, 0);
			AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
			AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(-1000, 30, -1000);*/
			break;
		case 4:
			AppContext->DisplayCharacter(AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i)), XMFLOAT3(200.f * i, 30.f, 0.f), true);
			AppContext->HiddenCharacter("husky", "husky" + std::to_string(100 + i));
			AppContext->HiddenCharacter("Penguin", "Penguin" + std::to_string(110 + i));
			AppContext->HiddenCharacter("ArcticFox", "ArcticFox" + std::to_string(120 + i));
			AppContext->HiddenCharacter("PolarBear", "PolarBear" + std::to_string(130 + i));

			/*	AppContext->FindObject<Character>("Seal", "Seal" + std::to_string(140 + i))->SetPosition(200 * i, 30, 0);
				AppContext->FindObject<Character>("husky", "husky" + std::to_string(100 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("Penguin", "Penguin" + std::to_string(110 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("ArcticFox", "ArcticFox" + std::to_string(120 + i))->SetPosition(-1000, 30, -1000);
				AppContext->FindObject<Character>("PolarBear", "PolarBear" + std::to_string(130 + i))->SetPosition(-1000, 30, -1000);*/
			break;
		}
	}

	AppContext->FindObject<Particle>("snowParticle", "snowParticle")->Update(fDeltaTime);


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

	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["UI_SelectCharater"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["UI_SelectCharater"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["UI_Up"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["UI_Up"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->Update2DPosition(AppContext->m_RItemsMap["UI_Down"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["UI_Down"], AppContext->m_RItemsVec);

	AppContext->FindObject<Character>("Penguin", "Penguin0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("husky", "husky0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("ArcticFox", "ArcticFox0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("PolarBear", "PolarBear0")->Update(fDeltaTime);
	AppContext->FindObject<Character>("Seal", "Seal0")->Update(fDeltaTime);

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowParticle"], AppContext->m_RItemsVec, true);

	/*Shadow*/
	GraphicsContext::GetApp()->UpdateShadowTransform(CREVASS::GetApp()->m_Lights[LIGHT_NAME_DIRECTIONAL].get(), m_SceneBounds);
	GraphicsContext::GetApp()->UpdateShadowPassCB();

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

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);

	//sky
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);

	/*Particle*/
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_ParticlePSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowParticle"], AppContext->m_RItemsVec);

	/* UI */
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_UIPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["UI_SelectCharater"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["UI_Up"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["UI_Down"], AppContext->m_RItemsVec);

	/*Shadow*/
	GraphicsContext::GetApp()->SetResourceShadowPassCB();
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_ShadowOpaquePSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby_icecube"], AppContext->m_RItemsVec);
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

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedShadowOpaquePSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["husky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["ArcticFox"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["PolarBear"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Seal"], AppContext->m_RItemsVec);


	\

		//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
}

void LobbyScene::RenderUI()
{
}
