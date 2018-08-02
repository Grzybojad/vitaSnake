#include "collectable.hpp"

// Initializer
Collectable::Collectable()
{
	srand( time( NULL ) );

	// Random position on screen, but not too close to the edges
	xPos = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	yPos = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	score = 0;
}

// Assign a texture to the object
void Collectable::setTextureAndFont( const char *filename )
{
	texture = vita2d_load_PNG_file( filename );

	pgf = vita2d_load_default_pgf();
}

bool Collectable::checkCollision( Player part )
{
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );
	if( distance < COLLISION_DISTANCE )
		return true;
	else
		return false;
}

// Pick up the collectable
void Collectable::collect()
{
	xPos = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	yPos = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	score++;
}

// Render the collectable
void Collectable::render()
{
	vita2d_draw_texture_rotate( texture, xPos, yPos, 0.0f );
}

// Render the score counter
void Collectable::renderScore()
{
	vita2d_pgf_draw_textf( pgf, 740, 30, RGBA8(255, 255, 0, 255), 1.0f, "SCORE: %d", getScore() );
}


// Get the score number
int Collectable::getScore()
{
	return score;
}

// Clear textures
void Collectable::destroyTextures()
{
	vita2d_free_texture( texture );
}