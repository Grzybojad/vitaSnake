#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <stdint.h>
#include <math.h>

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

// Game difficulty
extern int GAME_DIFFICULTY;

// Input
extern Input gInput;


/* Options */
extern int CONTROL_STYLE;


// Textures
extern Texture gSnakeHeadTexture;
extern Texture gSnakeHeadOpenTexture;
extern Texture gSnakeBodyTexture;
extern Texture gSnakeTailTexture;

extern Texture gAppleTexture;
extern Texture gSparkleTexture;

extern Texture gMenuButtonTexture;
extern Texture gCursorTexture;

extern Texture gSnakeEasy;
extern Texture gSnakeNormal;
extern Texture gSnakeHard;
extern Texture gSnakeSleep;

extern Texture gBgTexture;


// Sounds
extern SoLoud::Wav gBite;
extern SoLoud::Wav gSnakeDeath;
extern SoLoud::Wav gMenuMove;
extern SoLoud::Wav gMenuSelect;


// Fonts
extern vita2d_font *gJelle[ 99 ];	// Font sizes 1-99


// Texture loading functions
extern void loadPlayerTextures();
extern void loadMenuTextures();
extern void loadCollectableTextures();
extern void loadGameTextures();

/* Sound loading functions */
extern void loadPlayerSounds();
extern void loadMenuSounds();


/* Font loading functions */
extern void loadFonts();


#endif // GLOBAL_HPP
