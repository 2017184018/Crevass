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

void Hail::SetVelo(DirectX::XMFLOAT3 velo)
{
	Velo = velo;
}

void Hail::SetVelo(float x, float y, float z)
{
	Velo.x = x;
	Velo.y = y;
	Velo.z = z;
}

void Hail::SetAccel(DirectX::XMFLOAT3 accel)
{
	Accel = accel;
}

void Hail::SetAccel(float x, float y, float z)
{
	Accel.x = x;
	Accel.y = y;
	Accel.z = z;
}

void Hail::SetPosCalc(float x, float y, float z,float time)
{
	Pos.x = x + Velo.x * time + 0.5 * Accel.x * time * time;
	Pos.y = y + Velo.y * time + 0.5 * Accel.y * time * time;
	Pos.z = z + Velo.z * time + 0.5 * Accel.z * time * time;
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

