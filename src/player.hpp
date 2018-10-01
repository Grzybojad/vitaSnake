#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <queue>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#include "global.hpp"
#include "menu.hpp"
#include "particle.hpp"

class Player
{
	private:
		// Player dimensions
		static const int PLAYER_WIDTH = 30;
		static const int PLAYER_HEIGHT = 40;
		const float FOLLOW_DISTANCE = 28.0f;		// Follow distanse
		const float ANALOG_DEADZONE = 0.2f;			// Analog stick deadzone
		const float COLLISION_DISTANCE = 20.0f;		// The distance at which a collision occurs

		float xPos, yPos;					// Player coordinates
		float rotation;						// Player current rotation
		float PLAYER_SET_SPEED;				// The speed set by the difficulty
		float PLAYER_SET_ROTATION_SPEED;	// The rotation set by the difficulty
		float speed;						// Player current speed

		// Controls
		SceCtrlData pad;

	public:
		// Initializes variables
		Player();

		enum part
		{
			head = 0,
			body = 1,
			tail = 2
		};

		// Is snake close to the collectable
		bool isClose;

		// Set difficulty
		void setDifficulty();

		// Handles input
		void handleInput();

		// Follow part
		void follow( Player part );

		// Moves the player character
		void move();

		// Tail collision
		bool checkCollision( Player part );

		// Die from touching a wall on HARD
		bool wallDeath();

		// Renders the player on the screen
		void render( part part );

		// Reset player position
		void resetPos();

		// Get position
		float get_xPos();
		float get_yPos();
};


#endif // PLAYER_HPP