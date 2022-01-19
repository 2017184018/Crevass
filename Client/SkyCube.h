#pragma once
#include "GameObject.h"

class SkyCube : public GameObject
{
public:
	explicit SkyCube();
	virtual ~SkyCube();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};

