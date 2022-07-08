#pragma once
#include "SkinnedData.h"

class SkinnedModelInstance
{
public:
	// 매 프레임 이 메서드를 호출하면 애니메이션이 진행된다.
	void UpdateSkinnedAnimation(float dt)
	{
		//TimePos += dt;

		//// 애니메이션 루프
		//if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
		//	TimePos = 0.0f;
		//// 시간 위치를 증가하고, 현재 애니메이션 클립에
		//// 기초해서 각 뼈대의 애니메이션을 보간하고, 최종변환을
		//// 계산한다. 최종 변환은 이후 정점 쉐이더에 전달된다.
		//// 이러한 과정을 매 프레임 반복하면 애니매이션이 진행된다.
		//SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);

	}

	void ChangeSkinnedAnimation(std::string clipName, bool switchingAnim = true, float blendingTime = 0.4f, float blendingAnimSpeed = 2.5f);
	void ChangeSkinnedAnimation(std::string clipName, float timePos);
	void ChangeSkinnedAnimation(std::string prevClipName, float prevTimePos, std::string currClipName, float currTimePos, float factor);

public:

	std::unique_ptr<SkinnedData> SkinnedInfo = nullptr;

	// 주어진 시간에서의 최종 변환들을 담는다.
	std::vector<DirectX::XMFLOAT4X4> FinalTransforms;

	// 현재 애니메이션 클립
	std::string PrevClipName = "Idle";
	std::string ClipName;


	bool SwitchingAnim = false;

	float TimePos = 0.0f;
	float BlendTimePos = 0.f;
	float BlendingTime = 0.4f;
	float BlendingAnimSpeed = 2.5f;
};