#ifndef COLLECTABLE_HPP
#define COLLECTABLE_HPP

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h> 

#include <vita2d.h>

#include "global.hpp"
#include "player.hpp"

class Collectable
{
	public:
		Collectable();			// Initializer
		void setFont();			// Sets the score display font

		bool checkCollision( Player part );	// Player-collectable collision

		void collect();			// Pick up the collectable
		void render();			// Render the collectable

		int getHighscore();		// Read highscore from file
		void renderHighscore();	// Render the highscore text
		void writeHighscore();	// Write new highscore to file

		int getScore();			// Get the score number
		void resetScore();		// Reset the score counter
		void renderScore();		// Render the score text
		
	private:
		// Collectable dimensions
		static const int COLLECT_WIDTH = 30;
		static const int COLLECT_HEIGHT = 30;

		// The distance at which a collision occurs
		const float COLLISION_DISTANCE = 30.0f;	

		// Collectable coordinates
		float xPos, yPos;

		// Score
		int score;

		// Current highscore
		int highscore;

		// Score counter text
		vita2d_pgf *pgf;
};

#endif // COLLECTABLE_HPP