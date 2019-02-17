#include "options.hpp"
#include "global.hpp"

OptionsMenu::OptionsMenu()
{
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START );
	sceTouchEnableTouchForce( SCE_TOUCH_PORT_FRONT );

	// Initialize the "control type" selectable
	option[ 0 ].id		= 0;
	option[ 0 ].name	= "Control type:";
	option[ 0 ].name_x	= 20.0;
	option[ 0 ].name_y	= 140.0f;
	option[ 0 ].slct_x	= ITEM_X;
	option[ 0 ].slct_y	= option[ 0 ].name_y;
	option[ 0 ].desc_x	= DESC_X;
	option[ 0 ].desc_y	= option[ 0 ].name_y;
	option[ 0 ].selected = 0;
	option[ 0 ].nr_selectables = 2;

	// Initialize the "player style" selectable
	option[ 1 ].id		= 1;
	option[ 1 ].name	= "Player style:";
	option[ 1 ].name_x	= option[ 0 ].name_x;
	option[ 1 ].name_y	= option[ 0 ].name_y + 70;
	option[ 1 ].slct_x	= ITEM_X;
	option[ 1 ].slct_y	= option[ 1 ].name_y;
	option[ 1 ].desc_x	= DESC_X;
	option[ 1 ].desc_y	= option[ 1 ].name_y;
	option[ 1 ].selected = 0;
	option[ 1 ].nr_selectables = snakeTextures.size();

	// Initialize the "background style" selectable
	option[ 2 ].id = 2;
	option[ 2 ].name = "BG style:";
	option[ 2 ].name_x = option[ 0 ].name_x;
	option[ 2 ].name_y = option[ 1 ].name_y + 70;
	option[ 2 ].slct_x = ITEM_X;
	option[ 2 ].slct_y = option[ 2 ].name_y;
	option[ 2 ].desc_x = DESC_X;
	option[ 2 ].desc_y = option[ 2 ].name_y;
	option[ 2 ].selected = 0;
	option[ 2 ].nr_selectables = bgTextures.size();

	// Initialize the "apple style" selectable
	option[ 3 ].id = 3;
	option[ 3 ].name = "Apple style:";
	option[ 3 ].name_x = option[ 0 ].name_x;
	option[ 3 ].name_y = option[ 2 ].name_y + 70;
	option[ 3 ].slct_x = ITEM_X;
	option[ 3 ].slct_y = option[ 3 ].name_y;
	option[ 3 ].desc_x = DESC_X;
	option[ 3 ].desc_y = option[ 3 ].name_y;
	option[ 3 ].selected = 0;
	option[ 3 ].nr_selectables = collectableTextures.size();

	// Initialize the "Collectable effects" selectable
	option[ 4 ].id = 4;
	option[ 4 ].name = "Apple effects:";
	option[ 4 ].name_x = option[ 0 ].name_x;
	option[ 4 ].name_y = option[ 3 ].name_y + 70;
	option[ 4 ].slct_x = ITEM_X;
	option[ 4 ].slct_y = option[ 4 ].name_y;
	option[ 4 ].desc_x = DESC_X;
	option[ 4 ].desc_y = option[ 4 ].name_y;
	option[ 4 ].selected = 0;
	option[ 4 ].nr_selectables = 2;

	// Initialize cursor
	cursor = 0;

	// Initialize collectable to show the selected texture
	appleModel.setPos( DESC_X, option[ 3 ].slct_y - 28 );
}

void OptionsMenu::selectUp()
{
	gSoloud.play( gMenuMove );

	if( cursor != 0 )
		cursor--;
	else
		cursor = MENU_ITEMS - 1;
}


void OptionsMenu::selectDown()
{
	gSoloud.play( gMenuMove );

	if( cursor != MENU_ITEMS - 1 )
		cursor++;
	else
		cursor = 0;
}


void OptionsMenu::renderCursor( Option option, int text_width )
{
	unsigned int cursor_color = RGBA8( 0, 0, 0, 100 );

	switch( BACKGROUND_TEXTURE )
	{
		case 0:	// Desert
			cursor_color = RGBA8( 0, 0, 0, 100 );
			break;

		case 1:	// Classic
			cursor_color = RGBA8( 255, 255, 0, 40 );
			break;

		case 2:	// RPPHS
			cursor_color = RGBA8( 0, 255, 0, 40 );
			break;

		case 3:	// Nokia
			cursor_color = RGBA8( 40, 45, 34, 40 );
			break;
	}
	
	vita2d_draw_rectangle( option.slct_x - (text_width / 2) - 20, option.slct_y - 30, text_width + 40, 40, cursor_color );
}


void OptionsMenu::menuNav()
{
	if( gInput.wasPressed( Input::up ) || gInput.wasPressed( Input::lAnalogUp ) )
	{
		selectUp();
	}
	else if( gInput.wasPressed( Input::down ) || gInput.wasPressed( Input::lAnalogDown ) )
	{
		selectDown();
	}
	// Press O to go back
	if( gInput.wasPressed( Input::circle ) )
	{
		gSoloud.play( gMenuSelect );
		writeSettings();
		_gameState = showingMenu;
	}

	// Touch 
	if( gInput.wasPressed( Input::frontTouch ) )
	{
		for( int i = 0; i < MENU_ITEMS; ++i )
		{
			if( touchSelect( option[ i ] ) )
			{
				gSoloud.play( gMenuSelect );

				cursor = i;

				if( option[ i ].selected != option[ i ].nr_selectables - 1 )
					option[ i ].selected++;
				else
					option[ i ].selected = 0;
			}
		}
		// Touch the "Press O to go back text"
		if( gInput.touchToGoBack() )
		{
			gSoloud.play( gMenuSelect );
			writeSettings();
			_gameState = showingMenu;
		}
	}
}


void OptionsMenu::changeSelectable( Option & option )
{
	if( gInput.wasPressed( Input::left ) || gInput.wasPressed( Input::lAnalogLeft ) )
	{
		gSoloud.play( gMenuMove );

		if( option.selected != 0 )
			option.selected--;
		else
			option.selected = option.nr_selectables - 1;
	}
	if( gInput.wasPressed( Input::right ) || gInput.wasPressed( Input::lAnalogRight ) || gInput.wasPressed( Input::cross ) )
	{
		gSoloud.play( gMenuMove );

		if( option.selected != option.nr_selectables - 1 )
			option.selected++;
		else
			option.selected = 0;
	}

	// Set global variables
	if( option.id == 0 )
	{
		CONTROL_STYLE = option.selected;
	}
	else if( option.id == 1 )
	{
		PLAYER_TEXTURES = option.selected;
	}
	else if( option.id == 2 )
	{
		BACKGROUND_TEXTURE = option.selected;

		// Set font color based on background style
		switch( option.selected )
		{
			case 0:	// Desert
				MAIN_FONT_COLOR = RGBA8( 0, 0, 0, 255 );
				break;

			case 1:	// Classic
				MAIN_FONT_COLOR = RGBA8( 255, 255, 0, 255 );
				break;

			case 2:	// RPPHS
				MAIN_FONT_COLOR = RGBA8( 0, 255, 0, 255 );
				break;

			case 3:	// Nokia
				MAIN_FONT_COLOR = RGBA8( 40, 45, 34, 255 );
				break;
		}
	}
	else if( option.id == 3 )
	{
		APPLE_TEXTURE = option.selected;
	}
	// Collectable effects option
	else if( option.id == 4 )
	{
		if( option.selected == 0 )
		{
			ENABLE_PARTICLES = true;
			ENABLE_ANIMATION = true;
		}
		else
		{
			ENABLE_PARTICLES = false;
			ENABLE_ANIMATION = false;
		}
	}
}


bool OptionsMenu::touchSelect( Option option )
{
	int x = gInput.getTouchX();
	int y = gInput.getTouchY();

	if( (x > 0) && (x < 550) &&
		(y > option.slct_y-30) && (y < option.slct_y + 20)
		)
		return true;
	else
		return false;
}


void OptionsMenu::renderOptions()
{
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START );
	sceTouchEnableTouchForce( SCE_TOUCH_PORT_FRONT );

	drawBackground();
	updateNrTextures();

	int text_width;

	text_width = vita2d_font_text_width( gFont[ 60 ], 60, "Options" );
	vita2d_font_draw_text ( gFont[ (int)(60 * FONT_SCALE) ], (SCREEN_WIDTH - text_width) / 2, 70, MAIN_FONT_COLOR, (int)(60 * FONT_SCALE), "Options" );

	// Control Type option
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ 0 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ 0 ].name );
	switch( option[ 0 ].selected )
	{
		case 0:
			text_width = drawSelectable( "Classic", option[ 0 ].slct_y );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, option[ 0 ].slct_y - 20, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Move the left stick left or" );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, option[ 0 ].slct_y + 10, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "right to turn the snake." );
			break;
		case 1:
			text_width = drawSelectable( "Simplified", option[ 0 ].slct_y );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, option[ 0 ].slct_y - 20, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Move the left in the direction" );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, option[ 0 ].slct_y + 10, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "you want the snake to go." );
			break;
	}
	if( cursor == 0 )
		renderCursor( option[ cursor ], text_width );

	// player/background/collectable style options
	for( int i = 1; i < MENU_ITEMS-1; ++i )
	{
		vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ i ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ i ].name );
		switch( i )
		{
			case 1:
				text_width = drawSelectable( snakeTextures[ option[ i ].selected ].name, option[ i ].slct_y );
				break;
			case 2:
				text_width = drawSelectable( bgTextures[ option[ i ].selected ].name, option[ i ].slct_y );
				break;
			case 3:
				text_width = drawSelectable( collectableTextures[ option[ i ].selected ].name, option[ i ].slct_y );
				break;
		}
		if( cursor == i )
			renderCursor( option[ cursor ], text_width );
	}
	drawPlayerModel();
	appleModel.render();

	// Collectable effects selectable
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ 4 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ 4 ].name );
	switch( option[ 4 ].selected )
	{
		case 0:
			text_width = drawSelectable( "On", option[ 4 ].slct_y );
			break;
		case 1:
			text_width = drawSelectable( "Off", option[ 4 ].slct_y );
			break;
	}
	if( cursor == 4 )
		renderCursor( option[ cursor ], text_width );

	drawBackText();
}

void OptionsMenu::writeSettings()
{
	std::ofstream settings;
	sceIoMkdir("ux0:/data/vitaSnake", 0777);
	settings.open( "ux0:/data/vitaSnake/settings.txt" );

	settings << CONTROL_STYLE << "\n";
	settings << PLAYER_TEXTURES << "\n";
	settings << BACKGROUND_TEXTURE << "\n";
	settings << APPLE_TEXTURE << "\n";
	settings << MAIN_FONT_COLOR << "\n";
	settings << ENABLE_PARTICLES << "\n";
	settings << ENABLE_ANIMATION;

	settings.close();
}

void OptionsMenu::readSettings()
{
	std::ifstream settings;
	settings.open( "ux0:/data/vitaSnake/settings.txt", std::ifstream::in );
		
	settings >> CONTROL_STYLE;
	settings >> PLAYER_TEXTURES;
	settings >> BACKGROUND_TEXTURE;
	settings >> APPLE_TEXTURE;
	settings >> MAIN_FONT_COLOR;
	settings >> ENABLE_PARTICLES;
	settings >> ENABLE_ANIMATION;

	option[ 0 ].selected = CONTROL_STYLE;
	option[ 1 ].selected = PLAYER_TEXTURES;
	option[ 2 ].selected = BACKGROUND_TEXTURE;
	option[ 3 ].selected = APPLE_TEXTURE;

	if( ENABLE_PARTICLES && ENABLE_ANIMATION )
		option[ 4 ].selected = 0;
	else if( !ENABLE_PARTICLES && !ENABLE_ANIMATION )
		option[ 4 ].selected = 1;

	settings.close();
}


int OptionsMenu::drawSelectable( const char *name, int y )
{
	int text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), name );
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), name );

	return text_width;
}

void OptionsMenu::drawPlayerModel()
{
	for( int i = 0; i < 3; ++i )
		drawPlayer( (part)i, DESC_X + (i * 30) + 15, option[ 1 ].slct_y - 15, M_PI / 2 );
}

void OptionsMenu::updateNrTextures()
{
	if( option[ 1 ].nr_selectables == 0 || option[ 2 ].nr_selectables == 0 || option[ 3 ].nr_selectables == 0 )
	{
		option[ 1 ].nr_selectables = snakeTextures.size();
		option[ 2 ].nr_selectables = bgTextures.size();
		option[ 3 ].nr_selectables = collectableTextures.size();
	}
}
