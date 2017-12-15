#pragma once
#include "ParticleEffect.h"
#include "Particle.h"
namespace BGE
{
	class ButterflyEffect :
		public ParticleEffect
	{
	public:
		ButterflyEffect(bool hasTransform = false);
		~ButterflyEffect(void);

		bool Initialise();
		void InitParticle(Particle & particle);
		void UpdateParticle(Particle & particle);
	};
}

