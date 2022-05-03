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
	if (!mAnimations.count(clipName)) return;


	UINT numBones = mBoneOffsets.size();
	finalTransforms.resize(numBones);

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	if (clipName == "Attack")
		timePos *= 2.0f;

	if(clipName == "Jump")
		timePos *= 1.5f;

	if (clipName == "Fall")
		timePos *= 1.5f;

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

void SkinnedData::GetBlendedAnimationData(const std::string& clipName1, float timePos1, const std::string& clipName2, float timePos2, float factor, std::vector<DirectX::XMFLOAT4X4>& finalTransform)
{
	if ((clipName1 == "") || (clipName2 == "")) return;

	std::vector< DirectX::XMFLOAT4X4> bones1;
	std::vector< DirectX::XMFLOAT4X4> bones2;

	// 현재시간 / timepos2 = 0.0f
	GetFinalTransforms(clipName1, timePos1, bones1);
	GetFinalTransforms(clipName2, timePos2, bones2);

	bones1.resize(finalTransform.size());
	bones2.resize(finalTransform.size());

	if (factor < 0.0f) factor = 0.0f;
	if (factor > 1.0f) factor = 1.0f;

	XMFLOAT4X4 temp;

	for (unsigned int boneIndex = 0; boneIndex < finalTransform.size(); ++boneIndex)
	{
		temp._11 = (bones1[boneIndex]._11 * (1.0f - factor)) + (bones2[boneIndex]._11 * factor);
		temp._12 = (bones1[boneIndex]._12 * (1.0f - factor)) + (bones2[boneIndex]._12 * factor);
		temp._13 = (bones1[boneIndex]._13 * (1.0f - factor)) + (bones2[boneIndex]._13 * factor);
		temp._14 = (bones1[boneIndex]._14 * (1.0f - factor)) + (bones2[boneIndex]._14 * factor);

		temp._21 = (bones1[boneIndex]._21 * (1.0f - factor)) + (bones2[boneIndex]._21 * factor);
		temp._22 = (bones1[boneIndex]._22 * (1.0f - factor)) + (bones2[boneIndex]._22 * factor);
		temp._23 = (bones1[boneIndex]._23 * (1.0f - factor)) + (bones2[boneIndex]._23 * factor);
		temp._24 = (bones1[boneIndex]._24 * (1.0f - factor)) + (bones2[boneIndex]._24 * factor);

		temp._31 = (bones1[boneIndex]._31 * (1.0f - factor)) + (bones2[boneIndex]._31 * factor);
		temp._32 = (bones1[boneIndex]._32 * (1.0f - factor)) + (bones2[boneIndex]._32 * factor);
		temp._33 = (bones1[boneIndex]._33 * (1.0f - factor)) + (bones2[boneIndex]._33 * factor);
		temp._34 = (bones1[boneIndex]._34 * (1.0f - factor)) + (bones2[boneIndex]._34 * factor);

		temp._41 = (bones1[boneIndex]._41 * (1.0f - factor)) + (bones2[boneIndex]._41 * factor);
		temp._42 = (bones1[boneIndex]._42 * (1.0f - factor)) + (bones2[boneIndex]._42 * factor);
		temp._43 = (bones1[boneIndex]._43 * (1.0f - factor)) + (bones2[boneIndex]._43 * factor);
		temp._44 = (bones1[boneIndex]._44 * (1.0f - factor)) + (bones2[boneIndex]._44 * factor);

		finalTransform[boneIndex] = temp;
	}
}
