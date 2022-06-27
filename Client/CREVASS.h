#pragma once
#include "IGameApp.h"
#include "Singleton.h"
#include "Camera.h"
//#include "GameObject.h"
//#include "ObjectInfo.h"
#include "Waves.h"
//#include "BlurFilter.h"
#include "MainFramework.h"



namespace Core
{
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;

	extern std::unique_ptr<Waves> mWaves;
	//extern std::unique_ptr<BlurFilter> mBlurFilter;
	//extern ID3D12Resource* BackBuffer;
	//extern ComPtr<ID3D12RootSignature> mPostProcessRootSignature;

	extern MainFramework* g_pFramework;

	extern bool Inactive;
}

class SceneManager;
class MeshReference;
class MaterialReference;
class Character;
class CommandCenter;
class CREVASS : public IGameApp, public TemplateSingleton<CREVASS>
{

public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual void Update(float deltaT) override;
	virtual void RenderScene(void) override;
	virtual void RenderUI(void) override;

private:
	void BuildCharacters();
	void BuildStream();

public:
	Camera* m_Camera;
	std::map<std::string, std::unique_ptr<Light>> m_Lights;
public:
	MeshReference* m_MeshRef;
	MaterialReference* m_MaterialRef;
	SceneManager* m_SceneManager;
	CommandCenter* m_CommandCenter;
};

