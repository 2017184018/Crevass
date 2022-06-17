#include "Hail.h"

void Hail::SetPos(DirectX::XMFLOAT3 pos)
{
	Pos = pos;
}

void Hail::SetPos(float x, float y, float z)
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
}

DirectX::XMFLOAT3 Hail::GetPos() const
{
	return Pos;
}

DirectX::XMFLOAT3 Hail::GetVelo() const
{
	return Velo;
}

DirectX::XMFLOAT3 Hail::GetAccel() const
{
	return Accel;
}

