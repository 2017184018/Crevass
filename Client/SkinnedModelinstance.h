#pragma once
#include "SkinnedData.h"

class SkinnedModelInstance
{
public:
	// �� ������ �� �޼��带 ȣ���ϸ� �ִϸ��̼��� ����ȴ�.
	void UpdateSkinnedAnimation(float dt)
	{
		//TimePos += dt;

		//// �ִϸ��̼� ����
		//if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
		//	TimePos = 0.0f;
		//// �ð� ��ġ�� �����ϰ�, ���� �ִϸ��̼� Ŭ����
		//// �����ؼ� �� ������ �ִϸ��̼��� �����ϰ�, ������ȯ��
		//// ����Ѵ�. ���� ��ȯ�� ���� ���� ���̴��� ���޵ȴ�.
		//// �̷��� ������ �� ������ �ݺ��ϸ� �ִϸ��̼��� ����ȴ�.
		//SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);

	}

	void ChangeSkinnedAnimation(std::string clipName, bool switchingAnim = true, float blendingTime = 0.4f, float blendingAnimSpeed = 2.5f);
	void ChangeSkinnedAnimation(std::string clipName, float timePos);
	void ChangeSkinnedAnimation(std::string prevClipName, float prevTimePos, std::string currClipName, float currTimePos, float factor);

public:

	std::unique_ptr<SkinnedData> SkinnedInfo = nullptr;

	// �־��� �ð������� ���� ��ȯ���� ��´�.
	std::vector<DirectX::XMFLOAT4X4> FinalTransforms;

	// ���� �ִϸ��̼� Ŭ��
	std::string PrevClipName = "Idle";
	std::string ClipName;


	bool SwitchingAnim = false;

	float TimePos = 0.0f;
	float BlendTimePos = 0.f;
	float BlendingTime = 0.4f;
	float BlendingAnimSpeed = 2.5f;
};