#include "menu.hpp"

void Menu::selectUp()
{
	gSoloud.play( gMenuMove );

	if( cursor != 0 )
		cursor--;
	else
		cursor = MENU_ITEMS-1;
}


void Menu::selectDown()
{
	gSoloud.play( gMenuMove );

	if( cursor != MENU_ITEMS-1 )
		cursor++;
	else
		cursor = 0;
}


void Menu::menuNav()
{
	if( gInput.wasPressed( Input::up ) || gInput.wasPressed( Input::lAnalogUp ) )
	{
		selectUp();
	}
	if( gInput.wasPressed( Input::down ) || gInput.wasPressed( Input::lAnalogDown ) )
	{
		selectDown();
	}
}

bool Menu::selectItem()
{
	return gInput.wasPressed( Input::cross );
}


bool Menu::touchSelect( MenuItem item )
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( 0, &touch[0], 1 );

	int x = touch[0].report[0].x / 2;
	int y = touch[0].report[0].y / 2;

	if( ( x > item.x ) && ( x < item.x + BUTTON_WIDTH ) &&
		( y > item.y ) && ( y < item.y + BUTTON_HEIGHT )
		)
	{
		return true;
	}	
		
	return false;
}


void Menu::renderCursor( MenuItem item )
{
	vita2d_draw_rectangle( item.x, item.y, BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 30, 30, 30, cursorShadow ) );

	int border_thickness = 4;

	vita2d_draw_rectangle( item.x, item.y, BUTTON_WIDTH, border_thickness, MAIN_FONT_COLOR );
	vita2d_draw_rectangle( item.x + BUTTON_WIDTH - border_thickness, item.y, border_thickness, BUTTON_HEIGHT, MAIN_FONT_COLOR );
	vita2d_draw_rectangle( item.x, item.y + BUTTON_HEIGHT - border_thickness, BUTTON_WIDTH, border_thickness, MAIN_FONT_COLOR );
	vita2d_draw_rectangle( item.x, item.y, border_thickness, BUTTON_HEIGHT, MAIN_FONT_COLOR );
}

void Menu::renderCursor( int x, int y, int w, int h )
{
	vita2d_draw_rectangle( x, y, w, h, RGBA8( 30, 30, 30, cursorShadow ) );
}


void Menu::renderButton( MenuItem item )
{
	int textSize = 42;
	drawText_position( centered, item.x + (BUTTON_WIDTH/2), item.y + (BUTTON_HEIGHT/2), textSize, item.name );
}


// Main menu initialization
MainMenu::MainMenu()
{
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);

	// Nr of selectable menu items
	MENU_ITEMS = 4;

	// Initialize menu items
	item[0].name = "Start Game";
	item[0].x = 310.0f;
	item[0].y = 170.0f;

	item[1].name = "Extras";
	item[1].x = 310.0f;
	item[1].y = 260.0f;

	item[2].name = "Options";
	item[2].x = 310.0f;
	item[2].y = 350.0f;

	item[3].name = "Exit";
	item[3].x = 310.0f;
	item[3].y = 440.0f;

	// Initialize cursor
	cursor = startGame;
}

void MainMenu::readSplashFile()
{
	std::ifstream splashFile;
	splashFile.open( "app0:/font/splash.txt", std::ifstream::in );
		
	std::string temp;
	
	while( !splashFile.eof() )
	{
		getline( splashFile, temp );
		splash.push_back( temp );
	}

	splashFile.close();
}

void MainMenu::randomizeSplash()
{
	randSplash = rand() % splash.size();
}

void MainMenu::renderBackground()
{
	drawBackground();
}

void MainMenu::drawSplashText()
{
	int logoPosX = 95;
	int logoSize = 60;

	// Draw logo
	drawText_position( centeredX, (SCREEN_WIDTH/2), logoPosX, logoSize, "vitaSnake" );

	int font_size = 23;
	drawText_position( centeredX, SCREEN_WIDTH/2 + 8, logoPosX + 28, font_size, splash[randSplash].c_str() );
}


PauseMenu::PauseMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = "Resume game";
	item[0].x = 310.0f;
	item[0].y = 310.0f;

	item[1].name = "Main menu";
	item[1].x = 310.0f;
	item[1].y = 411.0f;

	// Initialize cursor
	cursor = resumeGame;
}

void PauseMenu::renderBackground()
{
	drawBackground();
}


GameOverMenu::GameOverMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = "Play Again";
	item[0].x = 310.0f - 200;
	item[0].y = 400.0f;

	item[1].name = "Main menu";
	item[1].x = 310.0f + 200;;
	item[1].y = item[0].y;

	// Initialize cursor
	cursor = playAgain;
}

void GameOverMenu::drawMenu()
{
	// Dim the background with a cutout for the selected button
	vita2d_draw_rectangle( 0, 0, SCREEN_WIDTH, item[0].y, RGBA8( 0, 0, 0, 100 ) );
	vita2d_draw_rectangle( 0, item[0].y + BUTTON_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - item[0].y - BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );
	vita2d_draw_rectangle( 0, item[0].y, item[0].x, BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );
	vita2d_draw_rectangle( item[0].x + BUTTON_WIDTH, item[0].y, item[1].x - item[0].x - BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );
	vita2d_draw_rectangle( item[1].x + BUTTON_WIDTH, item[0].y, SCREEN_WIDTH - item[1].x - BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );
	if( cursor == 0 ) vita2d_draw_rectangle( item[1].x, item[1].y, BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );
	else vita2d_draw_rectangle( item[0].x, item[0].y, BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 0, 0, 0, 100 ) );

	// Draw the game over banner
	gameOverBanner();

	renderCursor( item[ cursor ] );

	for( int i = 0; i < MENU_ITEMS; ++i )			
		renderButton( item[ i ] );

	// Animate the fade in effect
	if( fadeInValue < 255)
		fadeInValue += fadeInSpeed * timestep;
	
	if( fadeInValue > 255 )
		fadeInValue = 255;
}

void GameOverMenu::gameOverBanner()
{
	int bannerH = 240;
	int gameOverTextSize = 83;
	int borderThickness = 2;

	vita2d_draw_rectangle( 0, bannerY, SCREEN_WIDTH, bannerH, RGBA8( 0, 0, 0, (int)fadeInValue ) );

	// Draw border with color based on theme
	vita2d_draw_rectangle( 0, bannerY, SCREEN_WIDTH, borderThickness, MAIN_FONT_COLOR );
	vita2d_draw_rectangle( 0, bannerY+bannerH-borderThickness, SCREEN_WIDTH, borderThickness, MAIN_FONT_COLOR );

	drawText_color_position( centeredX, SCREEN_WIDTH/2, gameOverTextY, gameOverTextSize, RGBA8(255, 255, 255, (int)fadeInValue ), "Game Over" );
}

void GameOverMenu::drawFinalScore( int score )
{
	int textSize = 40;
	
	drawTextf_color_position( centeredX, SCREEN_WIDTH/2, scoreTextY, textSize, RGBA8(255, 255, 255, (int)fadeInValue ), "Your score: %d", score );
}

void GameOverMenu::drawNewHighscore()
{
	int textSize = 30;

	drawText_color_position( centeredX, SCREEN_WIDTH / 2, newHighscoreY, textSize, RGBA8( 255, 215, 0, (int)fadeInValue ), "NEW HIGHSCORE!" );
}

void GameOverMenu::menuNav()
{
	if( gInput.wasPressed( Input::left ) || gInput.wasPressed( Input::left ) )
	{
		selectUp();
	}
	if( gInput.wasPressed( Input::right ) || gInput.wasPressed( Input::lAnalogRight ) )
	{
		selectDown();
	}
}

void GameOverMenu::renderCursor( MenuItem item )
{
	vita2d_draw_rectangle( item.x, item.y, BUTTON_WIDTH, BUTTON_HEIGHT, RGBA8( 30, 30, 30, ((int)fadeInValue*cursorShadow)/255 ) );

	int border_thickness = 4;

	unsigned int textColor = MAIN_FONT_COLOR;
	textColor <<= 8;
	textColor >>= 8;
	unsigned int fade = (unsigned int)fadeInValue << 24;
	textColor += fade;

	vita2d_draw_rectangle( item.x, item.y, BUTTON_WIDTH, border_thickness, textColor );
	vita2d_draw_rectangle( item.x + BUTTON_WIDTH - border_thickness, item.y, border_thickness, BUTTON_HEIGHT, textColor );
	vita2d_draw_rectangle( item.x, item.y + BUTTON_HEIGHT - border_thickness, BUTTON_WIDTH, border_thickness, textColor );
	vita2d_draw_rectangle( item.x, item.y, border_thickness, BUTTON_HEIGHT, textColor );
}

void GameOverMenu::renderButton( MenuItem item )
{
	int fontSize = 42;

	unsigned int textColor = MAIN_FONT_COLOR;
	textColor <<= 8;
	textColor >>= 8;
	unsigned int fade = (unsigned int)fadeInValue << 24;
	textColor += fade;

	drawText_position( centered, item.x + (BUTTON_WIDTH/2), item.y + (BUTTON_HEIGHT/2), fontSize, item.name );
}

void GameOverMenu::resetFadeIn()
{
	fadeInValue = 0;
}


DifficultyMenu::DifficultyMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = "Classic";
	item[0].x = 310.0f;
	item[0].y = 207.0f;

	item[1].name = "Hardcore";
	item[1].x = 310.0f;
	item[1].y = 310.0f;

	// Initialize cursor
	cursor = classic;
}

void DifficultyMenu::drawMenu()
{
	drawBackground();

	int font_size = 60;
	drawText_position( centeredX, SCREEN_WIDTH/2, 110, font_size, "Choose the difficulty" );

	font_size = 35;
	drawTextf_position( centeredX, SCREEN_WIDTH/2, 160, font_size, "Mode: %s", getGameModeName() );


	renderCursor( item[ cursor ] );	// Draw cursor

	for( int i = 0; i < MENU_ITEMS; ++i )			
		renderButton( item[ i ] );

	renderDescription();
	renderSnake();
}

void DifficultyMenu::renderSnake()
{
	switch( GAME_DIFFICULTY )
	{
		case 0:
			drawPlayer( body, SCREEN_WIDTH - ( SCREEN_HEIGHT*0.04 ), SCREEN_HEIGHT - ( SCREEN_HEIGHT*0.04 ), 2.3, 2.3, ( -45 * M_PI ) / 180 );
			drawPlayer( head, SCREEN_WIDTH - ( SCREEN_HEIGHT*0.14 ), SCREEN_HEIGHT - ( SCREEN_HEIGHT*0.14 ), 2.5, 2.5, ( -45 * M_PI ) / 180 );
			break;
		case 1:
			drawPlayer( body, SCREEN_WIDTH - (SCREEN_HEIGHT*0.04), SCREEN_HEIGHT - (SCREEN_HEIGHT*0.04), 2.3, 2.3, (-45 * M_PI) / 180 );
			drawPlayer( headAngry, SCREEN_WIDTH - (SCREEN_HEIGHT*0.14), SCREEN_HEIGHT - (SCREEN_HEIGHT*0.14), 2.5, 2.5, (-45 * M_PI) / 180 );
			break;
		default:
			drawPlayer( body, SCREEN_WIDTH - ( SCREEN_HEIGHT*0.04 ), SCREEN_HEIGHT - ( SCREEN_HEIGHT*0.04 ), 2.3, 2.3, ( -45 * M_PI ) / 180 );
			drawPlayer( head, SCREEN_WIDTH - ( SCREEN_HEIGHT*0.14 ), SCREEN_HEIGHT - ( SCREEN_HEIGHT*0.14 ), 2.5, 2.5, ( -45 * M_PI ) / 180 );
			break;
	}
}

void DifficultyMenu::renderDescription()
{
	int text_width1, text_width2;
	int font_size = 30;

	switch( GAME_DIFFICULTY )
	{
	case 0:
		drawText_position( centeredX, SCREEN_WIDTH/2, 430, font_size, "Collect as many apples as you can," );
		drawText_position( centeredX, SCREEN_WIDTH/2, 470, font_size, "just don't bite your tail!" );
		break;
	case 1:
		drawText_position( centeredX, SCREEN_WIDTH/2, 430, font_size, "Recommended for experienced players," );
		drawText_position( centeredX, SCREEN_WIDTH/2, 470, font_size, "touching the game border kills the snake." );
		break;
	}
}


ModeMenu::ModeMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 5;

	// Initialize menu items
	item[0].name = "Classic";
	item[0].x = margin_x;
	item[0].y = margin_y;

	item[1].name = "Time Trial";
	item[1].x = margin_x;
	item[1].y = margin_y + BUTTON_HEIGHT + padding;

	item[2].name = "Hyper Snake";
	item[2].x = margin_x;
	item[2].y = margin_y + 2*BUTTON_HEIGHT + 2*padding;

	item[3].name = "Fibonacci";
	item[3].x = margin_x;
	item[3].y = margin_y + 3*BUTTON_HEIGHT + 3*padding;

	item[4].name = "Lazy Snake";
	item[4].x = margin_x;
	item[4].y = SCREEN_HEIGHT - margin_y - BUTTON_HEIGHT;

	// Initialize cursor
	cursor = classic;
}

void ModeMenu::renderMenu()
{
	drawBackground();

	// Draw a shadow where the buttons are
	vita2d_draw_rectangle( 10, 10, margin_x+BUTTON_WIDTH+margin_x-20, SCREEN_HEIGHT-20, RGBA8( 0, 0, 0, 50 ) );

	int text_size = 40;
	drawText_position( alignRight, SCREEN_WIDTH - margin_x, 100, text_size , "Choose a game mode" );

	renderCursor( item[ cursor ] );	// Draw cursor

	for( int i = 0; i < MENU_ITEMS; ++i )			
		renderButton( item[ i ] );

	renderDescription();

	drawBackText();
}

void ModeMenu::renderDescription()
{
	int font_size = 30;

	int desc_x = 500;
	int desc_y = 180;

	// Draw a "shadow box"
	vita2d_draw_rectangle( desc_x-20, desc_y-40, 454, 270, RGBA8( 0, 0, 0, 50 ) );

	switch( GAME_MODE )
	{
	// Classic
	case 0:
		drawText( desc_x, desc_y, font_size, "Play vitaSnake the" );
		drawText( desc_x, desc_y + 30, font_size, "classic way!" );
		break;
	// Time Trial
	case 1:
		drawText( desc_x, desc_y, font_size, "It's a desparate race" );
		drawText( desc_x, desc_y + 30, font_size, "against the time," );
		drawText( desc_x, desc_y + 60, font_size, "collect as many points" );
		drawText( desc_x, desc_y + 90, font_size, "as possible within a 30" );
		drawText( desc_x, desc_y + 120, font_size, "second time limit!" );
		break;
	// Hyper
	case 2:
		drawText( desc_x, desc_y, font_size, "The snake starts slow," );
		drawText( desc_x, desc_y + 30, font_size, "but gets faster with." );
		drawText( desc_x, desc_y + 60, font_size, "each point." );
		break;
	// Fibonacci
	case 3:
		drawText( desc_x, desc_y, font_size, "Instead of only growing");
		drawText( desc_x, desc_y + 30, font_size, "by one piece, each");
		drawText( desc_x, desc_y + 60, font_size, "time the snake gets");
		drawText( desc_x, desc_y + 90, font_size, "longer by a number");
		drawText( desc_x, desc_y + 120, font_size, "determined by the");
		drawText(  desc_x, desc_y + 150, font_size, "Fibonacci sequence.");
		break;
	// Lazy
	case 4:
		drawText( desc_x, desc_y, font_size, "The snake got lazy,  ");
		drawText( desc_x, desc_y + 30, font_size, "so it won't move");
		drawText( desc_x, desc_y + 60, font_size, "on it's own.");
		drawText( desc_x, desc_y + 90, font_size, "Drag it by the head");
		drawText( desc_x, desc_y + 120, font_size, "with your finger.");
		break;
	}
}

ExtrasMenu::ExtrasMenu()
{
	// Nr of selectable menu items
	MENU_ITEMS = 2;

	// Initialize menu items
	item[0].name = "How to play";
	item[0].x = margin_x;
	item[0].y = margin_y;

	item[1].name = "Credits";
	item[1].x = margin_x;
	item[1].y = item[0].y + BUTTON_HEIGHT + padding;

	// Initialize cursor
	cursor = howToPlay;
}

void ExtrasMenu::renderMenu()
{
	renderCursor( item[ cursor ] );	// Draw cursor

	for( int i = 0; i < MENU_ITEMS; ++i )			
		renderButton( item[ i ] );
}