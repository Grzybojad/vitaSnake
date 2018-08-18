#include "buttons.hpp"

Input::Input()
{
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
	memset( &pad, 0, sizeof( pad ) );

	/* Button bools */
	for( int i = 0; i < 12; ++i )
		button[i] = false;
}

bool Input::wasPressed( buttonIndex id )
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	int ctrl;

	switch( id )
	{
		case select:
			ctrl = 0x00000001;
			break;
		case start:
			ctrl = 0x00000008;
			break;
		case up:
			ctrl = 0x00000010;
			break;
		case right:
			ctrl = 0x00000020;
			break;
		case down:
			ctrl = 0x00000040;
			break;
		case left:
			ctrl = 0x00000080;
			break;
		case lTrigger:
			ctrl = 0x00000100;
			break;
		case rTrigger:
			ctrl = 0x00000200;
			break;
		case triangle:
			ctrl = 0x00001000;
			break;
		case circle:
			ctrl = 0x00002000;
			break;
		case cross:
			ctrl = 0x00004000;
			break;
		case square:
			ctrl = 0x00008000;
			break;
	}

	if( ( pad.buttons & ctrl ) && !button[id] )
	{
		button[id] = true;
		return true;
	}
	else if( !( pad.buttons & ctrl ) )
	{
		button[id] = false;
	}

	return false;
}