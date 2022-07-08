#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
	// XMFLOAT4X4 world
	// XMFLOAT2 size
	// particle count
	// 

public:
	explicit Particle(std::string type, std::string id);
	virtual ~Particle();

	bool SetParticleNameCount(std::string meshName);

	//virtual void Update() = 0;
public:
	void ParticleOnOff(bool onoff, XMFLOAT3 pos, XMFLOAT3 offset);

private:
	std::string m_ParticleName;
	int m_ParticleCount;
};

