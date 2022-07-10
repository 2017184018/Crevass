#include "pch.h"
#include "CommandContext.h"
#include "GameObject.h"
#include "ObjectInfo.h"
#include "GameTimer.h"
#include "Camera.h"
#include "GeometryMesh.h"
#include "SkinnedModelInstance.h"
#include "Character.h"
#include "CharacterParts.h"
#include "Particle.h"
#include "CREVASS.h"
#include "GraphicsRenderer.h"
#include "ShadowMap.h"
void GraphicsContext::Initialize()
{
	for (int i = 0; i < skinnedObjectCount; ++i)
	{
		m_SkinnedCBs[i] = std::make_unique<UploadBuffer<ShaderResource::SkinnedConstants>>(Core::g_Device.Get(), 1, true);
	}
	PassCB = std::make_unique<UploadBuffer<ShaderResource::PassConstants>>(Core::g_Device.Get(), passCount, true);
	//InstanceBuffer = std::make_unique<UploadBuffer<ShaderResource::InstanceData>>(Core::g_Device.Get(), InstanceCount, false);
	MaterialBuffer = std::make_unique<UploadBuffer<ShaderResource::MaterialData>>(Core::g_Device.Get(), materialCount, false);
	WavesVB = std::make_unique<UploadBuffer<Vertex>>(Core::g_Device.Get(), VertexCount, false);
}

void GraphicsContext::Release()
{
}

void GraphicsContext::BuildInstanceBuffer(ObjectInfo* objInfo)
{
	m_InstanceBuffers[objInfo->m_Type] = std::make_unique<UploadBuffer<ShaderResource::InstanceData>>(Core::g_Device.Get(), objInfo->m_InstanceCount, false);
}

void GraphicsContext::Update2DPosition(ObjectInfo* objInfo, std::vector<GameObject*>& rItems)
{
    if (!objInfo) return;

	const std::map<std::string, UINT>& info = objInfo->GetinstanceKeymap();
	int visibleInstanceCount = 0;
	for (auto& i : info)
	{
		if (rItems[i.second]->m_IsVisible) {
			XMFLOAT2 ratio = rItems[i.second]->m_positionRatio;
			XMFLOAT2 size = { rItems[i.second]->m_World._11 / 20.f, rItems[i.second]->m_World._22 / 20.f };
			if (Core::g_DisplayWidth * ratio.x != rItems[i.second]->m_World._41 - size.x)
				rItems[i.second]->m_World._41 = Core::g_DisplayWidth * ratio.x + size.x;
			if (Core::g_DisplayHeight * ratio.y != rItems[i.second]->m_World._42 - size.y)
				rItems[i.second]->m_World._42 = Core::g_DisplayHeight * ratio.y + size.y;

			ratio = rItems[i.second]->m_sizeRatio;
			if (Core::g_DisplayWidth * ratio.x != rItems[i.second]->m_World._11)
				rItems[i.second]->m_World._11 = Core::g_DisplayWidth * ratio.x;

			if (rItems[i.second]->m_World._11 * ratio.y != rItems[i.second]->m_World._22)
				rItems[i.second]->m_World._22 = rItems[i.second]->m_World._11 * ratio.y;
		}
	}


}

void GraphicsContext::UpdateInstanceData(ObjectInfo* objInfo, std::vector<GameObject*>& rItems, bool isParticle)
{
	if (!objInfo) return;

	const std::map<std::string, UINT>& info = objInfo->GetinstanceKeymap();

	int visibleInstanceCount = 0;

	for (auto& i : info)
	{
		

			if (rItems[i.second]->m_IsVisible)
			{
				XMMATRIX world = XMLoadFloat4x4(&rItems[i.second]->m_World);
				XMMATRIX TexTransform = XMLoadFloat4x4(&rItems[i.second]->m_TexTransform);
				XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

				ShaderResource::InstanceData data;
				XMStoreFloat4x4(&data.World, XMMatrixTranspose(world));
				XMStoreFloat4x4(&data.TexTransform, XMMatrixTranspose(TexTransform));
				data.MaterialIndex = rItems[i.second]->m_MaterialIndex;
				if (isParticle) {
					data.particleTime = dynamic_cast<Particle*>(rItems[i.second])->GetParticleTotalTime();
					
				}
				else
				{
					data.particleTime = 0.f;
				}

				// Write the instance data to structured buffer for the visible objects.
				m_InstanceBuffers[objInfo->m_Type]->CopyData(visibleInstanceCount++, data);
			}
		
	}

}

void GraphicsContext::UpdateInstanceDatas(std::vector<ObjectInfo*>& objInfos, std::vector<GameObject*>& rItems)
{
	for (auto& objinfo : objInfos) {
		const std::map<std::string, UINT>& info = objinfo->GetinstanceKeymap();

		int visibleInstanceCount = 0;

		for (auto& i : info)
		{
			XMMATRIX world = XMLoadFloat4x4(&rItems[i.second]->m_World);
			XMMATRIX TexTransform = XMLoadFloat4x4(&rItems[i.second]->m_TexTransform);
			XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

			ShaderResource::InstanceData data;
			XMStoreFloat4x4(&data.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&data.TexTransform, XMMatrixTranspose(TexTransform));
			data.MaterialIndex = rItems[i.second]->m_MaterialIndex;

			// Write the instance data to structured buffer for the visible objects.
			m_InstanceBuffers[objinfo->m_Type]->CopyData(visibleInstanceCount++, data);
		}
	}
}

void GraphicsContext::UpdateWave(Waves* wave, GameObject* waveobject) {
	auto currWavesVB = WavesVB.get();
	for (int i = 0; i < wave->VertexCount(); ++i)
	{
		Vertex v;

		v.Pos = wave->Position(i);
		v.Normal = wave->Normal(i);

		// Derive tex-coords from position by 
		// mapping [-w/2,w/2] --> [0,1]
		v.TexC.x = 0.5f + v.Pos.x / wave->Width();
		v.TexC.y = 0.5f - v.Pos.z / wave->Depth();

		currWavesVB->CopyData(i, v);
	}

	// Set the dynamic VB of the wave renderitem to the current frame VB.
	waveobject->Geo->VertexBufferGPU = currWavesVB->Resource();
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

void GraphicsContext::UpdateMainPassCB(Camera& camera, Light* light)
{
	XMMATRIX view = camera.GetView();
	XMMATRIX proj = camera.GetProj();
	XMMATRIX ortho = camera.GetOrtho();

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMMATRIX shadowTransform = XMLoadFloat4x4(&mShadowTransform);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	XMStoreFloat4x4(&mMainPassCB.Ortho, XMMatrixTranspose(ortho));
	XMStoreFloat4x4(&mMainPassCB.ShadowTransform, XMMatrixTranspose(shadowTransform));

	mMainPassCB.EyePosW = camera.GetPosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)Core::g_DisplayWidth, (float)Core::g_DisplayHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / Core::g_DisplayWidth, 1.0f / Core::g_DisplayHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 5000.0f;
	mMainPassCB.TotalTime = Core::g_GameTimer->TotalTime();
	mMainPassCB.DeltaTime = Core::g_GameTimer->DeltaTime();
	mMainPassCB.AmbientLight = { 0.6f, 0.6f, 0.6f, 1.0f };
	// Directional Light
	mMainPassCB.Lights[0].Direction = light->Direction;
	mMainPassCB.Lights[0].Strength = light->Strength;

	//mMainPassCB.Lights[0].Direction = mRotatedLightDirections[0];
	//mMainPassCB.Lights[0].Strength = { 0.9f, 0.8f, 0.7f };
	/*mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };*/
	//mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	//mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	//mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	//mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

	auto currPassCB = PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void GraphicsContext::UpdateSkinnedCBs(UINT skinnedCBIndex, SkinnedModelInstance* skinmodelInstance)
{
	//skinmodelInstance->UpdateSkinnedAnimation(Core::g_GameTimer->DeltaTime());

	ShaderResource::SkinnedConstants skinnedConstants;
	std::copy(
		std::begin(skinmodelInstance->FinalTransforms),
		std::end(skinmodelInstance->FinalTransforms),
		&skinnedConstants.BoneTransforms[0]);

	m_SkinnedCBs[skinnedCBIndex]->CopyData(0, skinnedConstants);
}

void GraphicsContext::UpdateShadowPassCB()
{

	

	XMMATRIX view = XMLoadFloat4x4(&mLightView);
	XMMATRIX proj = XMLoadFloat4x4(&mLightProj);

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	UINT w = GraphicsRenderer::GetApp()->mShadowMap->Width();
	UINT h = GraphicsRenderer::GetApp()->mShadowMap->Height();

	XMStoreFloat4x4(&mShadowPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mShadowPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mShadowPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mShadowPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mShadowPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mShadowPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mShadowPassCB.EyePosW = mLightPosW;
	mShadowPassCB.RenderTargetSize = XMFLOAT2((float)w, (float)h);
	mShadowPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / w, 1.0f / h);
	mShadowPassCB.NearZ = mLightNearZ;
	mShadowPassCB.FarZ = mLightFarZ;

	auto currPassCB = PassCB.get();
	currPassCB->CopyData(1, mShadowPassCB);
}

void GraphicsContext::UpdateShadowTransform(Light* light, DirectX::BoundingSphere sceneBounds)
{
	
	//mLightRotationAngle = 0.2f * Core::g_GameTimer->DeltaTime();

	//XMMATRIX R = DirectX::XMMatrixRotationY(mLightRotationAngle);
	//for (int i = 0; i < 1; i++) {
	//	XMVECTOR lightDir = XMLoadFloat3(&light->Direction);
	//	lightDir = XMVector3TransformNormal(lightDir, R);
	//	XMStoreFloat3(&light->Direction, lightDir);
	//}

	// Only the first "main" light casts a shadow.
	XMVECTOR lightDir = DirectX::XMLoadFloat3(&light->Direction);
	XMVECTOR lightPos = -2.0f * sceneBounds.Radius * lightDir;
	XMVECTOR targetPos = DirectX::XMLoadFloat3(&sceneBounds.Center);
	XMVECTOR lightUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX lightView = DirectX::XMMatrixLookAtLH(lightPos, targetPos, lightUp);

	XMStoreFloat3(&mLightPosW, lightPos);

	// Transform bounding sphere to light space.
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, lightView));

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.x - sceneBounds.Radius;
	float b = sphereCenterLS.y - sceneBounds.Radius;
	float n = sphereCenterLS.z - sceneBounds.Radius;
	float r = sphereCenterLS.x + sceneBounds.Radius;
	float t = sphereCenterLS.y + sceneBounds.Radius;
	float f = sphereCenterLS.z + sceneBounds.Radius;

	mLightNearZ = n;
	mLightFarZ = f;
	XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = lightView * lightProj * T;
	XMStoreFloat4x4(&mLightView, lightView);
	XMStoreFloat4x4(&mLightProj, lightProj);
	XMStoreFloat4x4(&mShadowTransform, S);
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

		if (ri->m_SkinnedModelInst != nullptr)
		{
			D3D12_GPU_VIRTUAL_ADDRESS skinnedCBAddress = m_SkinnedCBs[ri->m_SkinnedCBIndex]->Resource()->GetGPUVirtualAddress();
			Core::g_CommandList->SetGraphicsRootConstantBufferView(5, skinnedCBAddress);
		}
		else
		{
			Core::g_CommandList->SetGraphicsRootConstantBufferView(5, 0);
		}

		Core::g_CommandList->DrawIndexedInstanced(ri->IndexCount, info.size(), ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

void GraphicsContext::SetResourceShadowPassCB()
{
	Core::g_CommandList->RSSetViewports(1, &GraphicsRenderer::GetApp()->mShadowMap->Viewport());
	Core::g_CommandList->RSSetScissorRects(1, &GraphicsRenderer::GetApp()->mShadowMap->ScissorRect());

	// Change to DEPTH_WRITE.
	Core::g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GraphicsRenderer::GetApp()->mShadowMap->Resource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ShaderResource::PassConstants));

	// Clear the back buffer and depth buffer.
	Core::g_CommandList->ClearDepthStencilView(GraphicsRenderer::GetApp()->mShadowMap->Dsv(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	Core::g_CommandList->OMSetRenderTargets(0, nullptr, false, &GraphicsRenderer::GetApp()->mShadowMap->Dsv());

	// Bind the pass constant buffer for the shadow map pass.
	auto passCB = GraphicsContext::GetApp()->PassCB->Resource();
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress() + 1 * passCBByteSize;
	Core::g_CommandList->SetGraphicsRootConstantBufferView(2, passCBAddress);
}

void GraphicsContext::ShadowTransitionResourceBarrier()
{
	// Change back to GENERIC_READ so we can read the texture in a shader.
	Core::g_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GraphicsRenderer::GetApp()->mShadowMap->Resource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ));
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

void GraphicsContext::OnBlurEffect(bool onoff)
{
	GraphicsRenderer::GetApp()->m_SwitchBlur = onoff;
}

void GraphicsContext::OnResizeBlur()
{
	GraphicsRenderer::GetApp()->ExecuteResizeBlur();
}

void GraphicsContext::LoadFont(const wstring fontName, float fontSize)
{
	m_FontName = fontName;
	m_FontSize = fontSize;

	// Create D2D/DWrite objects for rendering text.
	ThrowIfFailed(Core::g_D2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_TextBrush));
	ThrowIfFailed(Core::g_DWriteFactory->CreateTextFormat(
		fontName.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"en-us",
		&m_TextFormat
	));
	ThrowIfFailed(m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ThrowIfFailed(m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

void GraphicsContext::SetTextSize(float fontSize)
{
	if (m_FontName.empty()) return;
	if (fontSize < 1.f) return;
	if (m_FontSize == fontSize) return;

	m_TextFormat.Reset();
	m_FontSize = fontSize;
	// Create D2D/DWrite objects for rendering text.
	ThrowIfFailed(Core::g_D2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_TextBrush));
	ThrowIfFailed(Core::g_DWriteFactory->CreateTextFormat(
		m_FontName.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_FontSize,
		L"en-us",
		&m_TextFormat
	));
	ThrowIfFailed(m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ThrowIfFailed(m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

void GraphicsContext::SetTextSize(float fontSize, DWRITE_TEXT_ALIGNMENT textType, D2D1::ColorF color)
{
	if (m_FontName.empty()) return;
	if (fontSize < 1.f) return;
	if (m_FontSize == fontSize) return;

	m_TextFormat.Reset();
	m_FontSize = fontSize;
	// Create D2D/DWrite objects for rendering text.
	ThrowIfFailed(Core::g_D2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(color), &m_TextBrush));
	ThrowIfFailed(Core::g_DWriteFactory->CreateTextFormat(
		m_FontName.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_FontSize,
		L"en-us",
		&m_TextFormat
	));
	ThrowIfFailed(m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
	ThrowIfFailed(m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

void GraphicsContext::SetTextAlignMent(DWRITE_TEXT_ALIGNMENT textAlignment)
{
	ThrowIfFailed(m_TextFormat->SetTextAlignment(textAlignment));
}

void GraphicsContext::DrawD2DText(const wstring wtext, float posX, float posY, float rectSize)
{
	D2D1_RECT_F textRect;
	textRect.left = posX - rectSize + Core::g_DisplayWidth / 2;
	textRect.top = posY - rectSize + Core::g_DisplayHeight / 2;
	textRect.right = posX + rectSize + Core::g_DisplayWidth / 2;
	textRect.bottom = posY + rectSize + Core::g_DisplayHeight / 2;


	Core::g_D2dDeviceContext->DrawText(
		wtext.c_str(),
		wtext.size(),
		m_TextFormat.Get(),
		&textRect,
		m_TextBrush.Get()
	);
}

void GraphicsContext::DrawD2DText(const wstring wtext, float posX, float posY, float rectSizeX, float rectSizeY, bool aligmentLeading)
{
	if (!aligmentLeading)
		return;

	// ÁÂ»ó´ÜÀÌ (0, 0)
	D2D1_RECT_F textRect;
	textRect.left = posX - rectSizeX + Core::g_DisplayWidth;
	textRect.top = posY - rectSizeY;
	textRect.right = posX + rectSizeX;
	textRect.bottom = posY + rectSizeY;


	Core::g_D2dDeviceContext->DrawText(
		wtext.c_str(),
		wtext.size(),
		m_TextFormat.Get(),
		&textRect,
		m_TextBrush.Get()
	);
}

