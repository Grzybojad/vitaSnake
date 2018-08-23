#include "global.hpp"

// Screen dimentions
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

// Game difficulty
int GAME_DIFFICULTY = 0;

// Textures
Texture gSnakeHeadTexture;
Texture gSnakeBodyTexture;
Texture gSnakeTailTexture;
Texture gAppleTexture;
Texture gCursorTexture;
Texture gMainMenuBgTexture;
Texture gDifficultyMenuBgTexture;
Texture gPauseMenuBgTexture;
Texture gGameOverMenuBgTexture;
Texture gInfoTexture;
Texture gEasySnake;
Texture gNormalSnake;
Texture gHardSnake;

// Texture loading functions
void loadPlayerTextures()
{
	gSnakeHeadTexture.texture = vita2d_load_PNG_file( "app0:/img/head.png" );
	gSnakeBodyTexture.texture = vita2d_load_PNG_file( "app0:/img/body.png" );
	gSnakeTailTexture.texture = vita2d_load_PNG_file( "app0:/img/tail.png" );
}

void loadCollectableTextures()
{
	gAppleTexture.texture = vita2d_load_PNG_file( "app0:/img/apple.png" );
}

void loadMenuTextures()
{
	gCursorTexture.texture				= vita2d_load_PNG_file( "app0:/img/cursor.png" );
	gMainMenuBgTexture.texture			= vita2d_load_PNG_file( "app0:/img/menuScreen.png" );
	gDifficultyMenuBgTexture.texture	= vita2d_load_PNG_file( "app0:/img/difficulty.png" );
	gPauseMenuBgTexture.texture			= vita2d_load_PNG_file( "app0:/img/pauseScreen.png" );
	gGameOverMenuBgTexture.texture		= vita2d_load_PNG_file( "app0:/img/overScreen.png" );
	gInfoTexture.texture				= vita2d_load_PNG_file( "app0:/img/infoScreen.png" );

	gEasySnake.texture					= vita2d_load_PNG_file( "app0:/img/snakeEasy.png" );
	gNormalSnake.texture				= vita2d_load_PNG_file( "app0:/img/snakeNormal.png" );
	gHardSnake.texture					= vita2d_load_PNG_file( "app0:/img/snakeHard.png" );
}
