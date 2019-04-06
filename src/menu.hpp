#ifndef MENU_HPP
#define MENU_HPP

#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <vita2d.h>

#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>

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

		virtual void menuNav();
		bool selectItem();

		bool touchSelect( MenuItem item );
		
		virtual void renderCursor( MenuItem item );
		virtual void renderCursor( int x, int y, int w, int h );
		virtual void renderButton( MenuItem item );
	
	protected:
		const int cursorShadow = 100;
};

class MainMenu: public Menu
{
	public:
		MainMenu();

		enum itemName
		{
			startGame = 0, 
			extras = 1, 
			options = 2,
			exitGame = 3
		};
		MenuItem item[ 4 ];

		std::vector <std::string> splash;
		int randSplash;
		void readSplashFile();
		void drawSplashText();
		void randomizeSplash();

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

		void drawMenu();
		void gameOverBanner();
		void drawFinalScore( int score );
		void drawNewHighscore();

		virtual void menuNav() override;
		virtual void renderCursor( MenuItem item ) override;
		virtual void renderButton( MenuItem item ) override;

		void resetFadeIn();

	private:
		const int bannerY = 100;
		const int gameOverTextY = bannerY + 100;
		const int scoreTextY = gameOverTextY + 75;
		const int newHighscoreY = scoreTextY + 30;

		// Values for the fade in animation
		double fadeInValue = 0;
		double fadeInSpeed = 12.0;

		int cursorShadow = 50;
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

		void drawMenu();
		void renderSnake();
		void renderDescription();
};

class ModeMenu: public Menu
{
	private:
		static const int margin_x = 45;
		static const int margin_y = 40;
		static const int padding = 40;

	public:
		ModeMenu();

		enum itemName
		{
			classic = 0, 
			timeTrial = 1,
			hyper = 2,
			fibonacci = 3,
			lazy = 4
		};
		MenuItem item[ 5 ];

		void renderMenu();
		void renderDescription();
};

class ExtrasMenu: public Menu
{
	private:
		 int margin_x = ( SCREEN_WIDTH - BUTTON_WIDTH ) / 2;
		static const int margin_y = 280;
		static const int padding = 30;

	public:
		ExtrasMenu();

		enum itemName
		{
			howToPlay = 0,
			credits = 1
		};
		MenuItem item[ 2 ];

		void renderMenu();
};


#endif // MENU_HPP