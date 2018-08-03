#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP

#include <cstdlib>
#include <ctime>
#include <cmath>

#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#include "global.hpp"
#include "player.hpp"

class Collectable
{
	public:
		// Collectable dimensions
		static const int COLLECT_WIDTH = 30;
		static const int COLLECT_HEIGHT = 30;

		const float COLLISION_DISTANCE = 30.0f;		// The distance at which a collision occurs

		// Score
		int score;

		// Initializer
		Collectable();

		// Assignes a texture to the object
		void setTextureAndFont( const char *filename );

		// Player-collectable collision
		bool checkCollision( Player part );

		// Pick up the collectable
		void collect();

		// Render the collectable
		void render();

		// Get the score number
		int getScore();

		// Render the score text
		void renderScore();

		// Clear textures
		void destroyTextures();

	private:
		// Collectable coordinates
		float xPos, yPos;

		// Collectable texture
		vita2d_texture *texture;

		// Score counter text
		vita2d_pgf *pgf;
};

#endif // COLLECTABLE_HPP