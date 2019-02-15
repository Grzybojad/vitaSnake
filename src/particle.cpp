#include "particle.hpp"

Particle::Particle( vec3 pos )
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.r = pos.r;

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

void Particle::renderTexturePart( const vita2d_texture *texture, int partX, int partY )
{
	// Only lower alpha near the end of the particle's lifetime
	int alpha = 255;;
	if( lifespan > ( 0.8 * MAX_LIFE ) )
		alpha = ( -lifespan + MAX_LIFE ) * ALPHA_MULT;
	
	vita2d_draw_texture_part_tint_scale_rotate( texture, pos.x, pos.y, partX, partY, 6, 6, 1, 1, 0, RGBA8( 255, 255, 255, alpha ) );

	lifespan += timestep;
}

void Particle::move()
{
	pos.x -= sin( pos.r ) * PARTICLE_SPEED * timestep;
	pos.y += cos( pos.r ) * PARTICLE_SPEED * timestep;
}

bool Particle::isDead()
{
	return lifespan > MAX_LIFE;
}