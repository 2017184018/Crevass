#include "pch.h"
#include "SkinnedData.h"

Keyframe::Keyframe()
	: TimePos(0.0f),
	Translation(0.0f, 0.0f, 0.0f),
	Scale(1.0f, 1.0f, 1.0f),
	RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Keyframe::~Keyframe()
{
}

float BoneAnimation::GetStartTime() const
{
	//키프레임은 시간별로 정렬되어있음
	//첫번째 키프레임
	return Keyframes.front().TimePos;
}

float BoneAnimation::GetEndTime() const
{
	//마지막 키프레임
	float f = Keyframes.back().TimePos;
	return f;
}

void BoneAnimation::Interpolate(float t, DirectX::XMFLOAT4X4& M) const
{
	if (t <= Keyframes.front().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.front().Scale);
		XMVECTOR R = XMLoadFloat4(&Keyframes.front().RotationQuat);
		XMVECTOR T = XMLoadFloat3(&Keyframes.front().Translation);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, R, T));
	}
	else if (t >= Keyframes.back().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.back().Scale);
		XMVECTOR T = XMLoadFloat3(&Keyframes.back().Translation);
		XMVECTOR R = XMLoadFloat4(&Keyframes.back().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, R, T));
	}
	else
	{
		for (UINT i = 0; i < Keyframes.size() - 1; ++i)
		{
			if (t >= Keyframes[i].TimePos && t <= Keyframes[i + 1].TimePos)
			{
				float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i + 1].TimePos - Keyframes[i].TimePos);

				XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
				XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].Scale);

				XMVECTOR t0 = XMLoadFloat3(&Keyframes[i].Translation);
				XMVECTOR t1 = XMLoadFloat3(&Keyframes[i + 1].Translation);

				XMVECTOR r0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
				XMVECTOR r1 = XMLoadFloat4(&Keyframes[i + 1].RotationQuat);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR T = XMVectorLerp(t0, t1, lerpPercent);
				XMVECTOR R = XMQuaternionSlerp(r0, r1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, R, T));

				break;
			}
		}
	}
}

float AnimationClip::GetClipStartTime() const
{
	// Find smallest start time over all bones in this clip.
	float t = MathHelper::Infinity;
	for (UINT i = 0; i < BoneAnimations.size(); i++) {
		t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime() const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const
{
	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		BoneAnimations[i].Interpolate(t, boneTransforms[i]);
	}
}

UINT SkinnedData::BoneCount() const
{
	return (UINT)mBoneHierarchy.size();
}

float SkinnedData::GetClipStartTime(const std::string& clipName) const
{
	auto clip = mAnimations.find(clipName);

	return clip->second.GetClipStartTime();
}

float SkinnedData::GetClipEndTime(const std::string& clipName) const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipEndTime();

}

std::string SkinnedData::GetAnimationName(int num) const
{
	return mAnimationName.at(num);
}

std::vector<int> SkinnedData::GetBoneHierarchy() const
{
	return mBoneHierarchy;
}

std::vector<DirectX::XMFLOAT4X4> SkinnedData::GetBoneOffsets() const
{
	return mBoneOffsets;
}


AnimationClip SkinnedData::GetAnimation(std::string clipName) const
{
	auto iter = mAnimations.find(clipName);

	if (iter != mAnimations.end())
	{
		return mAnimations.find(clipName)->second;
	}
	else {
		return mAnimations.find(clipName)->second;
	}
}

std::vector<int> SkinnedData::GetSubmeshOffset() const
{
	return mSubmeshOffset;
}

DirectX::XMFLOAT4X4 SkinnedData::getBoneOffset(int num) const
{
	return mBoneOffsets.at(num);
}


std::vector<std::string> SkinnedData::GetBoneName() const
{
	return mBoneName;
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<DirectX::XMFLOAT4X4>& boneOffsets, std::unordered_map<std::string, AnimationClip>* animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneOffsets = boneOffsets;
	if (animations != nullptr)
	{
		mAnimations = (*animations);
	}
}

void SkinnedData::SetAnimation(AnimationClip inAnimation, std::string ClipName)
{
	mAnimations[ClipName] = inAnimation;
}

void SkinnedData::SetAnimationName(const std::string& clipName)
{
	mAnimationName.push_back(clipName);
}

void SkinnedData::SetBoneName(std::string boneName)
{
	mBoneName.push_back(boneName);
}

void SkinnedData::SetSubmeshOffset(int num)
{
	mSubmeshOffset.push_back(num);
}

void SkinnedData::clear()
{
	mBoneName.clear();
	mBoneHierarchy.clear();
	mBoneOffsets.clear();
	mAnimationName.clear();
	mAnimations.clear();
	mSubmeshOffset.clear();
}

void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePos, std::vector<DirectX::XMFLOAT4X4>& finalTransforms) const
{
	UINT numBones = mBoneOffsets.size();
	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	//이 클립의 모든 뼈대를 시간에 맞게 보간함
	auto clip = mAnimations.find(clipName);
	clip->second.Interpolate(timePos, toParentTransforms);

	//골격 계통구조를 흝으면서 모든 뼈대를 뿌리 공간으로 변환한다.
	std::vector<XMFLOAT4X4> toRootTransforms(numBones);
	
	toRootTransforms[0] = toParentTransforms[0];

	//자식 뼈대들의 roottranseform을 구함
	for (UINT i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

		int parentIndex = mBoneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	//offset변환행렬을 곱해서 최종벼환을 구함
	for (UINT i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toParentTransforms[i]);
		XMMATRIX finalTransform = XMMatrixMultiply(offset, toRoot);
		
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixTranspose(finalTransform));
	}
}
