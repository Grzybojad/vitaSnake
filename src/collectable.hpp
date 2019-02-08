#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h> 

#include <vita2d.h>

#include "player.hpp"
#include "menu.hpp"
#include "global.hpp"


class Collectable
{
	public:
		Collectable();			// Initializer
		void setFont();			// Sets the score display font

		bool checkCollision( vec3 playerPos );		// Player-collectable collision
		bool checkOpenDistance( vec3 playerPos );	// Check if the player is close to the collectable

		int collect();			// Pick up the collectable
		void render();			// Render the collectable

		void readHighscore();		// Read highscore from file
		void writeHighscore();		// Write new highscore to file
		int getHighscore();			// Return highscore
		void renderHighscore();		// Render the highscore text
		void renderMenuScores();	// Render menu scores
		void checkAndFixHighscores();

		void renderParticles();

		int getScore();			// Get the score number
		void resetScore();		// Reset the score counter
		void renderScore();		// Render the score text

		void setPos( float x, float y );

	private:
		// Collectable dimensions
		static const int COLLECT_WIDTH = 30;
		static const int COLLECT_HEIGHT = 30;

		// The distance at which a collision occurs
		const float COLLISION_DISTANCE = 30.0f;	

		// The distance at which the snake opens it's mouth
		const float OPEN_DISTANCE = 100.0f;

		// Collectable position
		vec3 pos;

		// Score
		int score;
		int SCORE_ADD;

		const int fibonacci[24] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657 };

		struct Highscore
		{
			int casual;
			int hardcore;
		};
		Highscore highscore[5];

		// Sparkles to make the collectable more visible
		static const int TOTAL_PARTICLES = 3;
		Particle* particles[ TOTAL_PARTICLES ];

		// "Animation"
		const float ANIMATION_SPEED = 0.08f;
		float animation_step = 0.0f;
};


#endif // COLLECTABLE_HPP