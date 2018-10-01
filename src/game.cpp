#include "game.hpp"

// Initialize variables
Game::Game()
{
	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
	memset( &pad, 0, sizeof( pad ) );

	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
	

	// Initialize vita2d and set clear color to black
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );

	// Initialize SoLoud engine
	gSoloud.init(); 

	// Load textures
	loadPlayerTextures();
	loadMenuTextures();
	loadCollectableTextures();
	loadGameTextures();

	// Load sounds
	loadPlayerSounds();
	loadMenuSounds();

	// Load fonts
	loadFonts();

	// Read highscores from file
	collectable.readHighscore();

	// Set highscore if none is set, or highscore is incorrect
	if( collectable.getHighscore( 0 ) > 10000 || !( collectable.getHighscore( 0 ) > 0 ) )
		collectable.writeHighscore();
	if( collectable.getHighscore( 1 ) > 10000 || !( collectable.getHighscore( 1 ) > 0 ) )
		collectable.writeHighscore();
	if( collectable.getHighscore( 2 ) > 10000 || !( collectable.getHighscore( 2 ) > 0 ) )
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
			case choosingDifficulty:
				gameDifficulty();
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

	mainMenu.menuNav();

	bool select = false;

	if( mainMenu.selectItem() )
		select = true;
	
	if( gInput.wasPressed(Input::frontTouch) )
	{
		for( int i = 0; i < mainMenu.MENU_ITEMS; ++i )
		{
			if( mainMenu.touchSelect( mainMenu.item[i] ) )
			{
				mainMenu.cursor = i;
				select = true;
			}
		}
	}
	
	if( select )
	{
		gSoloud.play( gMenuSelect );

		if( mainMenu.cursor == MainMenu::startGame )
			_gameState = choosingDifficulty;
		else if( mainMenu.cursor == MainMenu::howToPlay )
			_gameState = showingHTP;
		else if( mainMenu.cursor == MainMenu::exitGame )
			_gameState = exiting;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// Draw menu background
	mainMenu.renderBackground();								

	// Draw logo
	int text_width = vita2d_font_text_width( gJelle[ 60 ], 60, "vitaSnake" );
	vita2d_font_draw_text( gJelle[ 60 ], (SCREEN_WIDTH - text_width)/2, 110, RGBA8(0, 0, 0, 255), 60, "vitaSnake" );

	for( int i = 0; i < mainMenu.MENU_ITEMS; ++i )			
		mainMenu.renderButton( mainMenu.item[ i ] );

	mainMenu.renderCursor( mainMenu.item[ mainMenu.cursor ] );	

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Menu for choosing game difficulty
void Game::gameDifficulty()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Menu controls
	difficultyMenu.menuNav();

	bool select = false;

	if( difficultyMenu.selectItem() )
		select = true;

	if( gInput.wasPressed(Input::frontTouch) )
	{
		for( int i = 0; i < difficultyMenu.MENU_ITEMS; ++i )
		{
			if( difficultyMenu.touchSelect(difficultyMenu.item[i]) )
			{
				difficultyMenu.cursor = i;
				select = true;
			}
		}
	}
	
	if( difficultyMenu.cursor == DifficultyMenu::classic )
		GAME_DIFFICULTY = DifficultyMenu::classic;
	else if( difficultyMenu.cursor == DifficultyMenu::hardcore )
		GAME_DIFFICULTY = DifficultyMenu::hardcore;

	if( select )
	{
		gSoloud.play( gMenuSelect );

		snakePart[0].setDifficulty();
		_gameState = playing;
	}
	if( gInput.wasPressed(Input::circle) )
	{
		gSoloud.play( gMenuSelect );
		_gameState = showingMenu;
	}


	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();								

	difficultyMenu.renderBackground();												// Draw menu background

	int topText_width = vita2d_font_text_width( gJelle[ 60 ], 60, "Choose the difficulty");
	vita2d_font_draw_text( gJelle[ 60 ], (SCREEN_WIDTH - topText_width)/2, 110, RGBA8(0, 0, 0, 255), 60, "Choose the difficulty" );

	for( int i = 0; i < difficultyMenu.MENU_ITEMS; ++i )			
		difficultyMenu.renderButton( difficultyMenu.item[ i ] );

	difficultyMenu.renderDescription();

	difficultyMenu.renderCursor( difficultyMenu.item[ difficultyMenu.cursor ] );	// Draw cursor
	difficultyMenu.renderSnake();
	collectable.renderMenuScores();													// Render highscores in the menu

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Main game loop
void Game::gameLoop()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Pause the game with START
	if( gInput.wasPressed(Input::start) )
		_gameState = paused;

	// Player controls
	snakePart[0].handleInput();

	// Calcule player position
	snakePart[0].move();

	// Check snake collisions
	for( int part = 4; part < SNAKE_LENGTH; ++part )
	{
		if( snakePart[0].checkCollision( snakePart[part] ) )
		{
			gSoloud.play( gSnakeDeath );
			_gameState = gameOver;
		}
	}

	// Check wall collisions
	if( snakePart[0].wallDeath() )
	{
		gSoloud.play( gSnakeDeath );
		_gameState = gameOver;
	}
	
	// Check if the player is close to the collectable
	if( collectable.checkOpenDistance(snakePart[0]) )
	{
		snakePart[0].isClose = true;

		// Check collectable collision with snake head
		if( collectable.checkCollision( snakePart[0] ) )
		{
			gSoloud.play( gBite );
			collectable.collect();
			SNAKE_LENGTH++;
		}
	}
	else
	{
		snakePart[0].isClose = false;
	}
	
	// Move the following parts
	for( int part = 1; part < SNAKE_LENGTH; ++part )
		snakePart[ part ].follow( snakePart[ part - 1 ] );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();

	gameDraw();

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Game paused screen
void Game::gamePaused()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Menu controls
	pauseMenu.menuNav();

	bool select = false;

	if( pauseMenu.selectItem() )
		select = true;

	if( gInput.wasPressed(Input::frontTouch) )
	{
		for( int i = 0; i < pauseMenu.MENU_ITEMS; ++i )
		{
			if( pauseMenu.touchSelect(pauseMenu.item[i]) )
			{
				pauseMenu.cursor = i;
				select = true;
			}
		}
	}

	if( select )
	{
		gSoloud.play( gMenuSelect );

		if( pauseMenu.cursor == PauseMenu::resumeGame )
			_gameState = playing;
		else if( pauseMenu.cursor == PauseMenu::returnToMenu )
			_gameState = needReinitialize;
	}
	if( gInput.wasPressed(Input::start) )
	{
		_gameState = playing;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// We still want to draw everything, so the player can see the paused game
	gameDraw();

	// Dim the background
	vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA8( 0, 0, 0, 40 ) );

	int topText_width = vita2d_font_text_width( gJelle[ 60 ], 60, "Paused" );
	vita2d_font_draw_text( gJelle[ 60 ], (SCREEN_WIDTH - topText_width)/2, 110, RGBA8(0, 0, 0, 255), 60, "Paused" );

	for( int i = 0; i < pauseMenu.MENU_ITEMS; ++i )			
		pauseMenu.renderButton( pauseMenu.item[ i ] );

	pauseMenu.renderCursor( pauseMenu.item[ pauseMenu.cursor ] );

	vita2d_draw_texture( gSnakeSleep.texture, SCREEN_WIDTH-gSnakeSleep.get_width(), SCREEN_HEIGHT-gSnakeSleep.get_height() );

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Game Over screen
void Game::gameEnd()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// New highscore flag
	bool highscore;

	// Check if the player set a new highscore
	if( collectable.getScore() > collectable.getHighscore( GAME_DIFFICULTY ) )
		highscore = true;

	gameDraw();

	// Draw the Game Over text over the game
	vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA8( 0, 0, 0, 40 ) );

	int topText_width = vita2d_font_text_width( gJelle[ 60 ], 60, "Game Over");
	vita2d_font_draw_text( gJelle[ 60 ], (SCREEN_WIDTH - topText_width)/2, 110, RGBA8(0, 0, 0, 255), 60, "Game Over" );

	for( int i = 0; i < gameOverMenu.MENU_ITEMS; ++i )			
		gameOverMenu.renderButton( gameOverMenu.item[ i ] );

	gameOverMenu.renderCursor( gameOverMenu.item[ gameOverMenu.cursor ] );

	// If the player set a new highscore
	if( highscore )
	{
		if( !score_read )
		{
			collectable.writeHighscore();
			collectable.readHighscore();
			score_read = true;
		}
		int text_width = vita2d_font_text_width( gJelle[ 35 ], 35, "NEW HIGHSCORE");
		vita2d_font_draw_textf( gJelle[ 35 ], ( SCREEN_WIDTH - text_width ) / 2, 270, RGBA8(255, 255, 0, 255), 35, "NEW HIGHSCORE");
	}
	
	// Render final score
	int text_width = 0;
	if( collectable.getScore() < 10 )
		text_width = vita2d_font_text_width( gJelle[ 35 ], 35, "Your score: 0" );
	else if( collectable.getScore() < 100 )
		text_width = vita2d_font_text_width( gJelle[ 35 ], 35, "Your score: 00" );
	else
		text_width = vita2d_font_text_width( gJelle[ 35 ], 35, "Your score: 000" );

	vita2d_font_draw_textf( gJelle[ 35 ], (SCREEN_WIDTH-text_width)/2, 230, RGBA8( 0, 0, 0, 255 ), 35, "Your score: %d", collectable.getScore() );

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();


	// Menu controls
	gameOverMenu.menuNav();

	bool select = false;

	if( gameOverMenu.selectItem() )
		select = true;

	if( gInput.wasPressed( Input::frontTouch ) )
	{
		for( int i = 0; i < gameOverMenu.MENU_ITEMS; ++i )
		{
			if( gameOverMenu.touchSelect( gameOverMenu.item[i] ) )
			{
				gameOverMenu.cursor = i;
				select = true;
			}
		}
	}

	if( select )
	{
		score_read = false;

		gSoloud.play( gMenuSelect );

		if( gameOverMenu.cursor == GameOverMenu::playAgain )
			_gameState = playAgain;
		else if( gameOverMenu.cursor == GameOverMenu::returnToMenu )
			_gameState = needReinitialize;
	}
}

// Destroy textures on game exit
void Game::gameQuit()
{
	vita2d_fini();

	// Free textures
	gSnakeHeadTexture.freeTexture();
	gSnakeHeadOpenTexture.freeTexture();
	gSnakeBodyTexture.freeTexture();
	gSnakeTailTexture.freeTexture();

	gAppleTexture.freeTexture();
	gSparkleTexture.freeTexture();

	gMenuButtonTexture.freeTexture();
	gCursorTexture.freeTexture();

	gSnakeNormal.freeTexture();
	gSnakeHard.freeTexture();
	gSnakeSleep.freeTexture();

	gBgTexture.freeTexture();

	// Free fonts
	for( int i = 0; i <= 99; ++i )
		vita2d_free_font( gJelle[ i ] );
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

	_gameState = choosingDifficulty;
}

// Show the "How to play" screen
void Game::gameHTP()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Press O to go back
	if( gInput.wasPressed(Input::circle) )
	{
		gSoloud.play( gMenuSelect );
		_gameState = showingMenu;
	}	

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	gBgTexture.draw( 0.0f, 0.0f );

	int text_width;

	text_width = vita2d_font_text_width( gJelle[ 35 ], 35, "How to play" );
	vita2d_font_draw_text( gJelle[ 35 ], (SCREEN_WIDTH-text_width)/2, 40, RGBA8( 0, 0, 0, 255 ), 35, "How to play" );

	vita2d_font_draw_text( gJelle[ 20 ], 15, 100, RGBA8( 0, 0, 0, 255 ), 20, "Your goal in this game is to eat as many apples as you can," );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 130, RGBA8( 0, 0, 0, 255 ), 20, "each apple eaten adds a point to your score." );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 160, RGBA8( 0, 0, 0, 255 ), 20, "Avoid biting your own tail, or you will lose the game." );

	vita2d_font_draw_text( gJelle[ 20 ], 15, 260, RGBA8( 0, 0, 0, 255 ), 20, "- The snake moves forward by itself." );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 310, RGBA8( 0, 0, 0, 255 ), 20, "- Steer the snake left and right using the left analog stick or DPAD" );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 340, RGBA8( 0, 0, 0, 255 ), 20, "  buttons." );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 390, RGBA8( 0, 0, 0, 255 ), 20, "- You can boost the snake's speed by holding the X button." );
	vita2d_font_draw_text( gJelle[ 20 ], 15, 440, RGBA8( 0, 0, 0, 255 ), 20, "- Pause the game with the START button." );

	text_width = vita2d_font_text_width( gJelle[ 25 ], 25, "Press O to go back" );
	vita2d_font_draw_text( gJelle[ 25 ], SCREEN_WIDTH-text_width-15, SCREEN_HEIGHT-15, RGBA8( 0, 0, 0, 255 ), 25, "Press O to go back." );

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Draw every gameplay element
void Game::gameDraw()
{
	difficultyMenu.renderBackground();

	collectable.render();			

	// Draw snake
	for( int part = 1; part < SNAKE_LENGTH-1; ++part )
		snakePart[ part ].render( Player::body );
	snakePart[ SNAKE_LENGTH-1 ].render( Player::tail );
	snakePart[ 0 ].render( Player::head );

	// Draw text
	collectable.renderScore();
	collectable.renderHighscore();
}