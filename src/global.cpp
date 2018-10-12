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


/* Options */
int CONTROL_STYLE = 0;


// Textures
Texture gSnakeSheet;

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
	gSnakeSheet.texture				= vita2d_load_PNG_file( "app0:/img/playerDefault.png" );
	for( int i = 0; i < 4; ++i )
	{
		gSnakeSheet.setClips( i, i * 30, 0, 30, 40 );
	}
}

void loadCollectableTextures()
{
	gAppleTexture.texture			= vita2d_load_PNG_file( "app0:/img/apple.png" );
	gSparkleTexture.texture			= vita2d_load_PNG_file( "app0:/img/sparkle.png" );
}

void loadMenuTextures()
{
	gMenuButtonTexture.texture		= vita2d_load_PNG_file( "app0:/img/menuButton.png" );
	gCursorTexture.texture			= vita2d_load_PNG_file( "app0:/img/cursor.png" );

	gSnakeNormal.texture			= vita2d_load_PNG_file( "app0:/img/snakeNormal.png" );
	gSnakeHard.texture				= vita2d_load_PNG_file( "app0:/img/snakeHard.png" );
	gSnakeSleep.texture				= vita2d_load_PNG_file( "app0:/img/snakeSleep.png" );
}

void loadGameTextures()
{
	gBgTexture.texture	 = vita2d_load_PNG_file( "app0:/img/bg.png" );
}


// Sound loading functions
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


// Font loading functions
void loadFonts()
{
	for( int i = 0; i <= 99; ++i )
		gJelle[ i ]	= vita2d_load_font_file( "app0:/font/Jellee-Roman.ttf" );
}


// Draw player character in specified location
void drawPlayer( part part, float x, float y, float rad )
{
	switch( part )
	{
		case tail:
			vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[0].x, gSnakeSheet.clips[0].y, gSnakeSheet.clips[0].w, gSnakeSheet.clips[0].h, 1, 1, rad );
			break;

		case body:
			vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[1].x, gSnakeSheet.clips[1].y, gSnakeSheet.clips[1].w, gSnakeSheet.clips[1].h, 1, 1, rad );
			break;

		case head:
			vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[2].x, gSnakeSheet.clips[2].y, gSnakeSheet.clips[2].w, gSnakeSheet.clips[2].h, 1, 1, rad );
			break;

		case headOpen:
			vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[3].x, gSnakeSheet.clips[3].y, gSnakeSheet.clips[3].w, gSnakeSheet.clips[3].h, 1, 1, rad );
	}
}

// Function overload with a scale factor
void drawPlayer( part part, float x, float y, float scale_x, float scale_y, float rad )
{
	switch( part )
	{
	case tail:
		vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[0].x, gSnakeSheet.clips[0].y, gSnakeSheet.clips[0].w, gSnakeSheet.clips[0].h, scale_x, scale_y, rad );
		break;

	case body:
		vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[1].x, gSnakeSheet.clips[1].y, gSnakeSheet.clips[1].w, gSnakeSheet.clips[1].h, scale_x, scale_y, rad );
		break;

	case head:
		vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[2].x, gSnakeSheet.clips[2].y, gSnakeSheet.clips[2].w, gSnakeSheet.clips[2].h, scale_x, scale_y, rad );
		break;

	case headOpen:
		vita2d_draw_texture_part_scale_rotate( gSnakeSheet.texture, x, y, gSnakeSheet.clips[3].x, gSnakeSheet.clips[3].y, gSnakeSheet.clips[3].w, gSnakeSheet.clips[3].h, scale_x, scale_y, rad );
	}
}