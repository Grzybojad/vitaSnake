#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <cstdlib>

#include <vita2d.h>

#include "global.hpp"


class Particle
{
	public:
		Particle( vec3 pos );

		void render();

		// Renders the particle with a 4x4 part of a texture
		void renderTexturePart( const vita2d_texture *texture, int partX, int partY );

		// Moves particle in it's pos.r direction
		void move();

		bool isDead();

	private:
		static const int MAX_LIFE = 40;		// The particle max nr of frames lifespan
		static const int RAND_LIFE = 20;	// Random nr of frames to substract from max life (from 0 to RAND_LIFE)
		const float ALPHA_MULT = 255.0f / MAX_LIFE;
		const float PARTICLE_SPEED = 1.6f;

		vec3 pos;
		float direction;
		float randRotationSpeed;

		float lifespan;
};


#endif // PARTICLE_HPP
