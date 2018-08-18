#include "game.hpp"

// Initialize variables
Game::Game()
{
	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
	memset( &pad, 0, sizeof( pad ) );

	// Initialize vita2d and set clear color to black
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );

	// The starting length of the snake
	SNAKE_LENGTH = 3;

	// Set snake textures
	snakePart[0].setTexture( "app0:/img/head.png" );
	for( int part = 1; part < SNAKE_LENGTH-1; ++part )
		snakePart[ part ].setTexture( "app0:/img/body.png" );
	snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );

	// Set collectable texture
	collectable.setTextureAndFont( "app0:/img/apple.png" );

	pvf = vita2d_load_default_pvf();

	// Game screen textures
	pauseTexture = vita2d_load_PNG_file( "app0:/img/pauseScreen.png" );
	overTexture = vita2d_load_PNG_file( "app0:/img/overScreen.png" );
	infoTexture = vita2d_load_PNG_file( "app0:/img/infoScreen.png" );


	// Set highscore if none is set
	if( collectable.getHighscore() > 10000 || !( collectable.getHighscore() > 0 ) )
	{
		collectable.writeHighscore();
	}

	_gameState = initialized;
}

// Start the game
void Game::gameStart()
{
	if( _gameState != initialized )
		return;

	_gameState = showingMenu;

	while( _gameState != exiting )
	{
		switch( _gameState )
		{
			case initialized:
				gameStart();
				break;
			case showingMenu:
				gameMenu();
				break;
			case playing:
				gameLoop();
				break;
			case paused:
				gamePaused();
				break;
			case gameOver:
				gameEnd();
				break;
			case needReinitialize:
				gameReinitialize();
				break;
			case showingHTP:
				gameHTP();
				break;
		}
	}
	gameQuit();

	vita2d_fini();
	return;
}

// The first screen the user sees
void Game::gameMenu()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Menu controls
	if( input.wasPressed(Input::up) )
	{
		mainMenu.selectUp();
	}
	if( input.wasPressed(Input::down) )
	{
		mainMenu.selectDown();
	}
	if( input.wasPressed(Input::cross) )
	{
		if( mainMenu.cursor == startGame )
			_gameState = playing;
		else if( mainMenu.cursor == howToPlay )
			_gameState = showingHTP;
		else if( mainMenu.cursor == exitGame )
			_gameState = exiting;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	mainMenu.renderBackground();								// Draw menu background
	mainMenu.renderCursor( mainMenu.item[ mainMenu.cursor ] );	// Draw cursor
	collectable.renderHighscore();								// Draw highscore text

	vita2d_end_drawing();
	vita2d_swap_buffers();
}

// Main game loop
void Game::gameLoop()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Pause the game with START
	if( input.wasPressed(Input::start) )
		_gameState = paused;

	// Player controls
	snakePart[0].handleInput();

	// Calcule player position
	snakePart[0].move();

	// Check snake collisions
	for( int part = 4; part < SNAKE_LENGTH; ++part )
	{
		if( snakePart[0].checkCollision( snakePart[part] ) )
			_gameState = gameOver;
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
	vita2d_clear_screen();

	for( int part = 0; part < SNAKE_LENGTH; ++part ) 
		snakePart[ part ].render();	// draw snake

	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	vita2d_end_drawing();
	vita2d_swap_buffers();
}

// Game paused screen
void Game::gamePaused()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// Unpause the game with START
	if( input.wasPressed(Input::start) )
		_gameState = playing;

	// We still want to draw everything, so the player can see the paused game
	for( int part = 0; part < SNAKE_LENGTH; ++part ) 
		snakePart[ part ].render();	// draw snake
	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	// Draw pause screen image
	vita2d_draw_texture( pauseTexture, 0.0f, 0.0f );

	vita2d_end_drawing();
	vita2d_swap_buffers();
}

// Game Over screen
void Game::gameEnd()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Check if the player set a new highscore
	if( collectable.score > collectable.getHighscore() )
		collectable.writeHighscore();

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// We still want to draw everything, so the player can see the game even after they lost
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].render();	// draw snake
	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	// Draw game over image
	vita2d_draw_texture( overTexture, 0.0f, 0.0f );
	vita2d_pvf_draw_textf( pvf, 420, 370, RGBA8( 255, 255, 0, 255 ), 5.0f, "%d", collectable.getScore() );

	vita2d_end_drawing();
	vita2d_swap_buffers();

	// Restart the game when the user presses X
	if( input.wasPressed(Input::cross) )
		_gameState = needReinitialize;

	// Quit the game with START
	if( input.wasPressed(Input::start) )
		_gameState = exiting;
}

// Destroy textures on game exit
void Game::gameQuit()
{
	// Wait for GPU to stop rendering
	vita2d_fini();

	// Destroy snake textures
	for( int part = 0; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].destroyTextures();

	// Destroy collectable texture
	collectable.destroyTextures();

	// Destroy screen textures
	vita2d_free_texture( pauseTexture );
	vita2d_free_texture( overTexture );
	vita2d_free_texture( infoTexture );
}

// Re-initialize variables
void Game::gameReinitialize()
{
	// Reset the player and collectable
	collectable.collect();
	collectable.score = 0;
	snakePart[0].xPos = SCREEN_WIDTH / 6;
	snakePart[0].yPos = SCREEN_HEIGHT / 2;
	snakePart[0].rotation = M_PI / 2;

	// The starting length of the snake
	SNAKE_LENGTH = 3;

	// Set snake textures
	snakePart[0].setTexture( "app0:/img/head.png" );
	for( int part = 1; part < SNAKE_LENGTH-1; ++part )
		snakePart[ part ].setTexture( "app0:/img/body.png" );
	snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );

	_gameState = initialized;
}

void Game::gameHTP()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// Press O to go back
	if( input.wasPressed(Input::start) )
		_gameState = showingMenu;

	// TO DO change this
	vita2d_draw_texture( infoTexture, 0.0f, 0.0f );

	vita2d_end_drawing();
	vita2d_swap_buffers();
}
