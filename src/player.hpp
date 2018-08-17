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

class Player
{
	public:
		// Player dimensions
		static const int PLAYER_WIDTH = 30;
		static const int PLAYER_HEIGHT = 40;

		const float PLAYER_MAX_SPEED = 5.0f;		// Player max speed
		const float PLAYER_ROTATION_SPEED = 0.07f;	// Player rotation speed
		const float FOLLOW_DISTANCE = 35.0f;		// Follow distanse
		const float ANALOG_DEADZONE = 0.2f;			// Analog stick deadzone
		const float COLLISION_DISTANCE = 20.0f;		// The distance at which a collision occurs

		float xPos, yPos;	// Player coordinates
		float rotation;		// Player current rotation

		// Initializes variables
		Player();

		// Assignes a texture to the object
		void setTexture( const char *filename );

		// Handles input
		void handleInput();

		// Follow part
		void follow( Player part );

		// Moves the player character
		void move();

		// Tail collision
		bool checkCollision( Player part );

		// Renders the player on the screen
		void render();

		// Get position
		float get_xPos();
		float get_yPos();

		// Clear textures
		void destroyTextures();

	private:
		// Player texture
		vita2d_texture *texture;

		// Player current speed
		float speed;

		// Controls
		SceCtrlData pad;
};


#endif // PLAYER_HPP