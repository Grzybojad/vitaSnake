#include "global.hpp"

// Screen dimentions
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

// SoLoud engine
SoLoud::Soloud gSoloud;

// Game difficulty
int GAME_DIFFICULTY = 0;

// Input
Input gInput;


// Textures
Texture gSnakeHeadTexture;
Texture gSnakeHeadOpenTexture;
Texture gSnakeBodyTexture;
Texture gSnakeTailTexture;

Texture gAppleTexture;
Texture gSparkleTexture;

Texture gMenuButtonTexture;
Texture gCursorTexture;

Texture gSnakeEasy;
Texture gSnakeNormal;
Texture gSnakeHard;
Texture gSnakeSleep;

Texture gBgTexture;


// Sounds
SoLoud::Wav gBite;
SoLoud::Wav gSnakeDeath;
SoLoud::Wav gMenuMove;
SoLoud::Wav gMenuSelect;


// Fonts
vita2d_font *gJelle[99];


/* Texture loading functions */
void loadPlayerTextures()
{
	gSnakeHeadTexture.texture		= vita2d_load_PNG_file( "app0:/img/head.png" );
	gSnakeHeadOpenTexture.texture	= vita2d_load_PNG_file( "app0:/img/headOpen.png" );
	gSnakeBodyTexture.texture		= vita2d_load_PNG_file( "app0:/img/body.png" );
	gSnakeTailTexture.texture		= vita2d_load_PNG_file( "app0:/img/tail.png" );
}

void loadCollectableTextures()
{
	gAppleTexture.texture		= vita2d_load_PNG_file( "app0:/img/apple.png" );
	gSparkleTexture.texture		= vita2d_load_PNG_file( "app0:/img/sparkle.png" );
}

void loadMenuTextures()
{
	gMenuButtonTexture.texture			= vita2d_load_PNG_file( "app0:/img/menuButton.png" );
	gCursorTexture.texture				= vita2d_load_PNG_file( "app0:/img/cursor.png" );

	gSnakeNormal.texture				= vita2d_load_PNG_file( "app0:/img/snakeNormal.png" );
	gSnakeHard.texture					= vita2d_load_PNG_file( "app0:/img/snakeHard.png" );
	gSnakeSleep.texture					= vita2d_load_PNG_file( "app0:/img/snakeSleep.png" );
}

void loadGameTextures()
{
	gBgTexture.texture	 = vita2d_load_PNG_file( "app0:/img/bg.png" );
}


/* Sound loading functions*/
void loadPlayerSounds()
{
	gBite.load( "app0:/sound/bite.wav" );
	gSnakeDeath.load( "app0:/sound/snake_death.ogg");
}

void loadMenuSounds()
{
	gMenuMove.load( "app0:/sound/menu_move.wav");
	gMenuSelect.load( "app0:/sound/menu_select.wav");
}


/* Font loading functions */
void loadFonts()
{
	for( int i = 0; i <= 99; ++i )
		gJelle[ i ]	= vita2d_load_font_file( "app0:/font/Jellee-Roman.ttf" );
}