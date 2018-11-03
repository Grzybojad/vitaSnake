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
int BACKGROUND_TEXTURE = 0;
float FONT_SCALE = 0.6;
int unsigned MAIN_FONT_COLOR = RGBA8( 0, 0, 0, 255 );


// Textures
Texture gSnakeSheet[ NR_PLAYER_TEXTURES ];

Texture gAppleTexture;
Texture gSparkleTexture;

Texture gMenuButtonTexture;
Texture gCursorTexture;
Texture gCrossTexture;
Texture gCircleTexture;

Texture gSnakeHard;
Texture gSnakeSleep;

Texture gBgTexture[ NR_BACKGROUND_TEXTURES ];
//Texture gBgTexture;


// Sounds
SoLoud::Wav gBite;
SoLoud::Wav gSnakeDeath;
SoLoud::Wav gMenuMove;
SoLoud::Wav gMenuSelect;


// Fonts
vita2d_font *gFont[99];


/* Texture loading functions */
void loadPlayerTextures()
{
	gSnakeSheet[ 0 ].texture = vita2d_load_PNG_file( "app0:/img/playerDefault.png" );
	gSnakeSheet[ 1 ].texture = vita2d_load_PNG_file( "app0:/img/playerClassic.png" );
	gSnakeSheet[ 2 ].texture = vita2d_load_PNG_file( "app0:/img/playerRPPHS.png" );
	gSnakeSheet[ 3 ].texture = vita2d_load_PNG_file( "app0:/img/playerNokia.png" );

	for( int i = 0; i < 5; ++i )
		for( int j = 0; j < NR_PLAYER_TEXTURES; ++j )
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
	gCrossTexture.texture			= vita2d_load_PNG_file( "app0:/img/button_cross.png" );
	gCircleTexture.texture			= vita2d_load_PNG_file( "app0:/img/button_circle.png" );
}

void loadGameTextures()
{
	gBgTexture[ 0 ].texture	 = vita2d_load_PNG_file( "app0:/img/bgDesertTile.png" );
	gBgTexture[ 1 ].texture = vita2d_load_PNG_file( "app0:/img/bgRPPHSFull.png" );
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
		gFont[ i ]	= vita2d_load_font_file( "app0:/font/font_main.ttf" );
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


// Background variables
bool color_plus = true;
int border_red = 200;

// Draw background
extern void drawBackground()
{
	switch( BACKGROUND_TEXTURE )
	{
		case 0:	// Desert
			gBgTexture[ 0 ].fill_tile();
			break;

		case 1:	// Classic
			// Just leave the background black
			break;

		case 2:	// RPPHS
			gBgTexture[ 1 ].draw();
			break;

		case 3:	// Nokia
			vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, SCREEN_WIDTH, RGBA8( 117, 134, 99, 255 ) );
			break;
	}

	if( GAME_DIFFICULTY == 1 )
	{
		// Change the color of the border for dramatic effect
		if( border_red >= 254 )
		{
			border_red = 255;
			color_plus = false;
		}
		else if( border_red <= 170 )
		{
			border_red = 170;
			color_plus = true;
		}

		if( color_plus )
			border_red += COLOR_CYCLE_SPEED;
		else
			border_red -= COLOR_CYCLE_SPEED;
	
		// Draw a red border
		vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, BORDER_THICKNESS, RGBA8( border_red, 0, 0, 255 ) );
		vita2d_draw_rectangle( SCREEN_WIDTH - BORDER_THICKNESS, 0, BORDER_THICKNESS, SCREEN_HEIGHT, RGBA8( border_red, 0, 0, 255 ) );
		vita2d_draw_rectangle( 0, SCREEN_HEIGHT - BORDER_THICKNESS, SCREEN_WIDTH, BORDER_THICKNESS, RGBA8( border_red, 0, 0, 255 ) );
		vita2d_draw_rectangle( 0, 0, BORDER_THICKNESS, SCREEN_HEIGHT, RGBA8( border_red, 0, 0, 255 ) );
	}
}


// Draw "Press O to go back" text
extern void drawBackText()
{
	int text_width;

	text_width = vita2d_font_text_width( gFont[ (int)(25 * FONT_SCALE) ], (int)(25 * FONT_SCALE), "Press   to go back" );
	vita2d_font_draw_text( gFont[ (int)(25 * FONT_SCALE) ], SCREEN_WIDTH - text_width - 15, SCREEN_HEIGHT - 15, MAIN_FONT_COLOR, (int)(25 * FONT_SCALE), "Press   to go back" );

	text_width = vita2d_font_text_width( gFont[ (int)(25 * FONT_SCALE) ], (int)(25 * FONT_SCALE), "  to go back" );
	gCircleTexture.draw_scale( SCREEN_WIDTH - text_width - 17, SCREEN_HEIGHT - 10 - ( gCircleTexture.get_height() * 0.35 ), 0.35, 0.35 );
}
