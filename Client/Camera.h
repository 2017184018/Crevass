#ifndef CAMERA_H
#define CAMERA_H

namespace Core
{
	extern int g_DisplayWidth;
	extern int g_DisplayHeight;
}

class Character;
class Camera
{
private:
	CameraType m_CameraType;

public:
	void SetCamera(CameraType cameraType, Character* owner);
	CameraType GetCameraType();

	virtual void Initialize();
	void Update(const DirectX::XMFLOAT3& lookAt, float deltaT);
	void SetCameraType(CameraType cameratype);
public:
	explicit Camera(CameraType cameraType = CameraType::Free);
	virtual ~Camera();

	void OnResize();

	void CameraInitialize(SceneType sceneType);

	// Get/Set world camera position.
	DirectX::XMVECTOR GetPosition()const;
	DirectX::XMFLOAT3 GetPosition3f()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRight()const;
	DirectX::XMFLOAT3 GetRight3f()const;
	DirectX::XMVECTOR GetUp()const;
	DirectX::XMFLOAT3 GetUp3f()const;
	DirectX::XMVECTOR GetLook()const;
	DirectX::XMFLOAT3 GetLook3f()const;

	void SetOffset(DirectX::XMFLOAT3 offset);
	DirectX::XMFLOAT3& GetOffset();

	void SetTimeLag(float fTimeLag);
	float GetTimeLag() { return(mTimeLag); }

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Set Target
	void SetTarget(const DirectX::XMFLOAT3& lookAt);
	void SetLook(XMFLOAT3 look);
	void SetLook(float x,float y,float z);

	// Define camera space via LookAt parameters.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// Get View/Proj matrices.
	DirectX::XMMATRIX GetView()const;
	DirectX::XMMATRIX GetProj()const;
	DirectX::XMMATRIX GetOrtho()const;

	DirectX::XMFLOAT4X4 GetView4x4f()const;
	DirectX::XMFLOAT4X4 GetProj4x4f()const;
	DirectX::XMFLOAT4X4 GetOrtho4x4f()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);
	void RotateX(float angle);
	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

private:
	Character* m_Owner = nullptr;

	DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mRight = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 mUp = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 mLook = { 0.0f, 0.0f, 1.0f };

	DirectX::XMFLOAT3	mTarget = { 0.f,0.f,0.f };
	DirectX::XMFLOAT3	mOffset = { 0.f,0.f,0.f };
	DirectX::XMFLOAT4	mRotation = { 0.f,0.f,0.f,0.f };
	float				mTimeLag = 0.f;

	// Cache frustum properties.
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 mOrtho = MathHelper::Identity4x4();
};

#endif // CAMERA_H