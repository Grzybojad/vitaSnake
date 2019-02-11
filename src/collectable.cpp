#include "collectable.hpp"

// Initializer
Collectable::Collectable()
{
	srand( time( NULL ) );

	// Random position on screen, but not too close to the edges
	pos.x = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	pos.y = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	score = 0;
	SCORE_ADD = 1;

	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		particles[i] = new Particle( pos.x, pos.y );
	}
}

// Player-collectable collision
bool Collectable::checkCollision( vec3 playerPos )
{
	float xMid = pos.x + ( COLLECT_WIDTH / 2 );
	float yMid = pos.y + ( COLLECT_HEIGHT / 2 );

	float distance = sqrt( pow( ( playerPos.x - xMid ), 2) + pow( ( playerPos.y - yMid ), 2 ) );
	if( distance < COLLISION_DISTANCE )
		return true;
	else
		return false;
}

// Check if the player is close to the collectable
bool Collectable::checkOpenDistance( vec3 playerPos )
{
	float distance = sqrt( pow( ( playerPos.x - pos.x ), 2) + pow( ( playerPos.y - pos.y ), 2 ) );
	if( distance < OPEN_DISTANCE )
		return true;
	else
		return false;
}

// Pick up the collectable
int Collectable::collect()
{
	pos.x = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	pos.y = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	score += SCORE_ADD;

	// Return how many snake parts should be added
	if( GAME_MODE == ModeMenu::fibonacci )
	{
		if ( score < 16 )
			return fibonacci[ score ];
		else
			return fibonacci[ 15 ];
	}
	else
	{
		return 1;
	}
}

// Render the collectable
void Collectable::render()
{
	float xMid = pos.x + ( COLLECT_WIDTH / 2 );
	float yMid = pos.y + ( COLLECT_HEIGHT / 2 );

	pos.r = sin( animation_step ) * M_PI/10;
	animation_step += ANIMATION_SPEED * timestep;
	if( animation_step > 2*M_PI ) animation_step -= 2*M_PI; 

	vita2d_draw_texture_rotate( collectableTextures[ APPLE_TEXTURE ].texture, xMid, yMid, pos.r );

	renderParticles();
}

// Render the score counter
void Collectable::renderScore()
{
	int text_width;
	unsigned int text_size = 25;
	int padding_top = 20;
	int padding_side = 10;

	if( score < 10 )
		text_width = vita2d_font_text_width( gFont[ (int)(text_size * FONT_SCALE) ], (int)(30 * FONT_SCALE), "SCORE: 0" );
	else if( score < 100 )
		text_width = vita2d_font_text_width( gFont[ (int)(text_size * FONT_SCALE) ], (int)(30 * FONT_SCALE), "SCORE: 00" );
	else
		text_width = vita2d_font_text_width( gFont[ (int)(text_size * FONT_SCALE) ], (int)(30 * FONT_SCALE), "SCORE: 000" );

	vita2d_font_draw_textf( gFont[ (int)(text_size * FONT_SCALE) ], SCREEN_WIDTH - text_width + padding_side, padding_top, MAIN_FONT_COLOR, (int)(text_size * FONT_SCALE), "SCORE: %d", score );
}

void Collectable::renderParticles()
{
	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		if( particles[i]->isDead() )
		{
			delete particles[i];
			particles[i] = new Particle( pos.x + ( COLLECT_WIDTH / 2), pos.y + ( COLLECT_HEIGHT / 2) );
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
		// Classic
		scoreList >> highscore[0].casual;
		scoreList >> highscore[0].hardcore;
		// Time Trial
		scoreList >> highscore[1].casual;
		scoreList >> highscore[1].hardcore;
		// Hyper
		scoreList >> highscore[2].casual;
		scoreList >> highscore[2].hardcore;
		// Fibonacci
		scoreList >> highscore[3].casual;
		scoreList >> highscore[3].hardcore;
		// Lazy
		scoreList >> highscore[4].casual;
		scoreList >> highscore[4].hardcore;
	scoreList.close();
}

int Collectable::getHighscore()
{
	if( GAME_DIFFICULTY == 0 ) 
		return highscore[ GAME_MODE ].casual;
	else
		return highscore[ GAME_MODE ].hardcore;
}


// Write highscore to file
void Collectable::writeHighscore()
{
	std::ofstream scoreList;
	sceIoMkdir("ux0:/data/vitaSnake", 0777);
	scoreList.open( "ux0:/data/vitaSnake/highscores.txt" );

	for( int i = 0; i < 5; ++i )
	{
		scoreList << highscore[i].casual << "\n";
		scoreList << highscore[i].hardcore << "\n";
	}
	scoreList.close();
}

// Write new highscore to file
void Collectable::writeNewHighscore()
{
	std::ofstream scoreList;
	sceIoMkdir("ux0:/data/vitaSnake", 0777);
	scoreList.open( "ux0:/data/vitaSnake/highscores.txt" );

	for( int i = 0; i < 5; ++i )
	{
		if( GAME_MODE != i )
		{
			scoreList << highscore[i].casual << "\n";
			scoreList << highscore[i].hardcore << "\n";
		}
		else if( GAME_DIFFICULTY == 0 )
		{
			scoreList << score << "\n";
			scoreList << highscore[i].hardcore << "\n";
		}
		else if( GAME_DIFFICULTY == 1 )
		{
			scoreList << highscore[i].casual << "\n";
			scoreList << score << "\n";
		}
	}
	scoreList.close();
}


// Render the highscore text
void Collectable::renderHighscore()
{
	unsigned int text_size = 25;
	int padding_top = 20;
	int padding_side = 10;

	if( GAME_DIFFICULTY == DifficultyMenu::classic )
		vita2d_font_draw_textf( gFont[ (int)(text_size * FONT_SCALE) ], padding_side, padding_top, MAIN_FONT_COLOR, (int)(text_size * FONT_SCALE), "HIGHSCORE: %d", highscore[ GAME_MODE ].casual );
	else
		vita2d_font_draw_textf( gFont[ (int)(text_size * FONT_SCALE) ], padding_side, padding_top, MAIN_FONT_COLOR, (int)(text_size * FONT_SCALE), "HIGHSCORE: %d", highscore[ GAME_MODE ].hardcore );
}

// Render menu scores
void Collectable::renderMenuScores()
{
	vita2d_font_draw_textf( gFont[ (int)(40 * FONT_SCALE) ], 660, 250, MAIN_FONT_COLOR, (int)(40 * FONT_SCALE), "HIGH: %d", highscore[ GAME_MODE ].casual );
	vita2d_font_draw_textf( gFont[ (int)(40 * FONT_SCALE) ], 660, 350, MAIN_FONT_COLOR, (int)(40 * FONT_SCALE), "HIGH: %d", highscore[ GAME_MODE ].hardcore );
}

void Collectable::setPos( float x, float y )
{
	pos.x = x;
	pos.y = y;
}

void Collectable::checkAndFixHighscores()
{
	for( int i = 0; i < 5; ++i )
	{
		if( highscore[i].casual < 0 || highscore[i].casual > 10000 )
			highscore[i].casual = 0;
		if( highscore[i].hardcore < 0 || highscore[i].hardcore > 10000 )
			highscore[i].hardcore = 0;
	}

	writeHighscore();
}