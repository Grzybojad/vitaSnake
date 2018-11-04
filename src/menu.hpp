#ifndef MENU_HPP
#define MENU_HPP

#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <vita2d.h>

#include "global.hpp"
#include "buttons.hpp"

class Menu
{
	public:
		static const int BUTTON_WIDTH = 338;
		static const int BUTTON_HEIGHT = 64;

		SceTouchData touch_old[ SCE_TOUCH_PORT_MAX_NUM ];
		SceTouchData touch[ SCE_TOUCH_PORT_MAX_NUM ];

		struct MenuItem
		{
			const char *name;
			float x;
			float y;
		};

		int MENU_ITEMS;
		int cursor;

		// Move cursor up and down
		void selectUp();
		void selectDown();

		void menuNav();
		bool selectItem();

		bool touchSelect( MenuItem item );
		
		void renderCursor( MenuItem item );
		void renderCursor( int x, int y, int w, int h );
		void renderButton( MenuItem item );
};

class MainMenu: public Menu
{
	public:
		MainMenu();

		enum itemName
		{
			startGame = 0, 
			howToPlay = 1, 
			options = 2,
			exitGame = 3
		};

		MenuItem item[ 4 ];

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

class DifficultyMenu: public Menu
{
	public:
		DifficultyMenu();

		enum itemName
		{
			classic = 0, 
			hardcore = 1
		};

		MenuItem item[ 2 ];

		void renderBackground();
		void renderSnake();
		void renderDescription();
};


#endif // MENU_HPP