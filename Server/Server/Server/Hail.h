#pragma once
#include <DirectXMath.h>
class Hail
{
private:
	DirectX::XMFLOAT3 Pos = DirectX::XMFLOAT3(-1000, -1000, -1000);
	DirectX::XMFLOAT3 Velo = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 Accel = DirectX::XMFLOAT3(0.0, -400.0, 0);

public:
	void SetPos(DirectX::XMFLOAT3);
	void SetPos(float x, float y, float z);
	void SetVelo(DirectX::XMFLOAT3);
	void SetVelo(float x, float y, float z);
	void SetAccel(DirectX::XMFLOAT3);
	void SetAccel(float x, float y, float z);
	void SetPosCalc(float x, float y, float z,float time);

	DirectX::XMFLOAT3 GetPos() const;
	DirectX::XMFLOAT3 GetVelo() const;
	DirectX::XMFLOAT3 GetAccel() const;
};

