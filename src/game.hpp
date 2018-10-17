#ifndef GAME_HPP
#define GAME_HPP

#include <string.h>
#include <string>
#include <fstream>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <psp2/io/stat.h> 
#include <vita2d.h>

#include "menu.hpp"
#include "player.hpp"
#include "collectable.hpp"
#include "buttons.hpp"
#include "texture.hpp"
#include "particle.hpp"
#include "options.hpp"
#include "global.hpp"

class Game
{
	public:
		Game();

		void gameStart();

	private:
		void gameMenu();			// Go to main menu
		void gameDifficulty();		// Menu for choosing game difficulty
		void gameLoop();			// Main game loop
		void gamePaused();			// Pause screen
		void gameEnd();				// Game over screen
		void gameQuit();			// Exit game and free resources
		void gameReinitialize();	// Reinitialize game variables and go to menu
		void gamePlayAgain();		// Reinitialize game variables and go to the game 
		void gameHTP();				// Instructions on how to play the game
		void gameOptions();
		
		void gameDraw();			// Draw all gameplay elements

		static const int START_SNAKE_LENGTH = 3;

		bool score_read = false;

		enum gameState
		{
			initialized = 0,
			showingMenu = 1,
			choosingDifficulty = 2,
			playing = 3,
			paused = 4,
			gameOver = 5,
			exiting = 6,
			needReinitialize = 7,
			playAgain = 8,
			showingHTP = 9,
			options = 10
		};
		gameState _gameState;

		MainMenu mainMenu;
		DifficultyMenu difficultyMenu;
		PauseMenu pauseMenu;
		GameOverMenu gameOverMenu;
		OptionsMenu optionsMenu;

		int SNAKE_LENGTH = START_SNAKE_LENGTH;
		Player snakePart[255];

		Collectable collectable;

		SceCtrlData pad;

		SceTouchData touch_old[SCE_TOUCH_PORT_MAX_NUM];
		SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];
};


#endif // GAME_HPP