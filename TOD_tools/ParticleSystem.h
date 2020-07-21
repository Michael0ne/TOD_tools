#pragma once

#include "Node.h"

#define PARTICLE_SYSTEM_CLASS_SIZE 244

class ParticleSystem : public Node
{
protected:
	//	FIXME: to be filled in.

public:
	ParticleSystem();	//	@8EB650
};

static_assert(sizeof(ParticleSystem) == PARTICLE_SYSTEM_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ParticleSystem));