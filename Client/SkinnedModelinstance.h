#pragma once
#include "SkinnedData.h"

class SkinnedModelInstance
{
public:
	// �� ������ �� �޼��带 ȣ���ϸ� �ִϸ��̼��� ����ȴ�.
	void UpdateSkinnedAnimation(float dt)
	{
		TimePos += dt;

		// �ִϸ��̼� ����
		if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
			TimePos = 0.0f;
		// �ð� ��ġ�� �����ϰ�, ���� �ִϸ��̼� Ŭ����
		// �����ؼ� �� ������ �ִϸ��̼��� �����ϰ�, ������ȯ��
		// ����Ѵ�. ���� ��ȯ�� ���� ���� ���̴��� ���޵ȴ�.
		// �̷��� ������ �� ������ �ݺ��ϸ� �ִϸ��̼��� ����ȴ�.
		SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);
		
	}

public:
	std::unique_ptr<SkinnedData> SkinnedInfo = nullptr;


	// �־��� �ð������� ���� ��ȯ���� ��´�.
	std::vector<DirectX::XMFLOAT4X4> FinalTransforms;
	// ���� �ִϸ��̼� Ŭ��
	std::string ClipName;
	// �ִϸ��̼� �ð� ��ġ
	float TimePos = 0.0f;
};