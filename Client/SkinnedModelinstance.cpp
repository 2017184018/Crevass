#include "pch.h"
#include "SkinnedModelInstance.h"

void SkinnedModelInstance::ChangeSkinnedAnimation(std::string clipName, bool switchingAnim, float blendingTime, float blendingAnimSpeed)
{
	if (!switchingAnim)
		SwitchingAnim = true;

	BlendingTime = blendingTime;
	BlendingAnimSpeed = blendingAnimSpeed;

	PrevClipName = ClipName;
	ClipName = clipName;
}

void SkinnedModelInstance::ChangeSkinnedAnimation(std::string clipName, float timePos)
{
	SkinnedInfo->GetFinalTransforms(clipName, timePos, FinalTransforms);
}

void SkinnedModelInstance::ChangeSkinnedAnimation(std::string prevClipName, float prevTimePos, std::string currClipName, float currTimePos, float factor)
{
	SkinnedInfo->GetBlendedAnimationData(PrevClipName, prevTimePos, currClipName, currTimePos, factor, FinalTransforms);
}
