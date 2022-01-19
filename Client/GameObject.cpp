#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	World = MathHelper::Identity4x4();
	TexTransform = MathHelper::Identity4x4();

	Mat = nullptr;
	Geo = nullptr;

	PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	IndexCount = 0;
	InstanceCount = 0;
	StartIndexLocation = 0;
	BaseVertexLocation = 0;
}

GameObject::~GameObject()
{
}

