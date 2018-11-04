#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <cstdlib>

#include <vita2d.h>

#include "global.hpp"


class Particle
{
	public:
		Particle( float x, float y );

		void render();

		bool isDead();

	private:
		static const int MAX_LIFE = 40;		// The particle max nr of frames lifespan
		static const int RAND_LIFE = 20;	// Random nr of frames to substract from max life (from 0 to RAND_LIFE)
		float ALPHA_MULT = 255.0f / MAX_LIFE;

		float xPos, yPos;

		float lifespan;
};


#endif // PARTICLE_HPP
