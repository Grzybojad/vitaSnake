#include "collectable.hpp"

// Initializer
Collectable::Collectable()
{
	srand( time( NULL ) );

	// Random position on screen, but not too close to the edges
	xPos = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	yPos = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	score = 0;

	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		particles[i] = new Particle( xPos, yPos );
	}
}

// Player-collectable collision
bool Collectable::checkCollision( Player part )
{
	float xMid = xPos + ( COLLECT_WIDTH / 2 );
	float yMid = yPos + ( COLLECT_HEIGHT / 2 );

	float distance = sqrt( pow( ( part.get_xPos() - xMid ), 2) + pow( ( part.get_yPos() - yMid ), 2 ) );
	if( distance < COLLISION_DISTANCE )
		return true;
	else
		return false;
}

// Check if the player is close to the collectable
bool Collectable::checkOpenDistance( Player part )
{
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );
	if( distance < OPEN_DISTANCE )
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
	vita2d_draw_texture( gAppleTexture.texture, xPos, yPos );

	renderParticles();
}

// Render the score counter
void Collectable::renderScore()
{
	int text_width;

	if( score < 10 )
		text_width = vita2d_font_text_width( gJelle[ 20 ], 20, "SCORE: 0" );
	else if( score < 100 )
		text_width = vita2d_font_text_width( gJelle[ 20 ], 20, "SCORE: 00" );
	else
		text_width = vita2d_font_text_width( gJelle[ 20 ], 20, "SCORE: 000" );

	vita2d_font_draw_textf( gJelle[ 20 ], SCREEN_WIDTH-text_width-10, 30, RGBA8(0, 0, 0, 255), 20, "SCORE: %d", score );
}

void Collectable::renderParticles()
{
	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		if( particles[i]->isDead() )
		{
			delete particles[i];
			particles[i] = new Particle( xPos + ( COLLECT_WIDTH / 2), yPos + ( COLLECT_HEIGHT / 2) );
		}
	}

	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		particles[i]->render();
	}
}

// Get the score number
int Collectable::getScore()
{
	return score;
}

// Reset the score counter
void Collectable::resetScore()
{
	score = 0;
}

// Read highscore from file
void Collectable::readHighscore()
{
	std::ifstream scoreList;

	scoreList.open( "ux0:/data/vitaSnake/highscores.txt", std::ifstream::in );
	scoreList >> classicHighscore;
	scoreList >> hardcoreHighscore;
	scoreList.close();
}

int Collectable::getHighscore( int difficulty )
{
	switch( difficulty )
	{
		case DifficultyMenu::classic:
			return classicHighscore;
		case DifficultyMenu::hardcore:
			return hardcoreHighscore;
	}
}


// Write new highscore to file
void Collectable::writeHighscore()
{
	std::ofstream scoreList;
	sceIoMkdir("ux0:/data/vitaSnake", 0777);
	scoreList.open( "ux0:/data/vitaSnake/highscores.txt" );

	switch( GAME_DIFFICULTY )
	{
		case DifficultyMenu::classic:
			scoreList << score << "\n" << hardcoreHighscore;
			break;
		case DifficultyMenu::hardcore:
			scoreList << classicHighscore << "\n" << score;
			break;
	}

	scoreList.close();
}


// Render the highscore text
void Collectable::renderHighscore()
{
	switch( GAME_DIFFICULTY )
	{
		case DifficultyMenu::classic:
			vita2d_font_draw_textf( gJelle[ 20 ], 10, 30, RGBA8(0, 0, 0, 255), 20, "HIGHSCORE: %d", classicHighscore );
			break;
		case DifficultyMenu::hardcore:
			vita2d_font_draw_textf( gJelle[ 20 ], 10, 30, RGBA8(0, 0, 0, 255), 20, "HIGHSCORE: %d", hardcoreHighscore );
			break;
	}
}

// Render menu scores
void Collectable::renderMenuScores()
{
	vita2d_font_draw_textf( gJelle[ 40 ], 660, 250, RGBA8(0, 0, 0, 255), 40, "HIGH: %d", classicHighscore );
	vita2d_font_draw_textf( gJelle[ 40 ], 660, 350, RGBA8(0, 0, 0, 255), 40, "HIGH: %d", hardcoreHighscore );
}