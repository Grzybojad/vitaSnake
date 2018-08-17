#include "menu.hpp"

Menu::Menu()
{
	// Initialize vita2d and set clear color to black
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );

	// Initialize menu items
	item[0].name = startGame;
	item[0].x = 310.0f; // TODO
	item[0].y = 207.0f; // TODO

	item[1].name = howToPlay;
	item[1].x = 310.0f; // TODO
	item[1].y = 310.0f; // TODO

	item[2].name = exitGame;
	item[2].x = 310.0f;	// TODO
	item[2].y = 411.0f;	// TODO

	// Initialize cursor
	cursor = startGame;

	// Set textures
	menuTexture = vita2d_load_PNG_file( "app0:/img/menuScreen.png" );
	cursorTexture = vita2d_load_PNG_file( "app0:/img/cursor.png" );
}

// Move cursor up an item
void Menu::selectUp()
{
	switch( cursor )
	{
		case startGame:
			cursor = exitGame;
			break;
		case howToPlay:
			cursor = startGame;
			break;
		case exitGame:
			cursor = howToPlay;
			break;
	}
}

// Move cursor down an item
void Menu::selectDown()
{
	switch( cursor )
	{
		case startGame:
			cursor = howToPlay;
			break;
		case howToPlay:
			cursor = exitGame;
			break;
		case exitGame:
			cursor = startGame;
			break;
	}
}

// Render the menu background image
void Menu::renderBackground()
{
	vita2d_draw_texture( menuTexture, 0.0f, 0.0f );
}

// Render the cursor
void Menu::renderCursor( MenuItem item )
{
	vita2d_draw_texture( cursorTexture, item.x, item.y );
}