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

		void readHighscore();					// Read highscore from file
		void writeHighscore();					// Write new highscore to file
		int getHighscore( int difficulty );		// Return highscore
		void renderHighscore();					// Render the highscore text
		void renderMenuScores();				// Render menu scores

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
		int easyHighscore = 0;
		int normalHighscore = 0;
		int hardHighscore = 0;

		// Score counter text
		vita2d_pgf *pgf;
};

#endif // COLLECTABLE_HPP