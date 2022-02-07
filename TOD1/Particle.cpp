#include "Particle.h"

Particle::Particle()
{
    MESSAGE_CLASS_CREATED(Particle);
}

Particle::~Particle()
{
    MESSAGE_CLASS_DESTROYED(Particle);
}

ParticleA::ParticleA()
{
    MESSAGE_CLASS_CREATED(ParticleA);

    field_4 = 1;
    field_8 = NULL;
    field_C = NULL;
}

ParticleA::~ParticleA()
{
    MESSAGE_CLASS_DESTROYED(ParticleA);
}

void ParticleA::SetParticlesListSize(const unsigned int size)
{
    m_Particles.resize(size);

    field_8 = NULL;
    field_C = NULL;
}

const size_t ParticleA::GetParticlesListSize() const
{
    return m_Particles.size();
}