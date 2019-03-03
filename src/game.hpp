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
#include "timer.hpp"
#include "stats.hpp"
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
		void gameMode();			// Menu for choosing the game mode
		
		void gameDraw();			// Draw all gameplay elements

		bool score_read = false;

		MainMenu mainMenu;
		DifficultyMenu difficultyMenu;
		PauseMenu pauseMenu;
		GameOverMenu gameOverMenu;
		OptionsMenu optionsMenu;
		ModeMenu modeMenu;

		Player snake;

		Collectable collectable;

		Timer timer;
		Timer gameTime;

		Stats stats;

		SceCtrlData pad;

		SceTouchData touch_old[SCE_TOUCH_PORT_MAX_NUM];
		SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];
};


#endif // GAME_HPP