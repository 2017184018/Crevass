#include "pch.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "ObjectInfo.h"
#include "GameTimer.h"
#include "Camera.h"
#include "GeometryMesh.h"

void GraphicsContext::Initialize()
{
	PassCB = std::make_unique<UploadBuffer<ShaderResource::PassConstants>>(Core::g_Device.Get(), passCount, true);
	//InstanceBuffer = std::make_unique<UploadBuffer<ShaderResource::InstanceData>>(Core::g_Device.Get(), InstanceCount, false);
	MaterialBuffer = std::make_unique<UploadBuffer<ShaderResource::MaterialData>>(Core::g_Device.Get(), materialCount, false);
}

void GraphicsContext::Release()
{
}

void GraphicsContext::BuildInstanceBuffer(ObjectInfo* objInfo)
{
	m_InstanceBuffers[objInfo->m_Type] = std::make_unique<UploadBuffer<ShaderResource::InstanceData>>(Core::g_Device.Get(), objInfo->m_InstanceCount, false);
}

void GraphicsContext::UpdateInstanceData(std::map<std::string, ObjectInfo*>& objInfos, std::vector<GameObject*>& rItems)
{
	for (auto& objinfo : objInfos)
	{
		//const auto& instanceData = e->Instances;
		const std::map<std::string, UINT>& info = objinfo.second->GetinstanceKeymap();

		int visibleInstanceCount = 0;

		for (auto& i : info)
		{
			XMMATRIX world = XMLoadFloat4x4(&rItems[i.second]->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&rItems[i.second]->TexTransform);
			XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

			ShaderResource::InstanceData data;
			XMStoreFloat4x4(&data.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&data.TexTransform, XMMatrixTranspose(texTransform));
			data.MaterialIndex = rItems[i.second]->m_MaterialIndex;

			// Write the instance data to structured buffer for the visible objects.
			m_InstanceBuffers[objinfo.first]->CopyData(visibleInstanceCount++, data);
		}
	}
}


void GraphicsContext::UpdateMaterialBuffer(std::unordered_map<std::string, std::unique_ptr<Material>>& materials)
{
	auto currMaterialBuffer = MaterialBuffer.get();
	for (auto& e : materials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();

		XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

		ShaderResource::MaterialData matData;
		matData.DiffuseAlbedo = mat->DiffuseAlbedo;
		matData.FresnelR0 = mat->FresnelR0;
		matData.Roughness = mat->Roughness;
		XMStoreFloat4x4(&matData.MatTransform, XMMatrixTranspose(matTransform));
		matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;

		currMaterialBuffer->CopyData(mat->MatCBIndex, matData);
	}
}

void GraphicsContext::UpdateMainPassCB(Camera& camera)
{
	XMMATRIX view = camera.GetView();
	XMMATRIX proj = camera.GetProj();

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mMainPassCB.EyePosW = camera.GetPosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)Core::g_DisplayWidth, (float)Core::g_DisplayHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / Core::g_DisplayWidth, 1.0f / Core::g_DisplayHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = Core::g_GameTimer->TotalTime();
	mMainPassCB.DeltaTime = Core::g_GameTimer->DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
	//mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	//mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	//mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	//mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

	auto currPassCB = PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void GraphicsContext::DrawRenderItems(ObjectInfo* objInfo, const std::vector<GameObject*>& rItems)
{
	const std::map<std::string, UINT>& info = objInfo->GetinstanceKeymap();

	for (auto& i : info)
	{
		auto ri = rItems[i.second];

		Core::g_CommandList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		Core::g_CommandList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		Core::g_CommandList->IASetPrimitiveTopology(ri->PrimitiveType);

		auto instanceBuffer = m_InstanceBuffers[ri->GetType()]->Resource();
		Core::g_CommandList->SetGraphicsRootShaderResourceView(0, instanceBuffer->GetGPUVirtualAddress());

		Core::g_CommandList->DrawIndexedInstanced(ri->IndexCount, info.size(), ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

void GraphicsContext::SetPipelineState(ID3D12PipelineState* PSO)
{
	if (m_CurPipelineState == PSO)
		return;
	//
	//m_CurPipelineState = PSO;
	Core::g_CommandList->SetPipelineState(PSO);
	
}

void GraphicsContext::SetGraphicsRootSignature(ID3D12RootSignature* RootSignature)
{
	if (m_CurGraphicsRootSignature == RootSignature)
		Core::g_CommandList->SetGraphicsRootSignature(m_CurGraphicsRootSignature);
	else
	{
		Core::g_CommandList->SetGraphicsRootSignature(RootSignature);
		m_CurGraphicsRootSignature = RootSignature;
	}
}
