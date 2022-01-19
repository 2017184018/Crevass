#include "pch.h"
#include "SkyCube.h"

SkyCube::SkyCube()
{
}

SkyCube::~SkyCube()
{
}

void SkyCube::Initialize()
{
	XMStoreFloat4x4(&World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
	TexTransform = MathHelper::Identity4x4();
}

void SkyCube::Update()
{
}

void SkyCube::Release()
{
}
