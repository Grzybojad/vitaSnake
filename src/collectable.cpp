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
		vec3 newParticlePos;
		newParticlePos.x = pos.x + rand() % COLLECT_WIDTH - (COLLECT_WIDTH/2);
		newParticlePos.y = pos.y + rand() % COLLECT_HEIGHT - (COLLECT_HEIGHT/2);
		particles[i] = new Particle( newParticlePos );
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
int Collectable::collect( vec3 playerPos )
{
	if( ENABLE_PARTICLES )
	{
		// Explode the apple
		for( int i = 0; i < MAX_EXPLOSION_PARTICLES; ++i )
		{
			vec3 newParticlePos;
			newParticlePos.x = (playerPos.x + sin( playerPos.r ) * 20) + (COLLECT_WIDTH/2);
			newParticlePos.y = (playerPos.y - cos( playerPos.r ) * 20) + (COLLECT_HEIGHT/2);

			int randSide = rand() % 2;
			if( randSide == 0 )
				newParticlePos.r = playerPos.r + ( (M_PI/2) + (rand() % 2 - 1)*(M_PI/10) );
			else
				newParticlePos.r = playerPos.r - ( (M_PI/2) + (rand() % 2 - 1)*(M_PI/10) );

			explosionParticles.push_back( new Particle( newParticlePos ) );
		}
	}

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

	if( ENABLE_ANIMATION )
	{
		pos.r = sin( animation_step ) * M_PI/10;
		animation_step += ANIMATION_SPEED * timestep;

		if( animation_step > 2*M_PI ) 
			animation_step -= 2*M_PI;
	}
	else
	{
		pos.r = 0;
	}
	
	vita2d_draw_texture_rotate( collectableTextures[ APPLE_TEXTURE ].texture, xMid, yMid, pos.r );

	if( ENABLE_PARTICLES )
		renderParticles();
}

// Render the score counter
void Collectable::renderScore()
{
	unsigned int text_size = 25;
	int padding_top = 20;
	int padding_side = 10;

	drawTextf_position( alignRight, SCREEN_WIDTH - padding_side, padding_top, text_size, "SCORE: %d", score );
}

void Collectable::renderParticles()
{
	// Replace old "shimmering" particles and render
	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		if( particles[i]->isDead() )
		{
			delete particles[i];
			vec3 newParticlePos;
			newParticlePos.x = pos.x + (COLLECT_WIDTH / 2) + rand() % COLLECT_WIDTH - (COLLECT_WIDTH/2);
			newParticlePos.y = pos.y + (COLLECT_HEIGHT / 2) + rand() % COLLECT_HEIGHT - (COLLECT_HEIGHT/2);
			particles[i] = new Particle( newParticlePos );
		}

		particles[i]->render();
	}

	// Remove "explosion" particles if they're all dead
	for( int i = 0; i < explosionParticles.size(); ++i )
	{
		if( !explosionParticles[i]->isDead() )
			continue;
		
		if( i == explosionParticles.size()-1 )
			explosionParticles.clear();
	}

	// Move "explosion particles" and render
	for( int i = 0; i < explosionParticles.size(); ++i )
	{
		// We only render living paricles
		if( !explosionParticles[i]->isDead() )
		{
			// Move exploded particles
			explosionParticles[i]->move();

			// Some dumb math to find a different texture part for each particle
			int formulaX = ( i % (int)sqrt(MAX_EXPLOSION_PARTICLES) ) * ( collectableTextures[APPLE_TEXTURE].get_width() / (int)sqrt(MAX_EXPLOSION_PARTICLES) );
			int formulaY = ( i / (int)sqrt(MAX_EXPLOSION_PARTICLES) ) * ( collectableTextures[APPLE_TEXTURE].get_height() / (int)sqrt(MAX_EXPLOSION_PARTICLES) );

			// We don't want to use the rim of the texture for particles, since it's likely that it's blank
			int texturePartX = ( formulaX * 0.6 ) + collectableTextures[APPLE_TEXTURE].get_width() * 0.2;
			int texturePartY = ( formulaY * 0.6 ) + collectableTextures[APPLE_TEXTURE].get_height() * 0.2;

			explosionParticles[i]->renderTexturePart( collectableTextures[APPLE_TEXTURE].texture, texturePartX, texturePartY );
		}
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
		drawTextf( padding_side, padding_top, text_size, "HIGHSCORE: %d", highscore[ GAME_MODE ].casual );
	else
		drawTextf( padding_side, padding_top, text_size, "HIGHSCORE: %d", highscore[ GAME_MODE ].hardcore );
}

// Render menu scores
void Collectable::renderMenuScores()
{
	int scoreSize = 40;

	drawTextf( 660, 250, scoreSize, "HIGH: %d", highscore[ GAME_MODE ].casual );
	drawTextf( 660, 350, scoreSize, "HIGH: %d", highscore[ GAME_MODE ].hardcore );
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

void Collectable::reset()
{
	// Reset position
	pos.x = rand() % SCREEN_WIDTH*0.8 + SCREEN_WIDTH*0.1;
	pos.y = rand() % SCREEN_HEIGHT*0.8 + SCREEN_HEIGHT*0.1;

	// Reset score
	score = 0;
	SCORE_ADD = 1;

	// Reset particles
	for( int i = 0; i < TOTAL_PARTICLES; ++i )
	{
		vec3 newParticlePos;
		newParticlePos.x = pos.x + rand() % COLLECT_WIDTH - (COLLECT_WIDTH/2);
		newParticlePos.y = pos.y + rand() % COLLECT_HEIGHT - (COLLECT_HEIGHT/2);
		particles[i] = new Particle( newParticlePos );
	}
}