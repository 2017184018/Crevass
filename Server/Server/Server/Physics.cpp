#include "Physics.h"
#include "Interface.h"
//#include "Boundary.h"

bool Physics::CheckCollision(const Interface* a, const Interface* b) const
{
	int aNumBS = a->m_boundaries->GetNumOfBS();
	int bNumBS = b->m_boundaries->GetNumOfBS();

	XMFLOAT3 aPos, bPos;
	float aRad, bRad;

	for (int i = 0; i < aNumBS; ++i)
	{
		aPos = a->m_boundaries->GetBSPos(i);
		aRad = a->m_boundaries->GetBSRad(i);

		for (int j = 0; j < bNumBS; ++j)
		{
			bPos = b->m_boundaries->GetBSPos(i);
			bRad = b->m_boundaries->GetBSRad(i);

			if (true == CheckCollisionSphere(aPos, bPos, aRad, bRad))
				if (true == CheckCollisionOBB(a, b))
					return true;
		}
	}

	return false;
}

bool Physics::CheckCollisionSphere(const XMFLOAT3& aPos, const XMFLOAT3& bPos, const float aRad, const float bRad) const
{
	if (pow(aRad + bRad, 2) > pow(aPos.x - bPos.x, 2) + pow(aPos.y - bPos.y, 2) + pow(aPos.z - bPos.z, 2))
		return true;
	return false;
}

bool Physics::CheckCollisionOBB(const Interface* a, const Interface* b)const
{
	int aNumBB = a->m_boundaries->GetNumOfBB();
	int bNumBB = b->m_boundaries->GetNumOfBB();

	BBPoints* aPoints = (BBPoints*)malloc(sizeof(BBPoints) * aNumBB);
	BBPoints* bPoints = (BBPoints*)malloc(sizeof(BBPoints) * bNumBB);

	// a, b각 꼭짓점 구하기
	GetPoints(a->m_boundaries, aPoints, aNumBB);
	GetPoints(b->m_boundaries, bPoints, bNumBB);


	BBTwoPoint aTwo;
	BBTwoPoint bTwo;

	// y, x, z 순서로 진행
	for (int i = 0; i < aNumBB; ++i)
	{
		for (int j = 0; j < bNumBB; ++j)
		{
			for (int asix = ASIX_Y; asix < ASIX_NUM; ++asix)
			{
				// a 에게 투영
				aTwo = GetTwoPoint(aPoints[i], (ASIX)asix);


				// b 에게 투영


			}
		}
	}

	free(aPoints);
	free(bPoints);
	return true;
}

BBTwoPoint Physics::GetTwoPoint(BBPoints bbpoints, ASIX asix)const
{
	BBTwoPoint p{};
	return p;
}

void Physics::GetPoints(const Boundary* boundary, BBPoints* points, int num)const
{
	XMFLOAT3 center;
	XMFLOAT3 look;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT3 size;

	for (int i = 0; i < num; ++i)
	{
		center = boundary->GetBBPos(i);
		look = boundary->GetBBLook(i);
		up = boundary->GetBBUp(i);
		right = boundary->GetBBRight(i);
		size = boundary->GetBBSize(i);

		points[i].point[0] = GetPointTo(right, GetPointTo(up, center, size.y), size.x);
		// 이런식으로 쭉
	}
}

BBTwoPoint Physics::FindMinMaxPoint(const BBTwoPoint in)const
{
	BBTwoPoint ret;
	float min = NULL;
	float max = NULL;

	for (int i = 0; i < 2; ++i)
	{
		float val = pow(in.point[i].x, 2) + pow(in.point[i].y, 2) + pow(in.point[i].z, 2);
		if (val < min || min == NULL)
		{
			min = val;
			ret.point[0] = in.point[i];
		}
		if (val > max)
		{
			max = val;
			ret.point[1] = in.point[i];
		}
	}

	return ret;
}
BBTwoPoint Physics::FindMinMaxPoint(const BBPoints in)const
{
	BBTwoPoint ret;
	float min = NULL;
	float max = NULL;

	for (int i = 0; i < 8; ++i)
	{
		float val = pow(in.point[i].x, 2) + pow(in.point[i].y, 2) + pow(in.point[i].z, 2);
		if (val < min || min == NULL)
		{
			min = val;
			ret.point[0] = in.point[i];
		}
		if (val > max)
		{
			max = val;
			ret.point[1] = in.point[i];
		}
	}

	return ret;
}

const XMFLOAT3 Physics::GetPointTo(const XMFLOAT3 to, XMFLOAT3 pos, float distance)const
{
	XMFLOAT3 res = Vector3::Add(pos, to, distance);
	return res;
}

void Physics::ProcCollisionNN(Interface* a, Interface* b)
{

}

void Physics::ProcCollisionNF(Interface* n, Interface* f)
{

}

