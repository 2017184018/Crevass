#include "pch.h"
#include "Particle.h"
#include "GeometryMesh.h"

Particle::Particle(std::string type, std::string id) :
	GameObject(type, id),
	m_ParticleName(""),
	m_ParticleCount(0),
	m_ParticleTotalTime(0)
{
}


Particle::~Particle()
{
}

void Particle::Update(const float deltaT)
{
	if (isPlaying)
	{
		cout << m_ParticleTotalTime << endl;
		m_ParticleTotalTime += deltaT;
	}
}

bool Particle::SetParticleNameCount(std::string meshName)
{
		m_ParticleName = meshName;
		m_ParticleCount = Geo->IndexBufferByteSize / sizeof(std::uint16_t);
	return false;
}


void Particle::PlayParticle()
{
	isPlaying = true;
	m_ParticleTotalTime = 0;
}

void Particle::StopParticle()
{
	isPlaying = false;
	m_ParticleTotalTime = 0;
}

float Particle::GetParticleTotalTime() const
{
	return m_ParticleTotalTime;
}

