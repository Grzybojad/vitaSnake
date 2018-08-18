#ifndef GAME_HPP
#define GAME_HPP

#include <string.h>
#include <string>
#include <fstream>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h> 
#include <vita2d.h>

#include "menu.hpp"
#include "player.hpp"
#include "collectable.hpp"
#include "buttons.hpp"
#include "global.hpp"

class Game
{
	public:
		Game();

		void gameStart();

	private:
		void gameMenu();			// Go to main menu
		void gameLoop();			// Main game loop
		void gamePaused();			// Pause screen
		void gameEnd();				// Game over screen
		void gameQuit();			// Exit game and free resources
		void gameReinitialize();	// Reinitialize game variables and restart

		void gameHTP();			// Instructions on how to play the game

		// Game state
		enum gameState
		{
			initialized = 0,
			showingMenu = 1,
			playing = 2,
			paused = 3,
			gameOver = 4,
			exiting = 5,
			needReinitialize = 6,
			showingHTP = 7
		};
		gameState _gameState;

		// Main menu
		Menu mainMenu;

		// Player length and buffer
		int SNAKE_LENGTH;
		Player snakePart[ 255 ];

		// Collectable
		Collectable collectable;

		// Inputs
		SceCtrlData pad;
		Input input;

		// PVF text
		vita2d_pvf *pvf;


		// Screen textures
		vita2d_texture *pauseTexture;
		vita2d_texture *overTexture;
		vita2d_texture *infoTexture;
};

#endif // GAME_HPP
