#pragma once
#include <DirectXMath.h>
class Interface
{
public:
	void SetPos(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetPos();

public:
	DirectX::XMFLOAT3 m_pos;
};