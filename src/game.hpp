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
		static bool isExiting();
		void gameMenu();
		void gameLoop();
		void gamePaused();
		void gameEnd();

		/* Game states are:
		0 = uninitialized
		1 = showingMenu
		2 = playing
		3 = paused
		4 = gameOver
		5 = exiting*/
		int _gameState;

		int SNAKE_LENGTH;
		Player snakePart[ 255 ];
		Collectable collectable;

		SceCtrlData pad;

		vita2d_pvf *pvf;

		bool startPressed;
};

#endif // GAME_HPP
