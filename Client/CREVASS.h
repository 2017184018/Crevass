#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"

namespace Graphics
{
	extern Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
}

namespace Core
{
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
}

class MeshReference;
class MaterialReference;
class GameObject;
class CREVASS : public IGameApp, public TemplateSingleton<CREVASS>
{
public:
	enum class RenderLayer : int { ID_OPAQUE = 0, ID_SKYCUBE, ID_COUNT };

public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;

	void OnKeyboardInput(const float deltaT);

private:
	void BuildScene();

private:
	Camera m_Camera;

public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;

	std::vector<GameObject*>	m_AllRItems;
	std::vector<GameObject*>	m_RitemLayer[static_cast<int>(RenderLayer::ID_COUNT)];
	UINT						m_InstanceCount = 0;
};

