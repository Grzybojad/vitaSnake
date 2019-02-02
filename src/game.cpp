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
			case options:
				gameOptions();
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

	GAME_DIFFICULTY = 0;

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
		else if( mainMenu.cursor == MainMenu::options )
			_gameState = options;
		else if( mainMenu.cursor == MainMenu::exitGame )
			_gameState = exiting;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	// Draw menu background
	mainMenu.renderBackground();								

	// Draw logo
	int text_width = vita2d_font_text_width( gFont[ (int)( 60 * FONT_SCALE ) ], (int)(60 * FONT_SCALE), "vitaSnake" );
	vita2d_font_draw_text( gFont[ (int)(60 * FONT_SCALE) ], (SCREEN_WIDTH - text_width)/2, 110, MAIN_FONT_COLOR, (int)(60 * FONT_SCALE), "vitaSnake" );

	mainMenu.renderCursor( mainMenu.item[ mainMenu.cursor ] );	

	for( int i = 0; i < mainMenu.MENU_ITEMS; ++i )
		mainMenu.renderButton( mainMenu.item[ i ] );

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

		snake.setDifficulty();
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

	int topText_width = vita2d_font_text_width( gFont[ (int)(60 * FONT_SCALE) ], (int)(60 * FONT_SCALE), "Choose the difficulty");
	vita2d_font_draw_text( gFont[ (int)(60 * FONT_SCALE) ], (SCREEN_WIDTH - topText_width)/2, 110, MAIN_FONT_COLOR, (int)(60 * FONT_SCALE), "Choose the difficulty" );

	difficultyMenu.renderCursor( difficultyMenu.item[ difficultyMenu.cursor ] );	// Draw cursor

	for( int i = 0; i < difficultyMenu.MENU_ITEMS; ++i )			
		difficultyMenu.renderButton( difficultyMenu.item[ i ] );

	difficultyMenu.renderDescription();
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
	snake.handleInput();

	// Calcule player position
	snake.move();

	// Check snake collisions
	snake.checkCollision();

	// Check wall collisions
	if( snake.wallDeath() )
	{
		gSoloud.play( gSnakeDeath );
		_gameState = gameOver;
	}
	
	// Check if the player is close to the collectable
	if( collectable.checkOpenDistance( snake.get_pos() ) )
	{
		snake.isClose = true;

		// Check collectable collision with snake head
		if( collectable.checkCollision( snake.get_pos() ) )
		{
			gSoloud.play( gBite );
			collectable.collect();
			snake.addParts(1);
		}
	}
	else
	{
		snake.isClose = false;
	}
	
	// Move the snake parts
	snake.follow();

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

	int topText_width = vita2d_font_text_width( gFont[ (int)(60 * FONT_SCALE) ], (int)(60 * FONT_SCALE), "Paused" );
	vita2d_font_draw_text( gFont[ (int)(60 * FONT_SCALE) ], (SCREEN_WIDTH - topText_width)/2, 110, MAIN_FONT_COLOR, (int)(60 * FONT_SCALE), "Paused" );

	pauseMenu.renderCursor( pauseMenu.item[ pauseMenu.cursor ] );

	for( int i = 0; i < pauseMenu.MENU_ITEMS; ++i )			
		pauseMenu.renderButton( pauseMenu.item[ i ] );

	// TODO add a sleeping snake texture
	drawPlayer( body, SCREEN_WIDTH - (SCREEN_HEIGHT*0.04), SCREEN_HEIGHT - (SCREEN_HEIGHT*0.04), 2.3, 2.3, (-45 * M_PI) / 180 );
	drawPlayer( head, SCREEN_WIDTH - (SCREEN_HEIGHT*0.14), SCREEN_HEIGHT - (SCREEN_HEIGHT*0.14), 2.5, 2.5, (-45 * M_PI) / 180 );

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
	bool highscore = false;

	// Check if the player set a new highscore
	if( collectable.getScore() > collectable.getHighscore( GAME_DIFFICULTY ) )
		highscore = true;

	gameDraw();

	// Draw the Game Over text over the game
	vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA8( 0, 0, 0, 40 ) );

	int topText_width = vita2d_font_text_width( gFont[ 60 ], 60, "Game Over");
	vita2d_font_draw_text( gFont[ 60 ], (SCREEN_WIDTH - topText_width)/2, 110, MAIN_FONT_COLOR, 60, "Game Over" );

	gameOverMenu.renderCursor( gameOverMenu.item[ gameOverMenu.cursor ] );

	for( int i = 0; i < gameOverMenu.MENU_ITEMS; ++i )			
		gameOverMenu.renderButton( gameOverMenu.item[ i ] );

	// If the player set a new highscore
	if( highscore )
	{
		if( !score_read )
		{
			collectable.writeHighscore();
			collectable.readHighscore();
			score_read = true;
		}
		int text_width = vita2d_font_text_width( gFont[ (int)(35 * FONT_SCALE) ], (int)(35 * FONT_SCALE), "NEW HIGHSCORE");
		vita2d_font_draw_text( gFont[ (int)(35 * FONT_SCALE) ], ( SCREEN_WIDTH - text_width ) / 2, 270, RGBA8( 244, 205, 65, 255 ), (int)(35 * FONT_SCALE), "NEW HIGHSCORE");
	}
	
	// Render final score
	int text_width = 0;
	if( collectable.getScore() < 10 )
		text_width = vita2d_font_text_width( gFont[ (int)(35 * FONT_SCALE) ], (int)(35 * FONT_SCALE), "Your score: 0" );
	else if( collectable.getScore() < 100 )
		text_width = vita2d_font_text_width( gFont[ (int)(35 * FONT_SCALE) ], (int)(35 * FONT_SCALE), "Your score: 00" );
	else
		text_width = vita2d_font_text_width( gFont[ (int)(35 * FONT_SCALE) ], (int)(35 * FONT_SCALE), "Your score: 000" );

	vita2d_font_draw_textf( gFont[ (int)(35 * FONT_SCALE) ], (SCREEN_WIDTH-text_width)/2, 230, MAIN_FONT_COLOR, (int)(35 * FONT_SCALE), "Your score: %d", collectable.getScore() );

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

	// Free player textures
	for( int i = 0; i < NR_PLAYER_TEXTURES; ++i )
		gSnakeSheet[ i ].freeTexture();

	// Free background textures
	for( int i = 0; i < NR_BACKGROUND_TEXTURES; ++i )
	{
		if( i != 1 && i != 3 )	// 1 and 3 aren't textures
			gBgTexture[ i ].freeTexture();
	}

	// Free apple textures
	for( int i = 0; i < NR_APPLE_TEXTURES; ++i )
		gAppleTexture[ i ].freeTexture();

	gSparkleTexture.freeTexture();
		

	// Free fonts
	for( int i = 0; i <= 99; ++i )
		vita2d_free_font( gFont[ i ] );
}

// Re-initialize variables
void Game::gameReinitialize()
{
	// Reset the player and collectable
	collectable.collect();
	collectable.resetScore();
	snake.resetPos();

	// The starting length of the snake
	snake.setSize( START_SNAKE_LENGTH );

	_gameState = showingMenu;
}

// Re-initialize variables
void Game::gamePlayAgain()
{
	// Reset the player and collectable
	collectable.collect();
	collectable.resetScore();
	snake.resetPos();

	// The starting length of the snake
	snake.setSize( START_SNAKE_LENGTH );

	_gameState = choosingDifficulty;
}

// Show the "How to play" screen
void Game::gameHTP()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Press O to go back
	if( gInput.wasPressed( Input::circle ) )
	{
		gSoloud.play( gMenuSelect );
		_gameState = showingMenu;
	}
	// Touch the "Press O to go back text" to go back
	else if( gInput.wasPressed( Input::frontTouch) )
	{
		if( gInput.backTouch() )
		{
			gSoloud.play( gMenuSelect );
			_gameState = showingMenu;
		}
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	drawBackground();

	int text_width;

	text_width = vita2d_font_text_width( gFont[ (int)(35 * FONT_SCALE) ], (int)(35 * FONT_SCALE), "How to play" );
	vita2d_font_draw_text( gFont[ (int)(35 * FONT_SCALE) ], ( ( SCREEN_WIDTH - text_width ) / 2 ), 40, MAIN_FONT_COLOR, (int)(35 * FONT_SCALE), "How to play" );

	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 100, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Your goal in this game is to eat as many apples as you can," );
	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 130, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "each apple eaten adds a point to your score." );
	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 160, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Avoid biting your own tail, or you will lose the game." );


	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 260, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "- The snake moves forward by itself." );
	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 310, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "- Use the left stick, DPAD or touch to control the snake" );
	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 360, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "- You can boost the snake's speed by holding the   button." );

	text_width = vita2d_font_text_width( gFont[ (int)(20 * FONT_SCALE) ], (int)(20 * FONT_SCALE), "- You can boost the snake's speed by holding the " );
	gCrossTexture.draw_scale( text_width + 12, 365 - (gCrossTexture.get_height()*0.30), 0.30, 0.30 );

	vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], 15, 440, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "- Pause the game with the START button." );

	drawBackText();

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}

// Draw every gameplay element
void Game::gameDraw()
{
	difficultyMenu.renderBackground();

	collectable.render();			

	snake.render();

	// Draw text
	collectable.renderScore();
	collectable.renderHighscore();
}

void Game::gameOptions()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	optionsMenu.menuNav();

	optionsMenu.changeSelectable( optionsMenu.option[ optionsMenu.cursor ] );


	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	optionsMenu.renderOptions();

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();
}