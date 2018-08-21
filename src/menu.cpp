#include "menu.hpp"

// Move cursor up an item
void Menu::selectUp()
{
	if( cursor != 0 )
		cursor--;
	else
		cursor = MENU_ITEMS-1;
}

// Move cursor down an item
void Menu::selectDown()
{
	if( cursor != MENU_ITEMS-1 )
		cursor++;
	else
		cursor = 0;
}

// Render the cursor
void Menu::renderCursor( MenuItem item )
{
	vita2d_draw_texture( gCursorTexture.texture, item.x, item.y );
}


// Main menu initialization
MainMenu::MainMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 3;

	// Initialize menu items
	item[0].name = startGame;
	item[0].x = 310.0f;
	item[0].y = 207.0f;

	item[1].name = howToPlay;
	item[1].x = 310.0f;
	item[1].y = 310.0f;

	item[2].name = exitGame;
	item[2].x = 310.0f;
	item[2].y = 411.0f;

	// Initialize cursor
	cursor = startGame;
}

void MainMenu::renderBackground()
{
	vita2d_draw_texture( gMainMenuBgTexture.texture, 0.0f, 0.0f );
}


PauseMenu::PauseMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = resumeGame;
	item[0].x = 310.0f;
	item[0].y = 310.0f;

	item[1].name = returnToMenu;
	item[1].x = 310.0f;
	item[1].y = 411.0f;

	// Initialize cursor
	cursor = resumeGame;
}

void PauseMenu::renderBackground()
{
	vita2d_draw_texture( gPauseMenuBgTexture.texture, 0.0f, 0.0f );
}


GameOverMenu::GameOverMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = playAgain;
	item[0].x = 310.0f;
	item[0].y = 310.0f;

	item[1].name = returnToMenu;
	item[1].x = 310.0f;
	item[1].y = 411.0f;

	// Initialize cursor
	cursor = playAgain;
}

void GameOverMenu::renderBackground()
{
	vita2d_draw_texture( gGameOverMenuBgTexture.texture, 0.0f, 0.0f );
}