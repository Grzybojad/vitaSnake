#include "game.hpp"

Game::Game()
{
	_gameState = 0;
}

void Game::gameStart()
{
	if( _gameState != 0 )	// uninitialized
		return;

	_gameState = 2;	// playing

	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
	memset( &pad, 0, sizeof( pad ) );

	// Initialize vita2d and set clear color to black
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );

	SNAKE_LENGTH = 3;

	// Set snake textures
	snakePart[0].setTexture( "app0:/img/head.png" );
	for( int part = 1; part < SNAKE_LENGTH-1; ++part )
		snakePart[ part ].setTexture( "app0:/img/body.png" );
	snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );

	// Set collectable texture
	collectable.setTextureAndFont( "app0:/img/apple.png" );

	pvf = vita2d_load_default_pvf();

	// Button bools
	startPressed = false;

	while( _gameState != 5 )
	{
		switch( _gameState )
		{
			case 0: // uninitialized
				gameStart();
				break;
			case 1:	// showingMenu
				gameMenu();
				break;
			case 2:	// playing
				gameLoop();
				break;
			case 3:	// paused
				gamePaused();
				break;
			case 4:	// gameOver
				gameEnd();
				break;
		}
	}
	gameQuit();

	vita2d_fini();
	return;
}

bool Game::isExiting()
{
	return false;
}

void Game::gameMenu()
{

}

void Game::gameLoop()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Dumb way of checking if the start was pressed, not held down
	if( (pad.buttons & SCE_CTRL_START) && !startPressed )
	{
		startPressed = true;

		// Pause the game
		_gameState = 3;
	}
	else if( !(pad.buttons & SCE_CTRL_START) )
	{
		startPressed = false;
	}


	// Player controls
	snakePart[0].handleInput();

	// Calcule player position
	snakePart[0].move();

	// Check snake collisions
	for( int part = 4; part < SNAKE_LENGTH; ++part )
	{
		if( snakePart[0].checkCollision( snakePart[part] ) )
			_gameState = 4; // gameOver
	}
	

	// Check collectable collisions
	if( collectable.checkCollision( snakePart[0] ) )
	{
		collectable.collect();

		snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/body.png" );
		SNAKE_LENGTH++;
		snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );
	}

	// Move the following parts
	for( int part = 1; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].follow( snakePart[ part - 1 ] );


	/* RENDERING */
	vita2d_start_drawing();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );
	vita2d_clear_screen();

	// Draw snake
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].render();

	// Draw collectable
	collectable.render();

	// Draw the score counter
	collectable.renderScore();

	vita2d_end_drawing();
	vita2d_swap_buffers();
}

void Game::gamePaused()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_set_clear_color( RGBA8( 0x10, 0x10, 0x10, 0xFF ) );
	vita2d_clear_screen();	

	// Dumb way of checking if the start was pressed, not held down
	if( (pad.buttons & SCE_CTRL_START) && !startPressed )
	{
		startPressed = true;

		// Unpause the game
		_gameState = 2;
	}
	else if( !(pad.buttons & SCE_CTRL_START) )
	{
		startPressed = false;
	}


	// We still want to draw everything, so the player can see the paused game
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].render();
	collectable.render();
	collectable.renderScore();

	// Draw pause text
	vita2d_pvf_draw_text( pvf, 330, 180, RGBA8( 255, 0, 0, 255 ), 4.0f, "PAUSED" );
	vita2d_pvf_draw_text( pvf, 340, 270, RGBA8( 255, 0, 0, 255 ), 1.4f, "Press START to unpause" );

	vita2d_end_drawing();
	vita2d_swap_buffers();
}


void Game::gameEnd()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_set_clear_color( RGBA8( 0x10, 0x10, 0x10, 0xFF ) );
	vita2d_clear_screen();	

	// Dumb way of checking if the start was pressed, not held down
	if( (pad.buttons & SCE_CTRL_START) && !startPressed )
	{
		startPressed = true;

		// Quit the game
		_gameState = 5;	// quitting
	}
	else if( !(pad.buttons & SCE_CTRL_START) )
	{
		startPressed = false;
	}

	// Restart the game when the user presses X
	if( pad.buttons & SCE_CTRL_CROSS )
	{
		_gameState = 0;	// uninitialized

		// Reset the player and collectable
		collectable.collect();
		collectable.score = 0;
		snakePart[0].xPos = SCREEN_WIDTH / 6;
		snakePart[0].yPos = SCREEN_HEIGHT / 2;
		snakePart[0].rotation = M_PI / 2;
	}


	// We still want to draw everything, so the player can see the ended game
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].render();
	collectable.render();
	collectable.renderScore();

	// Draw text over the game
	vita2d_pvf_draw_text ( pvf, 300, 180, RGBA8( 255, 0, 0, 255 ), 4.0f, "GAME OVER" );
	vita2d_pvf_draw_textf( pvf, 340, 270, RGBA8( 255, 0, 0, 255 ), 1.8f, "Your score was: %d", collectable.getScore() );
	vita2d_pvf_draw_text ( pvf, 350, 410, RGBA8( 255, 0, 0, 255 ), 1.4f, "Press START to quit" );
	vita2d_pvf_draw_text ( pvf, 370, 450, RGBA8( 255, 0, 0, 255 ), 1.4f, "Press X to restart" );

	vita2d_end_drawing();
	vita2d_swap_buffers();
}


void Game::gameQuit()
{
	// Wait for GPU to stop rendering
	vita2d_fini();

	// Destroy snake textures
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].destroyTextures();

	// Destroy collectable texture
	collectable.destroyTextures();
}