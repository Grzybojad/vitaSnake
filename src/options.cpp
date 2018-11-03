#include "options.hpp"
#include "global.hpp"

OptionsMenu::OptionsMenu()
{
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START );
	sceTouchEnableTouchForce( SCE_TOUCH_PORT_FRONT );

	// Nr of selectable menu items
	MENU_ITEMS = 3;

	// Initialize the "control type" selectable
	option[ 0 ].id		= 0;
	option[ 0 ].name	= "Control type:";
	option[ 0 ].name_x	= 20.0;
	option[ 0 ].name_y	= 200.0f;
	option[ 0 ].slct_x	= ITEM_X;
	option[ 0 ].slct_y	= option[ 0 ].name_y;
	option[ 0 ].desc_x	= DESC_X;
	option[ 0 ].desc_y	= option[ 0 ].name_y;
	option[ 0 ].selected = 0;
	option[ 0 ].nr_selectables = 2;

	// Initialize the "player style" selectable
	option[ 1 ].id		= 1;
	option[ 1 ].name	= "Player style:";
	option[ 1 ].name_x	= 20.0;
	option[ 1 ].name_y	= 270.0f;
	option[ 1 ].slct_x	= ITEM_X;
	option[ 1 ].slct_y	= option[ 1 ].name_y;
	option[ 1 ].desc_x	= DESC_X;
	option[ 1 ].desc_y	= option[ 1 ].name_y;
	option[ 1 ].selected = 0;
	option[ 1 ].nr_selectables = NR_PLAYER_TEXTURES;

	// Initialize the "background style" selectable
	option[ 2 ].id = 2;
	option[ 2 ].name = "BG style:";
	option[ 2 ].name_x = 20.0;
	option[ 2 ].name_y = 340.0f;
	option[ 2 ].slct_x = ITEM_X;
	option[ 2 ].slct_y = option[ 2 ].name_y;
	option[ 2 ].desc_x = DESC_X;
	option[ 2 ].desc_y = option[ 2 ].name_y;
	option[ 2 ].selected = 0;
	option[ 2 ].nr_selectables = NR_BACKGROUND_TEXTURES;

	// Initialize cursor
	cursor = 0;
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
	
	vita2d_draw_rectangle( option.slct_x - (text_width / 2) - 20, option.slct_y - 35, text_width + 40, 40, cursor_color );
}


void OptionsMenu::menuNav()
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

// Doesn't work yet
bool OptionsMenu::touchSelect( Option option )
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( 0, &touch[ 0 ], 1 );

	int x = touch[ 0 ].report[ 0 ].x / 2;
	int y = touch[ 0 ].report[ 0 ].y / 2;

	if( ( x > option.slct_x ) && ( x < option.slct_x + 200 ) &&
		( y > option.slct_y ) && ( y < option.slct_y + 40 )
		)
	{
		return true;
	}

	return false;
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
	if( gInput.wasPressed( Input::right ) || gInput.wasPressed( Input::lAnalogRight ) || touchSelect( option ) )
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
}


void OptionsMenu::renderOptions()
{
	drawBackground();

	int text_width;

	text_width = vita2d_font_text_width( gFont[ 60 ], 60, "Options" );
	vita2d_font_draw_text ( gFont[ (int)(60 * FONT_SCALE) ], (SCREEN_WIDTH - text_width) / 2, 100, MAIN_FONT_COLOR, (int)(60 * FONT_SCALE), "Options" );

	// Control Type option
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ 0 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ 0 ].name );
	switch( option[ 0 ].selected )
	{
		case 0:
			text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< classic >" );
			vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 0 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< classic >" );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, 180, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Move the left stick left or right" );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, 210, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "to turn the snake." );
			break;
		case 1:
			text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< simplified >" );
			vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 0 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< simplified >" );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, 180, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "Move the left in the direction you " );
			vita2d_font_draw_text( gFont[ (int)(20 * FONT_SCALE) ], DESC_X, 210, MAIN_FONT_COLOR, (int)(20 * FONT_SCALE), "want the snake to go." );
			break;
	}
	if( cursor == 0 )
		renderCursor( option[ cursor ], text_width );

	// Player style option
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ 1 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ 1 ].name );
	switch( option[ 1 ].selected )
	{
		case 0:
			text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< Brown >" );
			vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 1 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< Brown >" );
			for( int i = 0; i < 3; ++i )
				drawPlayer( (part)i, DESC_X + (i * 30) + 15, option[ 1 ].slct_y - 15, M_PI / 2 );
			break;
		case 1:
			text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< Classic >" );
			vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 1 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< Classic >" );
			for( int i = 0; i < 3; ++i )
				drawPlayer( (part)i, DESC_X + (i * 30) + 15, option[ 1 ].slct_y - 15, M_PI / 2 );
			break;
		case 2:
			text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< RPPHS >" );
			vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 1 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< RPPHS >" );
			for( int i = 0; i < 3; ++i )
				drawPlayer( (part)i, DESC_X + (i * 30) + 15, option[ 1 ].slct_y - 15, M_PI / 2 );
			break;
	}
	if( cursor == 1 )
		renderCursor( option[ cursor ], text_width );

	// Background style option
	vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], 20, option[ 2 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), option[ 2 ].name );
	switch( option[ 2 ].selected )
	{
	case 0:
		text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< Desert >" );
		vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 2 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< Desert >" );
		break;
	case 1:
		text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< Classic >" );
		vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 2 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< Classic >" );
		break;
	case 2:
		text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< RPPHS >" );
		vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 2 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< RPPHS >" );
		break;
	case 3:
		text_width = vita2d_font_text_width( gFont[ (int)(30 * FONT_SCALE) ], (int)(30 * FONT_SCALE), "< Nokia >" );
		vita2d_font_draw_text( gFont[ (int)(30 * FONT_SCALE) ], ITEM_X - (text_width / 2), option[ 2 ].slct_y, MAIN_FONT_COLOR, (int)(30 * FONT_SCALE), "< Nokia >" );
		break;
	}
	if( cursor == 2 )
		renderCursor( option[ cursor ], text_width );


	drawBackText();
}