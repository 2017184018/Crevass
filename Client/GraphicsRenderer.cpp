#include "pch.h"
#include "GraphicsRenderer.h"
#include "GameTimer.h"

#include "CommandContext.h"

#include "GameObject.h"
#include "GeometryMesh.h"
#include "DDSTextureLoader.h"

#include "MaterialReference.h"

namespace Graphics
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> g_OpaquePSO;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkinnedPSO;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> g_SkyPSO;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> g_BB;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> HorBlur;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> VerBlur;
}

using namespace Core;
using namespace Graphics;

void GraphicsRenderer::Initialize()
{
	m_CbvSrvDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	Core::mBlurFilter = std::make_unique<BlurFilter>(g_Device.Get(),
		g_DisplayWidth, g_DisplayHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

	LoadTextures();
	BuildRootSignatures();
	BuildPostProcessRootSignature();
	BuildDescriptorHeaps();
	BuildShaderAndInputLayout();
	BuildPipelineStateObjects();
}

void GraphicsRenderer::Shutdown()
{
}

void GraphicsRenderer::RenderGraphics()
{
	/****************************** Render **********************************/
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvDescriptorHeap.Get() };
	g_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	g_CommandList->SetGraphicsRootSignature(m_RenderRS.Get());

	auto matBuffer = GraphicsContext::GetApp()->MaterialBuffer->Resource();
	g_CommandList->SetGraphicsRootShaderResourceView(1, matBuffer->GetGPUVirtualAddress());
	auto passCB = GraphicsContext::GetApp()->PassCB->Resource();
	g_CommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(m_SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(mSkyTexHeapIndex, m_CbvSrvDescriptorSize);
	g_CommandList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

	g_CommandList->SetGraphicsRootDescriptorTable(4, m_SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}

void GraphicsRenderer::LoadTextures()
{
	std::vector<std::string> texNames =
	{
		"snowcube1024",
		"ice",
		"Penguin",
		"water",
		"husky",
		"heart",
		"heartline",
		"rope",
		"lobby",
		"arctic",
		"ArcticFox",
		"PolarBear",
		"Seal",
		"lobby1",
		"lobby2",
		"lobby3",
		"lobby4",
		"lobby5",
		"waterdrop",
		"snowmanicon",
	};

	std::vector<std::wstring> texFilenames =
	{
			L"./Textures/snowcube1024.dds",
		L"./Textures/cubeworld_tex.dds",
		L"./Textures/Penguin.dds",
		L"./Textures/water1.dds",
		L"./Textures/Tex_Husky.dds",
		L"./Textures/Heart.dds",
		L"./Textures/HeartLine_white.dds",
		L"./Textures/rope.dds",
		L"./Textures/lobby0.dds",
		L"./Textures/arctic_atlas01.dds",
		L"./Textures/Tex_ArcticFox.dds",
		L"./Textures/Tex_PolarBear.dds",
		L"./Textures/Tex_Seal.dds",
		L"./Textures/lobby1.dds",
		L"./Textures/lobby2.dds",
		L"./Textures/lobby3.dds",
		L"./Textures/lobby4.dds",
		L"./Textures/lobby5.dds",
		L"./Textures/waterdrop.dds",
		L"./Textures/snowmanicon.dds",
	};

	for (int i = 0; i < (int)texNames.size(); ++i)
	{
		auto texMap = std::make_unique<Texture>();
		texMap->Name = texNames[i];
		texMap->Filename = texFilenames[i];
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(g_Device.Get(),
			g_CommandList.Get(), texMap->Filename.c_str(),
			texMap->Resource, texMap->UploadHeap));

		m_Textures[texMap->Name] = std::move(texMap);
	}
}

void GraphicsRenderer::BuildDescriptorHeaps()
{
	const int blurDescriptorCount = 4;
	//
		// Create the SRV heap.
		//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = m_Textures.size() + blurDescriptorCount;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(g_Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvDescriptorHeap)));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto snowcube1024 = m_Textures["snowcube1024"]->Resource;
	auto ice = m_Textures["ice"]->Resource;
	auto Penguin = m_Textures["Penguin"]->Resource;
	auto water = m_Textures["water"]->Resource;
	auto husky = m_Textures["husky"]->Resource;
	auto heart = m_Textures["heart"]->Resource;
	auto heartline = m_Textures["heartline"]->Resource;
	auto rope = m_Textures["rope"]->Resource;
	auto lobby = m_Textures["lobby"]->Resource;
	auto arctic = m_Textures["arctic"]->Resource;
	auto ArcticFox = m_Textures["ArcticFox"]->Resource;
	auto PolarBear = m_Textures["PolarBear"]->Resource;
	auto Seal = m_Textures["Seal"]->Resource;
	auto lobby1 = m_Textures["lobby1"]->Resource;
	auto lobby2 = m_Textures["lobby2"]->Resource;
	auto lobby3 = m_Textures["lobby3"]->Resource;
	auto lobby4 = m_Textures["lobby4"]->Resource;
	auto lobby5 = m_Textures["lobby5"]->Resource;
	auto waterdrop = m_Textures["waterdrop"]->Resource;
	auto snowmanicon = m_Textures["snowmanicon"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// �Թ�ü�� TECTURECUBE
	srvDesc.Format = snowcube1024->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = snowcube1024->GetDesc().MipLevels;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(snowcube1024.Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = ice->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = ice->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(ice.Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = Penguin->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = Penguin->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(Penguin.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = water->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = water->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(water.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = husky->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = husky->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(husky.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = heart->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = heart->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(heart.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = heartline->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = heartline->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(heartline.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = rope->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = rope->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(rope.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = arctic->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = arctic->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(arctic.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = ArcticFox->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = ArcticFox->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(ArcticFox.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = PolarBear->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = PolarBear->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(PolarBear.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = Seal->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = Seal->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(Seal.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby1->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby1->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby1.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby2->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby2->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby2.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby3->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby3->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby3.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby4->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby4->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby4.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = lobby5->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = lobby5->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(lobby5.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = waterdrop->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = waterdrop->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(waterdrop.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, m_CbvSrvDescriptorSize);

	srvDesc.Format = snowmanicon->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = snowmanicon->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	g_Device->CreateShaderResourceView(snowmanicon.Get(), &srvDesc, hDescriptor);

	mSkyTexHeapIndex = 0;

	Core::mBlurFilter->BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_SrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_Textures.size(), m_CbvSrvDescriptorSize),
		CD3DX12_GPU_DESCRIPTOR_HANDLE(m_SrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), m_Textures.size(), m_CbvSrvDescriptorSize),
		m_CbvSrvDescriptorSize);
}

void GraphicsRenderer::BuildShaderAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};
	const D3D_SHADER_MACRO skinnedDefines[] =
	{
		"SKINNED", "1",
		NULL, NULL
	};

	m_Shaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	m_Shaders["skinnedVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", skinnedDefines, "VS", "vs_5_1");
	m_Shaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	m_Shaders["skyVS"] = d3dUtil::CompileShader(L"Shaders\\Sky.hlsl", nullptr, "VS", "vs_5_1");
	m_Shaders["skyPS"] = d3dUtil::CompileShader(L"Shaders\\Sky.hlsl", nullptr, "PS", "ps_5_1");

	m_Instancing_InputLayout =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_Skinned_InputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 68, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_Shaders["horzBlurCS"] = d3dUtil::CompileShader(L"Shaders\\Blur.hlsl", nullptr, "HorzBlurCS", "cs_5_1");
	m_Shaders["vertBlurCS"] = d3dUtil::CompileShader(L"Shaders\\Blur.hlsl", nullptr, "VertBlurCS", "cs_5_1");
}

void GraphicsRenderer::BuildRootSignatures()
{
	CD3DX12_DESCRIPTOR_RANGE skyboxTable;
	skyboxTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);

	CD3DX12_DESCRIPTOR_RANGE textureTable;
	textureTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, m_Textures.size(), 1, 0);		//�ؽ��� ��

	CD3DX12_ROOT_PARAMETER slotRootParameter[6];

	// ���� �� : ���󵵰� �����Ϳ��� ���� ���� ������ �����Ѵ�.
	/* Shader Register*/
	// Space 0
	// Texture(t1~8), passCB(b0), passCB(b1)
	// Space 1(����ü)
	// Instancing(t0), Material(t1)

	/* RootParameter slot*/
	// 0: Instancing / 1: Material / 2: passCB / 3: Texture / 4: Skybox
	slotRootParameter[0].InitAsShaderResourceView(0, 1);
	slotRootParameter[1].InitAsShaderResourceView(1, 1);
	slotRootParameter[2].InitAsConstantBufferView(0);
	slotRootParameter[3].InitAsDescriptorTable(1, &skyboxTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[4].InitAsDescriptorTable(1, &textureTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[5].InitAsConstantBufferView(1);

	auto staticSamplers = GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(6, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(g_Device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&m_RenderRS)));
}

void GraphicsRenderer::BuildPipelineStateObjects()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { m_Instancing_InputLayout.data(), (UINT)m_Instancing_InputLayout.size() };
	opaquePsoDesc.pRootSignature = m_RenderRS.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["standardVS"]->GetBufferPointer()),
		m_Shaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["opaquePS"]->GetBufferPointer()),
		m_Shaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = g_BackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = g_4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = g_4xMsaaState ? (g_4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = g_DepthStencilFormat;
	ThrowIfFailed(g_Device->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&g_OpaquePSO)));
	//
	// PSO for BB pass.
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC OpaquePsoBBDesc = opaquePsoDesc;
	OpaquePsoBBDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["standardVS"]->GetBufferPointer()),
		m_Shaders["standardVS"]->GetBufferSize()
	};
	OpaquePsoBBDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["opaquePS"]->GetBufferPointer()),
		m_Shaders["opaquePS"]->GetBufferSize()
	};
	OpaquePsoBBDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	ThrowIfFailed(g_Device->CreateGraphicsPipelineState(&OpaquePsoBBDesc, IID_PPV_ARGS(&g_BB)));


	//
	// PSO for skinned pass.
	//
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skinnedOpaquePsoDesc = opaquePsoDesc;
	skinnedOpaquePsoDesc.InputLayout = { m_Skinned_InputLayout.data(), (UINT)m_Skinned_InputLayout.size() };
	skinnedOpaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["skinnedVS"]->GetBufferPointer()),
		m_Shaders["skinnedVS"]->GetBufferSize()
	};
	skinnedOpaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["opaquePS"]->GetBufferPointer()),
		m_Shaders["opaquePS"]->GetBufferSize()
	};

	D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.LogicOpEnable = false;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	skinnedOpaquePsoDesc.BlendState.RenderTarget[0] = transparencyBlendDesc;

	ThrowIfFailed(g_Device->CreateGraphicsPipelineState(&skinnedOpaquePsoDesc, IID_PPV_ARGS(&g_SkinnedPSO)));

	////
	//// PSO for sky.
	////
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = opaquePsoDesc;
	skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	skyPsoDesc.pRootSignature = m_RenderRS.Get();
	skyPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["skyVS"]->GetBufferPointer()),
		m_Shaders["skyVS"]->GetBufferSize()
	};
	skyPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["skyPS"]->GetBufferPointer()),
		m_Shaders["skyPS"]->GetBufferSize()
	};
	ThrowIfFailed(g_Device->CreateGraphicsPipelineState(&skyPsoDesc, IID_PPV_ARGS(&g_SkyPSO)));

	//
	// PSO for horizontal blur
	//
	D3D12_COMPUTE_PIPELINE_STATE_DESC horzBlurPSO = {};
	horzBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
	horzBlurPSO.CS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["horzBlurCS"]->GetBufferPointer()),
		m_Shaders["horzBlurCS"]->GetBufferSize()
	};
	horzBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(g_Device->CreateComputePipelineState(&horzBlurPSO, IID_PPV_ARGS(&HorBlur)));

	//
	// PSO for vertical blur
	//
	D3D12_COMPUTE_PIPELINE_STATE_DESC vertBlurPSO = {};
	vertBlurPSO.pRootSignature = mPostProcessRootSignature.Get();
	vertBlurPSO.CS =
	{
		reinterpret_cast<BYTE*>(m_Shaders["vertBlurCS"]->GetBufferPointer()),
		m_Shaders["vertBlurCS"]->GetBufferSize()
	};
	vertBlurPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(g_Device->CreateComputePipelineState(&vertBlurPSO, IID_PPV_ARGS(&VerBlur)));
}

void GraphicsRenderer::BuildPostProcessRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE srvTable;
	srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE uavTable;
	uavTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Perfomance TIP: Order from most frequent to least frequent.
	slotRootParameter[0].InitAsConstants(12, 0);
	slotRootParameter[1].InitAsDescriptorTable(1, &srvTable);
	slotRootParameter[2].InitAsDescriptorTable(1, &uavTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter,
		0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(g_Device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mPostProcessRootSignature.GetAddressOf())));
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GraphicsRenderer::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
