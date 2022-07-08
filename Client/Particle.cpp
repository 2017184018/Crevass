#include "pch.h"
#include "Particle.h"
#include "GeometryMesh.h"

Particle::Particle(std::string type, std::string id) :
	GameObject(type, id),
	m_ParticleName(""),
	m_ParticleCount(0)
{
}

Particle::~Particle()
{
}

bool Particle::SetParticleNameCount(std::string meshName)
{
		m_ParticleName = meshName;
		m_ParticleCount = Geo->IndexBufferByteSize / sizeof(std::uint16_t);
	return false;
}

void Particle::ParticleOnOff(bool onoff, XMFLOAT3 pos, XMFLOAT3 offset)
{
	m_IsVisible = onoff;

	if (onoff)
		SetPosition(MathHelper::Add(pos, offset));
}

