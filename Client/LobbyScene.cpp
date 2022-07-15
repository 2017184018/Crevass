#include "pch.h"
#include "LobbyScene.h"
#include "ApplicationContext.h"
#include "CommandContext.h"
#include "MaterialReference.h"
#include "MeshReference.h"

#include "Character.h"
#include "CharacterParts.h"

#include "CREVASS.h"

void LobbyScene::Initialize()
{
	m_SceneController = new LobbyController(this);
	//AppContext->CreateLobby();
	AppContext->CreateSkycube("sky", "sky0", "snowcube1024");
	AppContext->CreateBlocks();
	AppContext->CreateWave();
	AppContext->CreateBackground();
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
	return false;
}

void LobbyScene::Exit()
{
	cout << "===========================================" << endl << endl;
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
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["fishrack"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateInstanceData(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->UpdateSkinnedCBs(BoneIndex::Penguin, MeshReference::GetApp()->m_SkinnedModelInsts["Penguin"].get());

	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	//meterial
	GraphicsContext::GetApp()->UpdateMaterialBuffer(MaterialReference::GetApp()->m_Materials);
}

void LobbyScene::Render()
{
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[0]);
	GraphicsContext::GetApp()->UpdateWave(Core::mWaves.get(), Core::wave[1]);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icecube"], AppContext->m_RItemsVec);		//fbx
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snow_top"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["icicle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["snowcube"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["mountain"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["tent"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayak"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["kayakpaddle"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_0"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["rock_1"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fish"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sled"], AppContext->m_RItemsVec);
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["fishrack"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Sea"], AppContext->m_RItemsVec);

	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkinnedPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["Penguin"], AppContext->m_RItemsVec);
	//sky
	GraphicsContext::GetApp()->SetPipelineState(Graphics::g_SkyPSO.Get());
	GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["sky"], AppContext->m_RItemsVec);
	//GraphicsContext::GetApp()->DrawRenderItems(AppContext->m_RItemsMap["lobby"], AppContext->m_RItemsVec);
}

void LobbyScene::RenderUI()
{
}
