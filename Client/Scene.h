#pragma once
#include "SceneController.h"

namespace Core
{
	extern Microsoft::WRL::ComPtr<ID3D12Device> g_Device;
	extern Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_CommandList;

	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
}

/* Scenes
@ 그릴 오브젝트들이 무엇인지를 설계한다.
@ 오브젝트 생성 및 배치
*/
class Scene abstract
{
protected:
	friend class SceneManager;
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize() = 0;
	virtual void OnResize() = 0;
public:
	virtual bool	Enter() = 0;
	virtual void	Exit() = 0;

	virtual void	Update(const float& fDeltaTime) = 0;
	virtual void	Render() = 0;
	virtual void	RenderUI() = 0;
	virtual void WriteShadow() {};
protected:
	std::string m_SceneName;
	Controller* m_SceneController;
	DirectX::BoundingSphere m_SceneBounds;
};

