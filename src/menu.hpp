#ifndef MENU_HPP
#define MENU_HPP

#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

#include "global.hpp"

class Menu
{
	public:
		struct MenuItem
		{
			int name;
			float x;
			float y;
		};

		int MENU_ITEMS;
		int cursor;

		void selectUp();
		void selectDown();
		
		void renderCursor( MenuItem item );
};

class MainMenu: public Menu
{
	public:
		MainMenu();

		enum itemName
		{
			startGame = 0, 
			howToPlay = 1, 
			exitGame = 2
		};

		MenuItem item[ 3 ];

		void renderBackground();
};

class PauseMenu: public Menu
{
	public:
		PauseMenu();

		enum itemName
		{
			resumeGame = 0, 
			returnToMenu = 1
		};

		MenuItem item[ 2 ];

		void renderBackground();
};

class GameOverMenu: public Menu
{
	public:
		GameOverMenu();

		enum itemName
		{
			playAgain = 0, 
			returnToMenu = 1
		};

		MenuItem item[ 2 ];

		void renderBackground();
};

#endif // MENU_HPP