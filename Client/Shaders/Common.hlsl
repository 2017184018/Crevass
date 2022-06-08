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

// Texture2DArray�ʹ� �޸� �� �迭���� ũ��� ������
// �ٸ� �ؽ�ó���� ���� �� �ִ�. ���� �� �� �����ϴ�.
Texture2D gDiffuseMap[3] : register(t1);

// ���� �ڷḦ space1�� �����Ѵ�. ���� ���� �ؽ�ó �迭���� ��ġ�� �ʴ´�.
// ���� �ؽ�ó �迭�� space0�� �������� t0, t1,,, t7�� �����Ѵ�.
StructuredBuffer<InstanceData> gInstanceData : register(t0, space1);
StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);

// ������ �н����� �޶����� ��� �ڷ�
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

	//�ִ� MAxLights���� ��ü�� ���� �߿���
	// [0, NUM_DIR_LIGHTS]������ ���ε��� ���Ɽ���̰�
	// [NUM_DIR_LIGHTS, NUM_DIR_LIGHTS + NUM_POINT_L
	Light gLights[MaxLights];
};

cbuffer cbSkinned : register(b1)
{
	// ĳ���ʹ� �ִ� 96���� ���븦 �����Ѵ�.
	float4x4 gBoneTransforms[96];
};