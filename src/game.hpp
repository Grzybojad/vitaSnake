#ifndef GAME_HPP
#define GAME_HPP

#include <string.h>
#include <string>
#include <fstream>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h> 
#include <vita2d.h>

#include "player.hpp"
#include "collectable.hpp"
#include "global.hpp"

class Game
{
	public:
		Game();

		void gameStart();
		void gameQuit();

	private:
		void gameMenu();
		void gameLoop();
		void gamePaused();
		void gameEnd();

		enum gameState
		{
			uninitialized = 0,
			showingMenu = 1,
			playing = 2,
			paused = 3,
			gameOver = 4,
			exiting = 5
		};

		gameState _gameState;

		int SNAKE_LENGTH;
		Player snakePart[ 255 ];
		Collectable collectable;

		SceCtrlData pad;

		vita2d_pvf *pvf;

		bool startPressed;

		// Screen textures
		vita2d_texture *pauseTexture;
		vita2d_texture *overTexture;
};

#endif // GAME_HPP
