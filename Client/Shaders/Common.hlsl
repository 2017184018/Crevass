//***************************************************************************************
// Common.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
#define NUM_DIR_LIGHTS 3
#endif

#ifndef NUM_POINT_LIGHTS
#define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
#define NUM_SPOT_LIGHTS 0
#endif

// Include structures and functions for lighting.
#include "LightingUtil.hlsl"

struct InstanceData
{
	float4x4 World;
	float4x4 TexTransform;
	uint     MaterialIndex;
	uint     InstPad0;
	uint     InstPad1;
	uint     InstPad2;
};


struct MaterialData
{
	float4   DiffuseAlbedo;
	float3   FresnelR0;
	float    Roughness;
	float4x4 MatTransform;
	uint     DiffuseMapIndex;
	uint     MatPad0;
	uint     MatPad1;
	uint     MatPad2;
};

TextureCube gCubeMap : register(t0);

// Texture2DArray와는 달리 이 배열에는 크기와 형식이
// 다른 텍스처들을 담을 수 있다. 따라서 좀 더 유연하다.
Texture2D gDiffuseMap[3] : register(t1);

// 재질 자료를 space1에 배정한다. 따라서 위의 텍스처 배열과는 겹치지 않는다.
// 위의 텍스처 배열은 space0의 레지스터 t0, t1,,, t7을 차지한다.
StructuredBuffer<InstanceData> gInstanceData : register(t0, space1);
StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

// 랜더링 패스마다 달라지는 상수 자료
cbuffer cbPass : register(b0)
{
	float4x4 gView;
	float4x4 gInvView;
	float4x4 gProj;
	float4x4 gInvProj;
	float4x4 gViewProj;
	float4x4 gInvViewProj;
	float4x4 gShadowTransform;
	float3 gEyePosW;
	float cbPerObjectPad1;
	float2 gRenderTargetSize;
	float2 gInvRenderTargetSize;
	float gNearZ;
	float gFarZ;
	float gTotalTime;
	float gDeltaTime;
	float4 gAmbientLight;

	//최대 MAxLights개의 물체별 광원 중에서
	// [0, NUM_DIR_LIGHTS]구간의 색인들은 지향광들이고
	// [NUM_DIR_LIGHTS, NUM_DIR_LIGHTS + NUM_POINT_L
	Light gLights[MaxLights];
};

cbuffer cbSkinned : register(b1)
{
	// 캐릭터당 최대 96개의 뼈대를 지원한다.
	float4x4 gBoneTransforms[96];
};