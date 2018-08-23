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
void Collectable::setFont()
{
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
	vita2d_draw_texture_rotate( gAppleTexture.texture, xPos, yPos, 0.0f );
}

// Render the score counter
void Collectable::renderScore()
{
	vita2d_pgf_draw_textf( pgf, 830, 30, RGBA8(255, 255, 0, 255), 1.0f, "SCORE: %d", score );
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
	scoreList >> easyHighscore;
	scoreList >> normalHighscore;
	scoreList >> hardHighscore;
	scoreList.close();
}

int Collectable::getHighscore( int difficulty )
{
	switch( difficulty )
	{
	case 0:
		return easyHighscore;
	case 1:
		return normalHighscore;
	case 2:
		return hardHighscore;
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
	case 0:
		scoreList << score << "\n" << normalHighscore << "\n" << hardHighscore;
		break;
	case 1:
		scoreList << easyHighscore << "\n" << score << "\n" << hardHighscore;
		break;
	case 2:
		scoreList << easyHighscore << "\n" << normalHighscore << "\n" << score;
		break;
	}

	scoreList.close();
}


// Render the highscore text
void Collectable::renderHighscore()
{
	switch( GAME_DIFFICULTY )
	{
		case 0:
			vita2d_pgf_draw_textf( pgf, 10, 30, RGBA8(255, 255, 0, 255), 1.0f, "HIGHSCORE: %d", easyHighscore );
			break;
		case 1:
			vita2d_pgf_draw_textf( pgf, 10, 30, RGBA8(255, 255, 0, 255), 1.0f, "HIGHSCORE: %d", normalHighscore );
			break;
		case 2:
			vita2d_pgf_draw_textf( pgf, 10, 30, RGBA8(255, 255, 0, 255), 1.0f, "HIGHSCORE: %d", hardHighscore );
			break;
	}
}

// Render menu scores
void Collectable::renderMenuScores()
{
	vita2d_pgf_draw_textf( pgf, 660, 240, RGBA8(255, 255, 0, 255), 1.4f, "HIGH: %d", easyHighscore );
	vita2d_pgf_draw_textf( pgf, 660, 340, RGBA8(255, 255, 0, 255), 1.4f, "HIGH: %d", normalHighscore );
	vita2d_pgf_draw_textf( pgf, 660, 444, RGBA8(255, 255, 0, 255), 1.4f, "HIGH: %d", hardHighscore );
}