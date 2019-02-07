#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <stdint.h>
#include <math.h>
#include <vector>

#include <psp2/ctrl.h>
#include <psp2/audioout.h>
#include <psp2/kernel/processmgr.h>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_speech.h"

#include <vita2d.h>

#include "texture.hpp"
#include "buttons.hpp"

// Pi define
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Screen dimentions
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// SoLoud engine
extern SoLoud::Soloud gSoloud;

// Input
extern Input gInput;

extern SceUInt64 prevFrameClock; // The time between the start of the program and the previous frame
extern SceUInt64 frameTime;		// Time between frames (in microseconds)
extern float timestep;			// The multiplier for time sensitive actions (1.0 for 60FPS)

// Game difficulty
extern int GAME_DIFFICULTY;

// The length at the start of the game;
static const int START_SNAKE_LENGTH = 3;

// Game version
static const char *VERSION = "1.4 WIP";


enum gameState
{
	initialized = 0,
	showingMenu = 1,
	choosingDifficulty = 2,
	playing = 3,
	paused = 4,
	gameOver = 5,
	exiting = 6,
	needReinitialize = 7,
	playAgain = 8,
	showingHTP = 9,
	options = 10
};
extern gameState _gameState;

enum part
{
	tail = 0,
	body = 1,
	head = 2,
	headOpen = 3,
	headAngry = 4
};

struct vec3
{
	float x;
	float y;
	float r;

	vec3()
	{ 
		x = SCREEN_WIDTH / 6;
		y = SCREEN_HEIGHT / 2;
		r = M_PI / 2;
	};
};

/* Options */
extern int CONTROL_STYLE;
extern int PLAYER_TEXTURES;
extern int BACKGROUND_TEXTURE;
extern int APPLE_TEXTURE;
extern float FONT_SCALE;
extern unsigned int MAIN_FONT_COLOR;


// Textures
extern std::vector <Texture> snakeTextures;

extern Texture gSparkleTexture;

extern Texture gCrossTexture;
extern Texture gCircleTexture;

extern std::vector <Texture> bgTextures;

extern std::vector <Texture> collectableTextures;


// Sounds
extern SoLoud::Wav gBite;
extern SoLoud::Wav gSnakeDeath;
extern SoLoud::Wav gMenuMove;
extern SoLoud::Wav gMenuSelect;


// Fonts
extern vita2d_font *gFont[ 99 ];	// Font sizes 1-99


// Calculate time between frames
void calcFrameTime();

// TimeStep is frameTime normalized in a way that it's 1 for 60 FPS
void calcTimestep();


// Texture loading functions
extern void loadPlayerTextures();
extern void loadMenuTextures();
extern void loadCollectableTextures();
extern void loadGameTextures();
extern Texture loadTexture( const char *path );

// Sound loading functions
extern void loadPlayerSounds();
extern void loadMenuSounds();


// Font loading functions
extern void loadFonts();


// Draw player character in specified location
extern void drawPlayer( part part, float x, float y, float rad );
extern void drawPlayer( part part, float x, float y, float scale_x, float scale_y, float rad );


// Hardcore border variables
static const int BORDER_THICKNESS = 3;
static const int COLOR_CYCLE_SPEED = 3;
extern bool color_plus;
extern int border_red;

// Draw background
extern void drawBackground();


// Draw "Press O to go back" text
extern void drawBackText();


#endif // GLOBAL_HPP
