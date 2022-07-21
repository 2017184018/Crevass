#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
	// XMFLOAT4X4 world
	// XMFLOAT2 size
	// particle count
	// 
	bool isPlaying = false;
public:
	explicit Particle(std::string type, std::string id);
	virtual ~Particle();

	virtual void Update(const float deltaT) override;
	bool SetParticleNameCount(std::string meshName);

	void SetIsLoop(bool isLoop);
	//virtual void Update() = 0;
public:
	
	void PlayParticle();
	void StopParticle();

	bool	GetIsLoop() const;
	float GetParticleTotalTime() const;

private:
	std::string m_ParticleName;
	int m_ParticleCount;
	float m_ParticleTotalTime;
	bool m_IsLoop = false;

};

