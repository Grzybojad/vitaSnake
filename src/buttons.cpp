#include "buttons.hpp"

Input::Input()
{
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
	memset( &pad, 0, sizeof( pad ) );

	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);

	/* Button bools */
	for( int i = 0; i < 12; ++i )
		button[i] = false;
}

bool Input::wasPressed( buttonIndex id )
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	int ctrl;

	// Button checks
	if( id < 12 )
	{
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
	}
	// Analog checks
	else if( id < 16 )
	{
		float lAnalogX = (float)( ( pad.lx - 128.0f ) / 128.0f );
		float lAnalogY = (float)( ( pad.ly - 128.0f ) / 128.0f );

		if( id == lAnalogUp )
		{
			if( ( lAnalogY < -ANALOG_DEADZONE ) && !button[id] )
			{
				button[id] = true;
				return true;
			}
			else if( !( lAnalogY < -ANALOG_DEADZONE ) )
			{
				button[id] = false;
			}
		}
		else if (id == lAnalogDown)
		{
			if ((lAnalogY > ANALOG_DEADZONE) && !button[id])
			{
				button[id] = true;
				return true;
			}
			else if (!(lAnalogY > ANALOG_DEADZONE))
			{
				button[id] = false;
			}
		}
		else if( id == lAnalogLeft )
		{
			if( ( lAnalogX < -ANALOG_DEADZONE ) && !button[id] )
			{
				button[id] = true;
				return true;
			}
			else if( !( lAnalogX < -ANALOG_DEADZONE ) )
			{
				button[id] = false;
			}
		}
		else if( id == lAnalogRight )
		{
			if( ( lAnalogX > ANALOG_DEADZONE ) && !button[id] )
			{
				button[id] = true;
				return true;
			}
			else if( !( lAnalogX > ANALOG_DEADZONE ) )
			{
				button[id] = false;
			}
		}
	}
	// Touch
	else if( id == 16 )
	{
		memcpy( touch_old, touch, sizeof( touch_old ) );
		sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch[ SCE_TOUCH_PORT_FRONT ], 1 );

		if( ( touch[ SCE_TOUCH_PORT_FRONT ].reportNum > 0 ) && !button[ id ] )
		{
			button[id] = true;
			return true;
		}
		else if( touch[ SCE_TOUCH_PORT_FRONT ].reportNum <= 0 )
		{
			button[id] = false;
		}
	}
	else if( id == 17 )
	{
		memcpy( touch_old, touch, sizeof( touch_old ) );
		sceTouchPeek( SCE_TOUCH_PORT_BACK, &touch[ SCE_TOUCH_PORT_BACK ], 1 );

		if( ( touch[ SCE_TOUCH_PORT_BACK ].reportNum > 0 ) && !button[ id ] )
		{
			button[id] = true;
			return true;
		}
		else if( touch[ SCE_TOUCH_PORT_BACK ].reportNum <= 0 )
		{
			button[id] = false;
		}
	}


	return false;
}

bool Input::isTouched()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch[ SCE_TOUCH_PORT_FRONT ], 1 );

	return ( touch[ SCE_TOUCH_PORT_FRONT ].reportNum > 0 );
}

bool Input::isBackTouched()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_BACK, &touch[ SCE_TOUCH_PORT_BACK ], 1 );

	return ( touch[ SCE_TOUCH_PORT_BACK ].reportNum > 0 );
}

int Input::getTouchX()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch[ SCE_TOUCH_PORT_FRONT ], 1 );

	return touch[ SCE_TOUCH_PORT_FRONT ].report[ 0 ].x / 2;
}

int Input::getTouchY()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch[ SCE_TOUCH_PORT_FRONT ], 1 );

	return touch[ SCE_TOUCH_PORT_FRONT ].report[ 0 ].y / 2;
}

int Input::getBackTouchX()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_BACK, &touch[ SCE_TOUCH_PORT_BACK ], 1 );

	return touch[ SCE_TOUCH_PORT_BACK ].report[ 0 ].x / 2;
}

int Input::getBackTouchY()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_BACK, &touch[ SCE_TOUCH_PORT_BACK ], 1 );

	return touch[ SCE_TOUCH_PORT_BACK ].report[ 0 ].y / 2;
}

bool Input::touchToGoBack()
{
	memcpy( touch_old, touch, sizeof( touch_old ) );
	sceTouchPeek( SCE_TOUCH_PORT_FRONT, &touch[ SCE_TOUCH_PORT_FRONT ], 1 );

	int x = touch[ SCE_TOUCH_PORT_FRONT ].report[ 0 ].x / 2;
	int y = touch[ SCE_TOUCH_PORT_FRONT ].report[ 0 ].y / 2;

	if( (x > 660) && (y > 500) )
		return true;

	return false;
}