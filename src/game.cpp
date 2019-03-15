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
	vita2d_init_advanced_with_msaa( (1 * 1024 * 1024), SCE_GXM_MULTISAMPLE_4X );
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
	collectable.checkAndFixHighscores();

	// Read settings
	optionsMenu.readSettings();

	// Read stats
	stats.readStats();

	// Load splash text file
	mainMenu.readSplashFile();
	mainMenu.randomizeSplash();

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
			case choosingMode:
				gameMode();
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
			_gameState = choosingMode;
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

	// Draw splash
	mainMenu.drawSplashText();

	// Draw version in the lower-left corner
	int text_height = vita2d_font_text_height( gFont[ (int)(24 * FONT_SCALE) ], (int)(24 * FONT_SCALE), "version: " );
	vita2d_font_draw_textf( gFont[ (int)(24 * FONT_SCALE) ], 10, SCREEN_HEIGHT-(text_height/2)-5, MAIN_FONT_COLOR, (int)(24 * FONT_SCALE), "version: %s", VERSION );

	mainMenu.renderCursor( mainMenu.item[ mainMenu.cursor ] );	

	for( int i = 0; i < mainMenu.MENU_ITEMS; ++i )
		mainMenu.renderButton( mainMenu.item[ i ] );

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();

	calcFrameTime();
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

		timer.start();

		if( gameTime.is_started() )
			gameTime.unpause();
		else
			gameTime.start();

		snake.setDifficulty();

		_gameState = playing;
	}
	if( gInput.wasPressed(Input::circle) )
	{
		gSoloud.play( gMenuSelect );
		_gameState = choosingMode;
	}


	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();								

	drawBackground();	// Draw menu background

	difficultyMenu.drawMenu();

	collectable.renderMenuScores();	// Render highscores in the menu

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();

	calcFrameTime();
}

// Main game loop
void Game::gameLoop()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Pause the game with START
	if( gInput.wasPressed(Input::start) )
	{
		timer.pause();

		gameTime.pause();

		_gameState = paused;
	}
		

	// Calcule player position
	if( GAME_MODE != ModeMenu::lazy )
	{
		snake.handleInput();
		snake.move();
	}
	else
	{
		if( gInput.isTouched() )
		{
			if( snake.isDragged )
			{
				snake.handleDrag();
			}
			else if( snake.touchingHead() )
			{
				snake.isDragged = true;
				snake.handleDrag();
			}
			else
			{
				snake.isDragged = false;
			}
		}
		else
		{
			snake.isDragged = false;
		}
	}

	// Check wall, snake collisions and time to determine if the snake should die
	if( snake.wallDeath() || 
		snake.checkCollision() || 
		( GAME_MODE == ModeMenu::timeTrial && timer.get_ticks() >= TIME_LIMIT )
	)
	{
		gSoloud.play( gSnakeDeath );
		timer.pause();
		gameTime.pause();
		stats.totalDeaths += 1;
		stats.timePlayed += gameTime.get_ticks();
		gameTime.start();
		stats.saveStats();
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

			snake.addParts(collectable.collect( snake.get_pos() ));
			stats.applesEaten += 1;

			// Collecting points increaces player speed in "hyper" mode
			if( GAME_MODE == ModeMenu::hyper )
				snake.speedUp();
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

	calcFrameTime();
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
		{
			timer.unpause();
			gameTime.unpause();
			_gameState = playing;
		}
		else if( pauseMenu.cursor == PauseMenu::returnToMenu )
			_gameState = needReinitialize;
	}
	if( gInput.wasPressed(Input::start) )
	{
		timer.unpause();
		gameTime.unpause();
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

	calcFrameTime();
}

// Game Over screen
void Game::gameEnd()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	gameDraw();

	// Draw the Game Over text over the game
	vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA8( 0, 0, 0, 40 ) );

	int topText_width = vita2d_font_text_width( gFont[ 60 ], 60, "Game Over");
	vita2d_font_draw_text( gFont[ 60 ], (SCREEN_WIDTH - topText_width)/2, 110, MAIN_FONT_COLOR, 60, "Game Over" );

	gameOverMenu.renderCursor( gameOverMenu.item[ gameOverMenu.cursor ] );

	for( int i = 0; i < gameOverMenu.MENU_ITEMS; ++i )			
		gameOverMenu.renderButton( gameOverMenu.item[ i ] );

	// If the player set a new highscore
	if( collectable.getScore() >= collectable.getHighscore() )
	{
		if( !score_read )
		{
			collectable.writeNewHighscore();
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

	calcFrameTime();
}

// Destroy textures on game exit
void Game::gameQuit()
{
	vita2d_fini();

	stats.timePlayed += gameTime.get_ticks();
	stats.saveStats();

	// Free player textures
	for( int i = 0; i < snakeTextures.size(); ++i )
		snakeTextures[ i ].freeTexture();

	// Free background textures
	for( int i = 0; i < bgTextures.size(); ++i )
	{
		bgTextures[ i ].freeTexture();
	}

	// Free apple textures
	for( int i = 0; i < collectableTextures.size(); ++i )
		collectableTextures[ i ].freeTexture();

	gSparkleTexture.freeTexture();
		

	// Free fonts
	for( int i = 0; i <= 99; ++i )
		vita2d_free_font( gFont[ i ] );
}

// Re-initialize variables
void Game::gameReinitialize()
{
	// Reset the player and collectable
	collectable.reset();
	collectable.resetScore();
	snake.reset();

	// The starting length of the snake
	snake.setSize( START_SNAKE_LENGTH );

	_gameState = showingMenu;
	mainMenu.randomizeSplash();
}

// Re-initialize variables
void Game::gamePlayAgain()
{
	// Reset the player and collectable
	collectable.reset();
	collectable.resetScore();
	snake.reset();

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
		mainMenu.randomizeSplash();
		_gameState = showingMenu;
	}
	// Touch the "Press O to go back text" to go back
	else if( gInput.wasPressed( Input::frontTouch) )
	{
		if( gInput.touchToGoBack() )
		{
			gSoloud.play( gMenuSelect );
			mainMenu.randomizeSplash();
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

	calcFrameTime();
}


// Draw every gameplay element
void Game::gameDraw()
{
	drawBackground();

	collectable.render();			

	snake.render();

	if( GAME_MODE == ModeMenu::timeTrial )
		timer.drawCountdown( TIME_LIMIT );

	// Draw text
	collectable.renderScore();
	collectable.renderHighscore();

	// Draw back touch crosshair
	if( gInput.isBackTouched() && ENABLE_BACKTOUCH )
	{
		int x = gInput.getBackTouchX();
		int y = gInput.getBackTouchY();

		int size = 16;
		int weight = 2;

		// Vertical line
		vita2d_draw_rectangle( x-(weight/2), y-(size/2), weight, size, MAIN_FONT_COLOR );

		// Horizontal line
		vita2d_draw_rectangle( x-(size/2), y-(weight/2), size, weight, MAIN_FONT_COLOR );
	}
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

	calcFrameTime();
}

void Game::gameMode()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	GAME_DIFFICULTY = 0;

	// Menu controls
	modeMenu.menuNav();

	bool select = false;

	if( modeMenu.selectItem() )
		select = true;

	if( gInput.wasPressed(Input::frontTouch) )
	{
		for( int i = 0; i < modeMenu.MENU_ITEMS; ++i )
		{
			if( modeMenu.touchSelect(modeMenu.item[i]) )
			{
				modeMenu.cursor = i;
				select = true;
			}
		}
		// Touch the "Press O to go back text" to go back
		if( gInput.touchToGoBack() )
		{
			gSoloud.play( gMenuSelect );
			mainMenu.randomizeSplash();
			_gameState = showingMenu;
		}
	}
	
	if( modeMenu.cursor == ModeMenu::classic )
		GAME_MODE = ModeMenu::classic;
	else if( modeMenu.cursor == ModeMenu::timeTrial )
		GAME_MODE = ModeMenu::timeTrial;
	else if( modeMenu.cursor == ModeMenu::hyper )
		GAME_MODE = ModeMenu::hyper;
	else if( modeMenu.cursor == ModeMenu::fibonacci )
		GAME_MODE = ModeMenu::fibonacci;
	else if( modeMenu.cursor == ModeMenu::lazy )
		GAME_MODE = ModeMenu::lazy;

	if( select )
	{
		gSoloud.play( gMenuSelect );
		_gameState = choosingDifficulty;
	}
	
	if( gInput.wasPressed(Input::circle) )
	{
		gSoloud.play( gMenuSelect );
		mainMenu.randomizeSplash();
		_gameState = showingMenu;
	}

	/* RENDERING */
	vita2d_start_drawing();
	vita2d_clear_screen();	

	modeMenu.renderMenu();

	vita2d_end_drawing();
	vita2d_wait_rendering_done();
	vita2d_swap_buffers();

	calcFrameTime();
}