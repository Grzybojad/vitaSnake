#include "global.hpp"

// Screen dimentions
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

SoLoud::Soloud gSoloud;
Input gInput;

SceUInt64 prevFrameClock = 0;
SceUInt64 frameTime = 0;
float timestep = 0;

int GAME_DIFFICULTY = 0;
int GAME_MODE = 0;
gameState _gameState;

/* Options */
int CONTROL_STYLE = 0;
int PLAYER_TEXTURES = 0;
int BACKGROUND_TEXTURE = 0;
int APPLE_TEXTURE = 0;
float FONT_SCALE = 0.6;
int unsigned MAIN_FONT_COLOR = RGBA8( 0, 0, 0, 255 );
bool ENABLE_PARTICLES = true;
bool ENABLE_ANIMATION = true;
bool ENABLE_BACKTOUCH = false;

/* Textures */
std::vector <Texture> snakeTextures;
std::vector <Texture> bgTextures;
std::vector <Texture> collectableTextures;

Texture gSparkleTexture;

Texture gMenuButtonTexture;
Texture gCursorTexture;
Texture gCrossTexture;
Texture gCircleTexture;


/* Sounds */
SoLoud::Wav gBite;
SoLoud::Wav gSnakeDeath;
SoLoud::Wav gMenuMove;
SoLoud::Wav gMenuSelect;


/* Fonts */
vita2d_font *gFont[99];


void calcFrameTime()
{	
	frameTime = ( sceKernelGetProcessTimeWide() - prevFrameClock );
	calcTimestep();

	prevFrameClock = sceKernelGetProcessTimeWide();
}

void calcTimestep()
{
	timestep = frameTime / 16666.6666;
}

/* Texture loading functions */
void loadPlayerTextures()
{
	snakeTextures.push_back( loadTexture( "app0:/img/playerDefault.png" ) );
	snakeTextures.back().name = "Default";
	snakeTextures.push_back( loadTexture( "app0:/img/playerClassic.png" ) );
	snakeTextures.back().name = "Classic";
	snakeTextures.push_back( loadTexture( "app0:/img/playerRPPHS.png" ) );
	snakeTextures.back().name = "RPPHS";
	snakeTextures.push_back( loadTexture( "app0:/img/playerNokia.png" ) );
	snakeTextures.back().name = "Nokia";

	for( int i = 0; i < 5; ++i )
		for( int j = 0; j < snakeTextures.size(); ++j )
			snakeTextures[ j ].setClips( i, i * 30, 0, 30, 40 );
}

void loadCollectableTextures()
{
	collectableTextures.push_back( loadTexture( "app0:/img/appleDefault.png" ) );
	collectableTextures.back().name = "Default";
	collectableTextures.push_back( loadTexture( "app0:/img/appleClassic.png" ) );
	collectableTextures.back().name = "Classic";
	collectableTextures.push_back( loadTexture( "app0:/img/appleRPPHS.png" ) );
	collectableTextures.back().name = "RPPHS";
	collectableTextures.push_back( loadTexture( "app0:/img/appleNokia.png" ) );
	collectableTextures.back().name = "Nokia";
	collectableTextures.push_back( loadTexture( "app0:/img/appleBattery.png" ) );
	collectableTextures.back().name = "Battery";
	collectableTextures.push_back( loadTexture( "app0:/img/appleMushroom.png" ) );
	collectableTextures.back().name = "Mushroom";

	gSparkleTexture.texture	= vita2d_load_PNG_file( "app0:/img/sparkle.png" );
}

void loadMenuTextures()
{
	gCrossTexture.texture = vita2d_load_PNG_file( "app0:/img/button_cross.png" );
	gCircleTexture.texture = vita2d_load_PNG_file( "app0:/img/button_circle.png" );
}

void loadGameTextures()
{
	bgTextures.push_back( loadTexture( "app0:/img/bgDesertTile.png" ) );
	bgTextures.back().name = "Desert";
	bgTextures.push_back( loadTexture( "app0:/img/bgClassic.png" ) );
	bgTextures.back().name = "Classic";
	bgTextures.push_back( loadTexture( "app0:/img/bgRPPHSFull.png" ) );
	bgTextures.back().name = "RPPHS";
	bgTextures.push_back( loadTexture( "app0:/img/bgNokia.png" ) );
	bgTextures.back().name = "Nokia";
}

Texture loadTexture( const char *path )
{
	Texture texture;
	texture.texture = vita2d_load_PNG_file( path );
	return texture;
}


/* Sound loading functions */
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
		gFont[ i ] = vita2d_load_font_file( "app0:/font/font_main.ttf" );
}


// Draw player character in specified location
void drawPlayer( part part, float x, float y, float rad )
{
	vita2d_draw_texture_part_scale_rotate( snakeTextures[ PLAYER_TEXTURES ].texture, x, y, snakeTextures[ PLAYER_TEXTURES ].clips[part].x, 
		snakeTextures[ PLAYER_TEXTURES ].clips[part].y, snakeTextures[ PLAYER_TEXTURES ].clips[part].w, snakeTextures[ PLAYER_TEXTURES ].clips[part].h, 1, 1, rad );
}

// Function overload with a scale factor
void drawPlayer( part part, float x, float y, float scale_x, float scale_y, float rad )
{
	vita2d_draw_texture_part_scale_rotate( snakeTextures[ PLAYER_TEXTURES ].texture, x, y, snakeTextures[ PLAYER_TEXTURES ].clips[part].x,
		snakeTextures[ PLAYER_TEXTURES ].clips[part].y, snakeTextures[ PLAYER_TEXTURES ].clips[part].w, snakeTextures[ PLAYER_TEXTURES ].clips[part].h, scale_x, scale_y, rad );
}


// Background variables
bool color_plus = true;
float border_red = 200;

// Draw background
void drawBackground()
{
	if( bgTextures[ BACKGROUND_TEXTURE ].name == "RPPHS" )
		bgTextures[ BACKGROUND_TEXTURE ].draw();
	else
		bgTextures[ BACKGROUND_TEXTURE ].fill_tile();

	int unsigned border_color = 0;

	if( GAME_DIFFICULTY == 1 )
	{
		// For Nokia theme
		if( BACKGROUND_TEXTURE == 3 )
		{
			border_color = MAIN_FONT_COLOR;
		}
		else
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
				border_red += COLOR_CYCLE_SPEED * timestep;
			else
				border_red -= COLOR_CYCLE_SPEED;

			border_color = RGBA8( (int)border_red, 0, 0, 255 );
		}

		// Draw a border
		vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, BORDER_THICKNESS, border_color );
		vita2d_draw_rectangle( SCREEN_WIDTH - BORDER_THICKNESS, 0, BORDER_THICKNESS, SCREEN_HEIGHT, border_color );
		vita2d_draw_rectangle( 0, SCREEN_HEIGHT - BORDER_THICKNESS, SCREEN_WIDTH, BORDER_THICKNESS, border_color );
		vita2d_draw_rectangle( 0, 0, BORDER_THICKNESS, SCREEN_HEIGHT, border_color );
	}
}


// Draw "Press O to go back" text
void drawBackText()
{
	int text_width;

	drawText_position( alignRight, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 15, 25, "Press   to go back" );

	text_width = vita2d_font_text_width( gFont[ (int)(25 * FONT_SCALE) ], (int)(25 * FONT_SCALE), "  to go back" );
	gCircleTexture.draw_scale( SCREEN_WIDTH - text_width - 17, SCREEN_HEIGHT - 10 - ( gCircleTexture.get_height() * 0.35 ), 0.35, 0.35 );
}

const char* getGameModeName()
{
	const char* mode_name = "";
	if( GAME_MODE == 0 )
		mode_name = "Classic";
	else if( GAME_MODE == 1 )
		mode_name = "Time Trial";
	else if( GAME_MODE == 2 )
		mode_name = "Hyper";
	else if( GAME_MODE == 3 )
		mode_name = "Fibonacci";
	else if( GAME_MODE == 4 )
		mode_name = "Lazy Snake";

	return mode_name;
}

std::string timeToString( SceUInt64 time )
{
	std::string clock = "";

	int second = 1;

	int hours = time / (second*60*60);
	int minutes = ( time % (second*60*60) / (second*60) );
	int seconds = ( time % (second*60) ) / second;

	if( hours > 0 )
	{
		clock.append(std::to_string(hours) );
		clock.append(":");
	}

	clock.append(std::to_string(minutes / 10) );
	clock.append(std::to_string(minutes % 10) );

	clock.append(":");

	clock.append(std::to_string(seconds / 10) );
	clock.append(std::to_string(seconds % 10) );
	
	return clock;
}

void drawText( int x, int y, unsigned int size, const char *text )
{
	vita2d_font_draw_text( gFont[ (int)(size * FONT_SCALE) ], x, y, MAIN_FONT_COLOR, (int)(size * FONT_SCALE), text );
}

void drawTextf( int x, int y, unsigned int size, const char *text, ... )
{
	char buf[1024];
	va_list argptr;

	va_start(argptr, text);
	vsnprintf(buf, sizeof(buf), text, argptr);
	va_end(argptr);

	return drawText( x, y, size, buf );
}

void drawText_color( int x, int y, unsigned int size, unsigned int color, const char *text )
{
	vita2d_font_draw_text( gFont[ (int)(size * FONT_SCALE) ], x, y, color, (int)(size * FONT_SCALE), text );
}

void drawTextf_color( int x, int y, unsigned int size, unsigned int color, const char *text, ... )
{
	char buf[1024];
	va_list argptr;

	va_start( argptr, text );
	vsnprintf( buf, sizeof(buf), text, argptr );
	va_end( argptr );

	return drawText_color( x, y, size, color, buf );
}

void drawText_position( position p, int x, int y, unsigned int size, const char *text )
{
	int w = vita2d_font_text_width( gFont[ (int)(size * FONT_SCALE) ], (int)(size * FONT_SCALE), text );
	int h = vita2d_font_text_height( gFont[ (int)(size * FONT_SCALE) ], (int)(size * FONT_SCALE), text );

	switch( p )
	{
		case normal:
		default:
			return drawText( x, y, size, text );

		case centered:
			return drawText( x - (w/2), y + (h/2), size, text );

		case centeredX:
			return drawText( x - (w/2), y, size, text );

		case centeredY:
			return drawText( x, y + (h/2), size, text );

		case alignRight:
			return drawText( x - w, y, size, text );
	}
}

void drawTextf_position( position p, int x, int y, unsigned int size, const char *text, ... )
{
	char buf[1024];
	va_list argptr;

	va_start( argptr, text );
	vsnprintf( buf, sizeof(buf), text, argptr );
	va_end( argptr );

	return drawText_position( p, x, y, size, buf );
}

void drawText_color_position( position p, int x, int y, unsigned int size, unsigned int color, const char *text )
{
	int w = vita2d_font_text_width( gFont[ (int)(size * FONT_SCALE) ], (int)(size * FONT_SCALE), text );
	int h = vita2d_font_text_height( gFont[ (int)(size * FONT_SCALE) ], (int)(size * FONT_SCALE), text );

	switch( p )
	{
		case normal:
		default:
			return drawText_color( x, y, size, color, text );

		case centered:
			return drawText_color( x - (w/2), y + (h/2), size, color, text );

		case centeredX:
			return drawText_color( x - (w/2), y, size, color, text );

		case centeredY:
			return drawText_color( x, y + (h/2), size, color, text );

		case alignRight:
			return drawText_color( x - w, y, size, color, text );
	}
}

void drawTextf_color_position( position p, int x, int y, unsigned int size, unsigned int color, const char *text, ... )
{
	char buf[1024];
	va_list argptr;

	va_start( argptr, text );
	vsnprintf( buf, sizeof(buf), text, argptr );
	va_end( argptr );

	return drawText_color_position( p, x, y, size, color, buf );
}