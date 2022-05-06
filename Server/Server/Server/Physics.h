#pragma once
#include "Global.h"
class Interface;
class Boundary;

enum ASIX { ASIX_Y, ASIX_X, ASIX_Z, ASIX_NUM };

struct BBPoints {
	DirectX::XMFLOAT3 point[8];
};

struct BBTwoPoint {
	DirectX::XMFLOAT3 point[2];
};

class Physics
{
public:
	bool CheckCollision(const Interface* a, const Interface* b)const;

	bool CheckCollisionOBB(const Interface* a, const Interface* b)const;
	bool CheckCollisionSphere(const DirectX::XMFLOAT3& aPos, const DirectX::XMFLOAT3& bPos, const float aRad, const float bRad) const;


	// N == not fix
	// F == fix
	void ProcCollisionNN(Interface* a, Interface* b);
	void ProcCollisionNF(Interface* a, Interface* b);

	BBTwoPoint GetTwoPoint(BBPoints bbpoints, ASIX asix) const;
	const DirectX::XMFLOAT3 GetPointTo(const DirectX::XMFLOAT3 to, DirectX::XMFLOAT3 pos, float distance)const;
	void GetPoints(const Boundary* boundary, BBPoints* points, int num)const;
	//BBTwoPoint ProjectionPoint2Vec( const BBTwoPoint in, XMFLOAT3 vec )const;
	//BBTwoPoint ProjectionPoint2Vec( const BBPoints in, XMFLOAT3 vec )const;

	BBTwoPoint FindMinMaxPoint(const BBTwoPoint in)const;
	BBTwoPoint FindMinMaxPoint(const BBPoints in)const;
};
