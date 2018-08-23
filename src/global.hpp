#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <vita2d.h>
#include "texture.hpp"

// Pi define
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Screen dimentions
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Game difficulty
extern int GAME_DIFFICULTY;

// Textures
extern Texture gSnakeHeadTexture;
extern Texture gSnakeBodyTexture;
extern Texture gSnakeTailTexture;
extern Texture gAppleTexture;
extern Texture gCursorTexture;
extern Texture gMainMenuBgTexture;
extern Texture gDifficultyMenuBgTexture;
extern Texture gPauseMenuBgTexture;
extern Texture gGameOverMenuBgTexture;
extern Texture gInfoTexture;
extern Texture gEasySnake;
extern Texture gNormalSnake;
extern Texture gHardSnake;

// Texture loading functions
extern void loadPlayerTextures();
extern void loadMenuTextures();
extern void loadCollectableTextures();

#endif // GLOBAL_HPP
