#include "pch.h"
#include "Camera.h"
#include "Character.h"

void Camera::SetCamera(CameraType cameraType, Character* owner)
{
	m_CameraType = cameraType;
	m_Owner = owner;

	switch (m_CameraType)
	{
	case CameraType::First:
		mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		mRight.y = 0.0f;
		mLook.y = 0.0f;
		mRight = MathHelper::Normalize(mRight);
		mLook = MathHelper::Normalize(mLook);
		break;
	case CameraType::Third:
		mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
		mRight.y = 0.0f;
		mLook.y = 0.0f;
		mRight = MathHelper::Normalize(mRight);
		mLook = MathHelper::Normalize(mLook);
		break;
	case CameraType::Free:
		break;
	}

	mViewDirty = true;
}

CameraType Camera::GetCameraType()
{
	return m_CameraType;
}

void Camera::Initialize()
{
	m_Owner = nullptr;

	mPosition = { 0.0f, 0.0f, 0.0f };
	mRight = { 1.0f, 0.0f, 0.0f };
	mUp = { 0.0f, 1.0f, 0.0f };
	mLook = { 0.0f, 0.0f, 1.0f };

	mTarget = { 0.f,0.f,0.f };
	mOffset = { 0.f,0.f,0.f };
	mRotation = { 0.f,0.f,0.f,0.f };
	mTimeLag = 0.f;

	mViewDirty = true;
}

void Camera::Update(const DirectX::XMFLOAT3& lookAt, float deltaT)
{
	switch (m_CameraType)
	{
	case CameraType::First:
	{
		XMVECTOR Offset = XMLoadFloat3(&mOffset);

		//Extract only the Rotation Matrix from the Player
		XMMATRIX RotMat = XMLoadFloat4x4(&m_Owner->m_World);
		RotMat.r[3] = XMVectorZero();
		XMVectorSetW(RotMat.r[3], 1.f);

		Offset = XMVector3Transform(Offset, RotMat);
		XMStoreFloat3(&mPosition, XMVectorAdd(XMLoadFloat3(&lookAt), Offset));

		XMFLOAT4X4 LookAtMat;
		XMStoreFloat4x4(&LookAtMat, DirectX::XMMatrixLookToLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&m_Owner->GetLook()), XMLoadFloat3(&m_Owner->GetUp())));

		mRight = XMFLOAT3(LookAtMat._11, LookAtMat._21, LookAtMat._31);
		mUp = XMFLOAT3(LookAtMat._12, LookAtMat._22, LookAtMat._32);
		mLook = XMFLOAT3(LookAtMat._13, LookAtMat._23, LookAtMat._33);
		break;
	}
	case CameraType::Third:
	{
		if (!m_Owner) return;

		XMFLOAT4X4 xmf4x4Rotate = MathHelper::Identity4x4();
		XMFLOAT3 xmf3Right = { 1,0,0 };
		XMFLOAT3 xmf3Up = { 0,1,0 };
		XMFLOAT3 xmf3Look = { 0,0,1 };
		xmf4x4Rotate._11 = xmf3Right.x; xmf4x4Rotate._21 = xmf3Up.x; xmf4x4Rotate._31 = xmf3Look.x;
		xmf4x4Rotate._12 = xmf3Right.y; xmf4x4Rotate._22 = xmf3Up.y; xmf4x4Rotate._32 = xmf3Look.y;
		xmf4x4Rotate._13 = xmf3Right.z; xmf4x4Rotate._23 = xmf3Up.z; xmf4x4Rotate._33 = xmf3Look.z;
	
		XMFLOAT3 xmf3Offset = MathHelper::TransformCoord(mOffset, xmf4x4Rotate);
		XMFLOAT3 xmf3Position;
		if (m_Owner->is_fall == false) {
		 xmf3Position = MathHelper::Add(m_Owner->GetPosition(), xmf3Offset);
		}
		else {
			XMFLOAT3 pos;
			pos.x = m_Owner->GetPosition().x;
			pos.y = m_Owner->GetPosition().y+300;
			pos.z = m_Owner->GetPosition().z+250;
		 xmf3Position = MathHelper::Add(pos, xmf3Offset);
		}

		XMFLOAT3 xmf3Direction = MathHelper::Subtract(xmf3Position, mPosition);
		float fLength = MathHelper::Length(xmf3Direction);
		xmf3Direction = MathHelper::Normalize(xmf3Direction);
		float fTimeLagScale = (mTimeLag) ? deltaT * (1.0f / mTimeLag) : 1.0f;
		float fDistance = fLength * fTimeLagScale;
		if (fDistance > fLength) fDistance = fLength;
		if (fLength < 0.01f) fDistance = fLength;
		if (fDistance > 0)
		{
			mPosition = MathHelper::Add(mPosition, xmf3Direction, fDistance);
			LookAt(mPosition, lookAt, m_Owner->GetUp());
		}

		break;
	}
	case CameraType::Free:
		break;
	}

	mViewDirty = true;
}

Camera::Camera(CameraType cameraType) :
	m_CameraType(cameraType)
{
	Initialize();
	OnResize();
}

Camera::~Camera()
{
}

void Camera::OnResize()
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 2000.0f);
}

XMVECTOR Camera::GetPosition()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 Camera::GetPosition3f()const
{
	return mPosition;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
	mViewDirty = true;
}

void Camera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
	mViewDirty = true;
}

XMVECTOR Camera::GetRight()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Camera::GetRight3f()const
{
	return mRight;
}

XMVECTOR Camera::GetUp()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Camera::GetUp3f()const
{
	return mUp;
}

XMVECTOR Camera::GetLook()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Camera::GetLook3f()const
{
	return mLook;
}

void Camera::SetOffset(DirectX::XMFLOAT3 offset)
{
	mOffset = offset;
	mPosition.x += offset.x;
	mPosition.y += offset.y;
	mPosition.z += offset.z;

	mViewDirty = true;
}

DirectX::XMFLOAT3& Camera::GetOffset()
{
	return mOffset;
}

void Camera::SetTimeLag(float fTimeLag)
{
	mTimeLag = fTimeLag;
	mViewDirty = true;
}

float Camera::GetNearZ()const
{
	return mNearZ;
}

float Camera::GetFarZ()const
{
	return mFarZ;
}

float Camera::GetAspect()const
{
	return mAspect;
}

float Camera::GetFovY()const
{
	return mFovY;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);

	// Ortho
	XMMATRIX O = DirectX::XMMatrixOrthographicLH(Core::g_DisplayWidth, Core::g_DisplayHeight, 0.f, 1.f);
	XMStoreFloat4x4(&mOrtho, O);

}

void Camera::SetTarget(const DirectX::XMFLOAT3& lookAt)
{
	switch (m_CameraType)
	{
	case CameraType::Third:
	{
		XMFLOAT4X4 LookAtMat;
		XMStoreFloat4x4(&LookAtMat, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&lookAt), XMLoadFloat3(&m_Owner->GetUp())));

		mRight = XMFLOAT3(LookAtMat._11, LookAtMat._21, LookAtMat._31);
		mUp = XMFLOAT3(LookAtMat._12, LookAtMat._22, LookAtMat._32);
		mLook = XMFLOAT3(LookAtMat._13, LookAtMat._23, LookAtMat._33);
		break;
	}
	case CameraType::Free:
	{
		XMVECTOR Up = { 0.f,1.f,0.f };
		
		Up = XMVector3Transform(Up, XMMatrixRotationQuaternion(XMLoadFloat4(&mRotation)));

		XMFLOAT4X4 LookAtMat;
		XMStoreFloat4x4(&LookAtMat, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&lookAt), Up));

		mRight = XMFLOAT3(LookAtMat._11, LookAtMat._21, LookAtMat._31);
		mUp = XMFLOAT3(LookAtMat._12, LookAtMat._22, LookAtMat._32);
		mLook = XMFLOAT3(LookAtMat._13, LookAtMat._23, LookAtMat._33);
		break;
	}
	}

	mViewDirty = true;
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	mViewDirty = true;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	mViewDirty = true;
}

XMMATRIX Camera::GetView()const
{
	assert(!mViewDirty);
	return XMLoadFloat4x4(&mView);
}

XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX Camera::GetOrtho() const
{
	return XMLoadFloat4x4(&mOrtho);
}

XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!mViewDirty);
	return mView;
}

XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return mProj;
}

DirectX::XMFLOAT4X4 Camera::GetOrtho4x4f() const
{
	return mOrtho;
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

	mViewDirty = true;
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	mViewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

void Camera::RotateX(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationX(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mViewDirty = true;
}

void Camera::UpdateViewMatrix()
{
	if (mViewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&mRight);
		XMVECTOR U = XMLoadFloat3(&mUp);
		XMVECTOR L = XMLoadFloat3(&mLook);
		XMVECTOR P = XMLoadFloat3(&mPosition);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&mRight, R);
		XMStoreFloat3(&mUp, U);
		XMStoreFloat3(&mLook, L);

		mView(0, 0) = mRight.x;
		mView(1, 0) = mRight.y;
		mView(2, 0) = mRight.z;
		mView(3, 0) = x;

		mView(0, 1) = mUp.x;
		mView(1, 1) = mUp.y;
		mView(2, 1) = mUp.z;
		mView(3, 1) = y;

		mView(0, 2) = mLook.x;
		mView(1, 2) = mLook.y;
		mView(2, 2) = mLook.z;
		mView(3, 2) = z;

		mView(0, 3) = 0.0f;
		mView(1, 3) = 0.0f;
		mView(2, 3) = 0.0f;
		mView(3, 3) = 1.0f;

		mViewDirty = false;
	}
}