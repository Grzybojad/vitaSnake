#include "player.hpp"

// Initialize variables
Player::Player()
{
	// Initialize position
	xPos = SCREEN_WIDTH / 6;
	yPos = SCREEN_HEIGHT / 2;

	// Set speed
	speed = -PLAYER_MAX_SPEED;

	// Set starting rotation to 90 degrees (pi rad / 2)
	rotation = M_PI / 2;

	// Initialize texture
	//texture = vita2d_load_PNG_file( "app0:/img/body.png" );

	memset( &pad, 0, sizeof( pad ) );

	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
}

// Assign a texture to the object
void Player::setTexture( const char *filename )
{
	texture = vita2d_load_PNG_file( filename );
}


// Handle input
void Player::handleInput()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Calculate rotation
	// DPAD controls
	if ( pad.buttons & SCE_CTRL_LEFT )
		rotation -= PLAYER_ROTATION_SPEED;
	else if ( pad.buttons & SCE_CTRL_RIGHT )
		rotation += PLAYER_ROTATION_SPEED;

	// Analog stick controls with deadzone
	float analogInput = (float)( ( pad.lx - 128.0f ) / 128.0f );
	if( analogInput > ANALOG_DEADZONE || analogInput < -ANALOG_DEADZONE )
	{
		rotation += analogInput * PLAYER_ROTATION_SPEED;
	}
}


// Move the player
void Player::move()
{
	// Calcule player position
	xPos -= sin( rotation ) * speed;
	yPos += cos( rotation ) * speed;

	// Trap the player inside the screen
	if( xPos < PLAYER_HEIGHT / 2 )
		xPos = PLAYER_HEIGHT / 2;
	else if( xPos > SCREEN_WIDTH - PLAYER_HEIGHT / 2 )
		xPos = SCREEN_WIDTH - PLAYER_HEIGHT / 2;
	if( yPos < PLAYER_HEIGHT / 2 )
		yPos = PLAYER_HEIGHT / 2;
	else if( yPos > SCREEN_HEIGHT - PLAYER_HEIGHT / 2 )
		yPos = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;
}


// Tail collision
bool Player::checkCollision( Player part )
{
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );
	if( distance < COLLISION_DISTANCE )
		return true;
	else
		return false;
}

// Move the body parts
void Player::follow( Player part )
{
	// Calculate distance to previous part
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );

	// Rotate to look at previous part
	rotation = atan2( part.get_yPos() - yPos, part.get_xPos() - xPos ) + ( M_PI / 2 );

	// If the distance is too far, move to the correct distance
	if( distance > FOLLOW_DISTANCE )
	{
		xPos += sin( rotation ) * ( distance - FOLLOW_DISTANCE );
		yPos -= cos( rotation ) * ( distance - FOLLOW_DISTANCE );
	}

	// Trap the part inside the screen
	if( xPos < PLAYER_HEIGHT / 2 )
		xPos = PLAYER_HEIGHT / 2;
	else if( xPos > SCREEN_WIDTH - PLAYER_HEIGHT / 2 )
		xPos = SCREEN_WIDTH - PLAYER_HEIGHT / 2;
	if( yPos < PLAYER_HEIGHT / 2 )
		yPos = PLAYER_HEIGHT / 2;
	else if( yPos > SCREEN_HEIGHT - PLAYER_HEIGHT / 2 )
		yPos = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;
}


// Render head or body
void Player::render()
{
	vita2d_draw_texture_rotate( texture, xPos, yPos, rotation );
}


// Get position coordinates
float Player::get_xPos()
{
	return xPos;
}

float Player::get_yPos()
{
	return yPos;
}