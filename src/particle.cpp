#include "particle.hpp"

Particle::Particle( float x, float y )
{
	xPos = x + rand() % MAX_LIFE - RAND_LIFE;
	yPos = y + rand() % MAX_LIFE - RAND_LIFE;

	lifespan = rand() % RAND_LIFE;
}

void Particle::render()
{
	// The particle loses it's alpha over time

	// We need to convert the lifespan to a 0-255 alpha value
	int alpha = ( -lifespan + MAX_LIFE ) * ALPHA_MULT;

	vita2d_draw_texture_tint( gSparkleTexture.texture, xPos, yPos, RGBA8( 255, 255, 255, alpha ) );

	lifespan++;
}

bool Particle::isDead()
{
	return lifespan > MAX_LIFE;
}