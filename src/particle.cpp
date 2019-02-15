#include "particle.hpp"

Particle::Particle( vec3 pos )
{
	this->pos.x = pos.x + rand() % MAX_LIFE - RAND_LIFE;
	this->pos.y = pos.y + rand() % MAX_LIFE - RAND_LIFE;
	this->pos.r = 0;

	lifespan = rand() % RAND_LIFE;
}

void Particle::render()
{
	// The particle loses it's alpha over time

	// We need to convert the lifespan to a 0-255 alpha value
	int alpha = ( -lifespan + MAX_LIFE ) * ALPHA_MULT;

	vita2d_draw_texture_tint( gSparkleTexture.texture, pos.x, pos.y, RGBA8( 255, 255, 255, alpha ) );

	lifespan += timestep;
}

bool Particle::isDead()
{
	return lifespan > MAX_LIFE;
}