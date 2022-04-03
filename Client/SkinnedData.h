#pragma once

struct Keyframe
{
	Keyframe();
	~Keyframe();

	float TimePos;
	DirectX::XMFLOAT3 Translation;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT4 RotationQuat;

	bool operator == (const Keyframe& key)
	{
		if (Translation.x != key.Translation.x || Translation.y != key.Translation.y || Translation.z != key.Translation.z)
			return false;

		if (Scale.x != key.Scale.x || Scale.y != key.Scale.y || Scale.z != key.Scale.z)
			return false;

		if (RotationQuat.x != key.RotationQuat.x || RotationQuat.y != key.RotationQuat.y || RotationQuat.z != key.RotationQuat.z || RotationQuat.w != key.RotationQuat.w)
			return false;

		return true;
	}
};

struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, DirectX::XMFLOAT4X4& M) const;

	std::vector<Keyframe> Keyframes;
};

struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const;

	std::vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:
	UINT BoneCount() const;

	float GetClipStartTime(const std::string& clipName)const;
	float GetClipEndTime(const std::string& clipName) const;
	std::string GetAnimationName(int num) const;
	std::vector<int> GetBoneHierarchy() const;
	std::vector<DirectX::XMFLOAT4X4> GetBoneOffsets() const;
	AnimationClip GetAnimation(std::string clipName) const;
	std::vector<int> GetSubmeshOffset() const;
	DirectX::XMFLOAT4X4 getBoneOffset(int num) const;
	std::vector<std::string> GetBoneName() const;

	void Set(
		std::vector<int>& boneHierarchy,
		std::vector<DirectX::XMFLOAT4X4>& boneOffsets,
		std::unordered_map<std::string, AnimationClip>* animations = nullptr);

	void SetAnimation(AnimationClip inAnimation, std::string ClipName);
	void SetAnimationName(const std::string& clipName);
	void SetBoneName(std::string boneName);
	void SetSubmeshOffset(int num);

	void clear();
	void GetFinalTransforms(const std::string& clipName, float timePos,
		std::vector<DirectX::XMFLOAT4X4>& finalTransforms)const;

public:
	std::vector<std::string> mBoneName;

	// Gives parentIndex of ith bone.
	std::vector<int> mBoneHierarchy;

	std::vector<DirectX::XMFLOAT4X4> mBoneOffsets;

	std::vector<std::string> mAnimationName;
	std::unordered_map<std::string, AnimationClip> mAnimations;

	std::vector<int> mSubmeshOffset;
};