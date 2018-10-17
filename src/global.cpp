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
int PLAYER_TEXTURES = 0;


// Textures
Texture gSnakeSheet[ 2 ];

Texture gAppleTexture;
Texture gSparkleTexture;

Texture gMenuButtonTexture;
Texture gCursorTexture;

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
	gSnakeSheet[ 0 ].texture = vita2d_load_PNG_file( "app0:/img/playerDefault.png" );
	gSnakeSheet[ 1 ].texture = vita2d_load_PNG_file( "app0:/img/playerClassic.png" );

	for( int i = 0; i < 4; ++i )
		for( int j = 0; j < 2; ++j )
			gSnakeSheet[ j ].setClips( i, i * 30, 0, 30, 40 );
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
	vita2d_draw_texture_part_scale_rotate( gSnakeSheet[ PLAYER_TEXTURES ].texture, x, y, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].x, 
		gSnakeSheet[ PLAYER_TEXTURES ].clips[part].y, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].w, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].h, 1, 1, rad );
}

// Function overload with a scale factor
void drawPlayer( part part, float x, float y, float scale_x, float scale_y, float rad )
{
	vita2d_draw_texture_part_scale_rotate( gSnakeSheet[ PLAYER_TEXTURES ].texture, x, y, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].x,
		gSnakeSheet[ PLAYER_TEXTURES ].clips[part].y, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].w, gSnakeSheet[ PLAYER_TEXTURES ].clips[part].h, scale_x, scale_y, rad );
}