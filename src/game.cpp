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

	pvf = vita2d_load_default_pvf();

	// Load textures
	loadPlayerTextures();
	loadMenuTextures();
	loadCollectableTextures();

	// Load collectable font
	collectable.setFont();

	// Set highscore if none is set
	if( collectable.getHighscore() > 10000 || !( collectable.getHighscore() > 0 ) )
		collectable.writeHighscore();

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
			case playAgain:
				gamePlayAgain();
				break;
			case showingHTP:
				gameHTP();
				break;
		}
	}
	gameQuit();

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
		if( mainMenu.cursor == MainMenu::startGame )
			_gameState = playing;
		else if( mainMenu.cursor == MainMenu::howToPlay )
			_gameState = showingHTP;
		else if( mainMenu.cursor == MainMenu::exitGame )
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
		SNAKE_LENGTH++;
	}

	// Move the following parts
	for( int part = 1; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].follow( snakePart[ part - 1 ] );


	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();

	// Draw snake
	snakePart[ 0 ].render( Player::head );	
	for( int part = 1; part < SNAKE_LENGTH-1; ++part ) 
		snakePart[ part ].render( Player::body );
	snakePart[ SNAKE_LENGTH-1 ].render( Player::tail );

	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Game paused screen
void Game::gamePaused()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Menu controls
	if( input.wasPressed(Input::up) )
	{
		pauseMenu.selectUp();
	}
	if( input.wasPressed(Input::down) )
	{
		pauseMenu.selectDown();
	}
	if( input.wasPressed(Input::cross) )
	{
		if( pauseMenu.cursor == PauseMenu::resumeGame )
			_gameState = playing;
		else if( pauseMenu.cursor == PauseMenu::returnToMenu )
			_gameState = needReinitialize;
	}
	if( input.wasPressed(Input::start) )
	{
		_gameState = playing;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// We still want to draw everything, so the player can see the paused game
	// Draw snake
	snakePart[ 0 ].render( Player::head );	
	for( int part = 1; part < SNAKE_LENGTH-1; ++part ) 
		snakePart[ part ].render( Player::body );
	snakePart[ SNAKE_LENGTH-1 ].render( Player::tail );

	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	pauseMenu.renderBackground();									// Draw menu background
	pauseMenu.renderCursor( pauseMenu.item[ pauseMenu.cursor ] );	// Draw cursor

	vita2d_end_drawing();
	vita2d_swap_buffers();
}

// Game Over screen
void Game::gameEnd()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Check if the player set a new highscore
	if( collectable.getScore() > collectable.getHighscore() )
		collectable.writeHighscore();

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// We still want to draw everything, so the player can see the game even after they lost
	// Draw snake
	snakePart[ 0 ].render( Player::head );	
	for( int part = 1; part < SNAKE_LENGTH-1; ++part ) 
		snakePart[ part ].render( Player::body );
	snakePart[ SNAKE_LENGTH-1 ].render( Player::tail );

	collectable.render();			// draw collectable
	collectable.renderScore();		// draw the score counter
	collectable.renderHighscore();	// draw highscore text

	// Draw game over image
	gameOverMenu.renderBackground();
	gameOverMenu.renderCursor( gameOverMenu.item[ gameOverMenu.cursor ] );

	// Render final score
	int text_width = vita2d_pvf_text_width(pvf, 2.0f, "Your score: 00" );
	vita2d_pvf_draw_textf( pvf, (SCREEN_WIDTH-text_width)/2, 230, RGBA8( 255, 255, 0, 255 ), 2.0f, "Your score: %d", collectable.getScore() );

	vita2d_end_drawing();
	vita2d_swap_buffers();

	// Menu controls
	if( input.wasPressed(Input::up) )
	{
		gameOverMenu.selectUp();
	}
	if( input.wasPressed(Input::down) )
	{
		gameOverMenu.selectDown();
	}
	if( input.wasPressed(Input::cross) )
	{
		if( gameOverMenu.cursor == GameOverMenu::playAgain )
			_gameState = playAgain;
		else if( gameOverMenu.cursor == GameOverMenu::returnToMenu )
			_gameState = needReinitialize;
	}
}

// Destroy textures on game exit
void Game::gameQuit()
{
	// Wait for GPU to stop rendering
	vita2d_fini();

	// Free textures
	gSnakeHeadTexture.freeTexture();
	gSnakeBodyTexture.freeTexture();
	gSnakeTailTexture.freeTexture();
	gAppleTexture.freeTexture();
	gCursorTexture.freeTexture();
	gMainMenuBgTexture.freeTexture();
	gPauseMenuBgTexture.freeTexture();
	gGameOverMenuBgTexture.freeTexture();
	gInfoTexture.freeTexture();
}

// Re-initialize variables
void Game::gameReinitialize()
{
	// Reset the player and collectable
	collectable.collect();
	collectable.resetScore();
	snakePart[0].resetPos();

	// The starting length of the snake
	SNAKE_LENGTH = START_SNAKE_LENGTH;

	_gameState = showingMenu;
}

// Re-initialize variables
void Game::gamePlayAgain()
{
	// Reset the player and collectable
	collectable.collect();
	collectable.resetScore();
	snakePart[0].resetPos();

	// The starting length of the snake
	SNAKE_LENGTH = START_SNAKE_LENGTH;

	_gameState = playing;
}

void Game::gameHTP()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// Press O to go back
	if( input.wasPressed(Input::circle) )
		_gameState = showingMenu;

	// Draw how to play info texture
	gInfoTexture.draw( 0.0f, 0.0f );

	vita2d_end_drawing();
	vita2d_swap_buffers();
}
